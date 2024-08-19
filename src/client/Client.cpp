#include <client/Client.hpp>

// STD

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

//

#include <exceptions/InternalError.hpp>
#include <utils/LoggerMacros.hpp>
#include <Config.hpp>

namespace redislite
{

    Client::Client(const int iPort, const char *iSvrIP)
    {
        // create socket
        _clientFd = socket(AF_INET, SOCK_STREAM, 0);

        if (_clientFd <= 0)
        {
            throw InternalError("cannot create socket");
        }

        setNonBlocking();
        connectToServer(iPort, iSvrIP);
    }

    void Client::setNonBlocking()
    {
        int flags = fcntl(_clientFd, F_GETFL, 0);
        if (flags < 0 || fcntl(_clientFd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            throw InternalError("Failed to set socket to non-blocking mode");
        }
    }

    void Client::connectToServer(int iPort, const char *iSvrIP)
    {
        sockaddr_in aServerAddr{};
        aServerAddr.sin_family = AF_INET;
        aServerAddr.sin_port = htons(iPort);

        if (inet_pton(AF_INET, iSvrIP, &aServerAddr.sin_addr) <= 0)
        {
            throw InternalError("Invalid address/Address not supported");
        }

        int aCnxResult = connect(_clientFd, reinterpret_cast<sockaddr *>(&aServerAddr), sizeof(aServerAddr));
        if (aCnxResult < 0 && errno != EINPROGRESS)
        {
            throw InternalError("Connection failed: " + std::string(strerror(errno)));
        }
        else if (errno == EINPROGRESS)
        {
            // _clientFd socket flag O_NONBLOCK is set, meaning it is a non blocking socket.
            // Hence the connection is not immediate and we should wait till it's connected.
            // ref: https://man7.org/linux/man-pages/man2/connect.2.html -> EINPROGRESS

            pollfd aPollFd = {_clientFd, POLLOUT, 0};
            poll(&aPollFd, 1, ClientConfig::CNX_WAIT_TIME); // wait

            if (aPollFd.revents & POLLOUT)
            {
                int error = 0;
                socklen_t len = sizeof(error);
                if (getsockopt(_clientFd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0)
                {
                    throw InternalError("Connection failed: " + std::string(strerror(errno)));
                }
            }
            else
            {
                throw InternalError("Connection Timeout: " + std::string(strerror(errno)));
            }
        }
    }

    void Client::run()
    {
        // init by checking for both read/ write polls
        pollfd aPollFd = {_clientFd, POLLIN | POLLOUT, 0};

        // Event polling, do read/ write only when socket is ready to.

        while (true)
        {
            int pollResult = poll(&aPollFd, 1, ClientConfig::POLLTIMEOUT);

            if (pollResult < 0)
            {
                throw InternalError("Poll failed: " + std::string(strerror(errno)));
            }
            else if (pollResult == 0)
            {
                LOG_CSL_MSG("Poll timeout: " << strerror(errno));
                continue;
            }

            if (aPollFd.revents & POLLOUT)
            {
                LOG_CSL_MSG("socket ready to send data");
                // client socket ready to send data
                if (!sendData())
                {
                    // empty write, break loop.
                    break;
                }

                // check for read after sending data
                aPollFd.events = POLLIN;
            }

            if (aPollFd.revents & POLLIN)
            {
                LOG_CSL_MSG("socket ready to read data");
                // client socket ready to read data
                if (!readData())
                {
                    // empty read, break loop.
                    break;
                }

                // check for write after reading data
                aPollFd.events = POLLOUT;
            }

            if (aPollFd.revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                throw InternalError("Poll detected an error " + std::string(strerror(errno)));
            }
        }
    }

    bool Client::readData()
    {
        char buffer[ClientConfig::BUFFER_SIZE] = {0};
        int bytes_read = read(_clientFd, buffer, ClientConfig::BUFFER_SIZE);
        if (bytes_read > 0)
        {
            std::cout << buffer;
        }
        else if (bytes_read == 0)
        {
            LOG_MSG("Server disconnected.");
            return false;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // no data to read, continue polling
                return true;
            }
            // else
            throw InternalError("Read failed: " + std::string(strerror(errno)));
        }
        return true;
    }

    bool Client::sendData()
    {
        std::string command;
        std::getline(std::cin, command);

        if (command.empty())
        {
            LOG_CSL_MSG("Closing connection...");
            return false;
        }

        LOG_MSG("Client: sent command: " << command);

        // Append CRLF
        command += "\r\n";
        send(_clientFd, command.c_str(), command.size(), 0);

        return true;
    }

    Client::~Client()
    {
        if (_clientFd > 0)
            close(_clientFd);
    }
}