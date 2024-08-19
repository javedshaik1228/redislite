#include <server/Server.hpp>

#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

#include <utils/LoggerMacros.hpp>
#include <StringStore.hpp>
#include <CmdHandler.hpp>
#include <Config.hpp>

#include <exceptions/InternalError.hpp>
#include <exceptions/DataError.hpp>

namespace redislite
{
    Server::Server(const int iPort)
    {
        // create a socket
        _serverFd = socket(AF_INET, SOCK_STREAM, 0);

        if (_serverFd <= 0)
        {
            throw InternalError("Socket failed");
        }

        setNonBlocking();

        // prepare the sockaddr_in struct
        sockaddr_in hints{};
        hints.sin_family = AF_INET;
        hints.sin_addr.s_addr = INADDR_ANY;
        hints.sin_port = htons(iPort);

        int opt = 1;
        if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            throw InternalError("setsockopt failed");
        }

        // bind the port
        if (bind(_serverFd, reinterpret_cast<sockaddr *>(&hints), sizeof(hints)) < 0)
        {
            throw InternalError("Bind failed");
        }

        // Listen
        if (listen(_serverFd, ServerConfig::BACKLOG) < 0)
        {
            throw InternalError("Listen failed");
        }

        _pollFds.push_back({_serverFd, POLLIN, 0});
        LOG_CSL_MSG("Server is listening on port: " << iPort);
    }

    void Server::setNonBlocking()
    {
        int flags = fcntl(_serverFd, F_GETFL, 0);
        if (flags < 0 || fcntl(_serverFd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            throw InternalError("Failed to set socket to non-blocking mode");
        }
    }

    void Server::acceptNewClient()
    {
        sockaddr_in aClientAddress{};
        socklen_t aAddrLen = sizeof(sockaddr_in);

        int clientFd = accept(_serverFd, reinterpret_cast<sockaddr *>(&aClientAddress), &aAddrLen);
        if (clientFd < 0)
        {
            throw InternalError("Accept failed: " + std::string(std::strerror(errno)));
        }

        // a new client socket fd has been added to poll
        LOG_CSL_MSG("add new clientFd: " << clientFd);
        _pollFds.push_back({clientFd, POLLIN, 0});
    }

    void Server::removeClient(size_t iIdx)
    {
        close(_pollFds[iIdx].fd);
        _pollFds.erase(_pollFds.begin() + iIdx);
    }

    void Server::handleClient(int iClientFd)
    {
        char aBuffer[ServerConfig::BUFFER_SIZE] = {0};
        int bytes_read = read(iClientFd, aBuffer, ServerConfig::BUFFER_SIZE);

        if (bytes_read <= 0)
        {
            if (bytes_read == 0)
            {
                // if cmd is empty, then exit
                LOG_CSL_MSG("Client disconnected.");
            }
            else if (bytes_read < 0)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    throw InternalError("Read failed: " + std::string(strerror(errno)));
                }
            }
            close(iClientFd);

            // remove this fd from poll vector
            auto newEndItr = (std::remove_if(_pollFds.begin(), _pollFds.end(),
                                             [iClientFd](const pollfd &pfd)
                                             { return pfd.fd == iClientFd; }),
                              _pollFds.end());
            _pollFds.erase(newEndItr, _pollFds.end());
            return;
        }

        LOG_CSL_MSG("Received command from " << iClientFd << " fd : " << aBuffer);

        std::string response = CmdHandler::processCommand(aBuffer);
        send(iClientFd, response.c_str(), response.size(), 0);
    }

    void Server::run()
    {
        while (true)
        {
            int pollCount = poll(_pollFds.data(), _pollFds.size(), ServerConfig::POLLTIMEOUT);
            if (pollCount < 0)
            {
                throw InternalError("Poll failed: " + std::string(strerror(errno)));
            }

            // _pollFds[0] = server fd
            if (_pollFds[0].revents & POLLIN)
            {
                // server socket ready to read.
                acceptNewClient();
            }

            for (size_t itr = 1; itr < _pollFds.size(); ++itr)
            {
                if (_pollFds[itr].revents & (POLLERR | POLLHUP | POLLNVAL))
                {
                    removeClient(itr);
                    --itr; // Adjust index after removal
                }
                else if (_pollFds[itr].revents & POLLIN)
                {
                    // Client will always initiate the transaction when it's fd is ready to read.
                    // Client has sent data and is ready to read.
                    handleClient(_pollFds[itr].fd);
                }
            }
        }
    }

    Server::~Server()
    {
        if (_serverFd > 0)
            close(_serverFd);

        for (const auto &clientFd : _pollFds)
        {
            if (clientFd.fd > 0)
            {
                close(clientFd.fd);
            }
        }
    }
}