//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>

#include <redislite/socket/SocketFactory.hpp>

#include <redislite/CmdHandler.hpp>
#include <redislite/Config.hpp>

#include <StringStore.hpp>
#include <exceptions/DataError.hpp>
#include <exceptions/InternalError.hpp>
#include <server/Server.hpp>
#include <utils/LoggerMacros.hpp>


namespace redislite
{

	Server::Server(const int iPort) : _serverSocket(std::move(SocketFactory::createServerSocket()))
	{
		_serverSocket->bindAndListen(iPort, ServerConfig::BACKLOG);

		_pollMgr.addFd(_serverSocket->getFd(), POLLIN);
		LOG_CSL_MSG("Server is listening on port: " << iPort);
	}

	void Server::acceptNewClient()
	{
		int clientFd = _serverSocket->acceptClient();
		_pollMgr.addFd(clientFd, POLLIN);
		LOG_CSL_MSG("Client connected: " << clientFd);
	}

	void Server::handleClient(int iClientFd)
	{
		char aBuffer[ServerConfig::BUFFER_SIZE] = { 0 };
		int bytes_read = read(iClientFd, aBuffer, ServerConfig::BUFFER_SIZE);

		if (bytes_read <= 0)
		{
			if (bytes_read == 0)
			{
				LOG_CSL_MSG("Client disconnected.");
			}
			else if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				throw InternalError("Read failed: " + std::string(strerror(errno)));
			}

			_pollMgr.removeFd(iClientFd);
			close(iClientFd);
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
			int pollCount = _pollMgr.wait(ServerConfig::POLLTIMEOUT);
			if (pollCount < 0)
			{
				throw InternalError("Poll failed: " + std::string(strerror(errno)));
			}

			for (const auto& itrPollInFd : _pollMgr.getReadyFds(POLLIN))
			{
				if (itrPollInFd == _serverSocket->getFd())
				{
					acceptNewClient();
				}
				else
				{
					handleClient(itrPollInFd);
				}
			}

			for (const auto& itrPollfd : _pollMgr.getReadyFds(POLLERR | POLLHUP | POLLNVAL))
			{
				LOG_CSL_MSG("Client error or hangup: " << itrPollfd);
				_pollMgr.removeFd(itrPollfd);
				close(itrPollfd);
			}
		}
	}

	Server::~Server()
	{
		_pollMgr.removeFd(_serverSocket->getFd());
	}

}  // namespace redislite