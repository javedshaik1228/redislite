//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <client/Client.hpp>

// STD

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/poll.h>
#include <unistd.h>

#include <redislite/Config.hpp>
#include <cstring>
#include <exceptions/InternalError.hpp>
#include <iostream>
#include <utils/LoggerMacros.hpp>

namespace redislite
{

Client::Client(const int iPort, const std::string &iSvrIP)
{
	_clientSocket.setNonBlocking();
	_clientSocket.connectToServer(iSvrIP, iPort, ClientConfig::CNX_WAIT_TIME);
	_pollMgr.addFd(_clientSocket.getFd(), POLLIN | POLLOUT);
}

void Client::run()
{
	while (true)
	{
		int pollcount = _pollMgr.wait(ClientConfig::POLLTIMEOUT);
		if (pollcount < 0)
		{
			throw InternalError("Poll failed: " + std::string(strerror(errno)));
		}
		else if (pollcount == 0)
		{
			LOG_CSL_MSG("Poll timed out.");
			exit(EXIT_SUCCESS);
		}

		std::vector<int> readyFds = _pollMgr.getReadyFds(POLLIN | POLLOUT);
		LOG_CSL_MSG("Ready FDs count: " << readyFds.size());

		for (int fd : _pollMgr.getReadyFds(POLLERR | POLLHUP | POLLNVAL))
		{
			throw InternalError("Poll detected an error: " + std::string(strerror(errno)));
		}

		for (int itrPolloutFd : _pollMgr.getReadyFds(POLLOUT))
		{
			LOG_CSL_MSG("Client Socket ready to send data");
			if (!sendData())
			{
				return;
			}
			_pollMgr.updateEvents(itrPolloutFd, POLLIN);
		}

		for (int itrPollinFd : _pollMgr.getReadyFds(POLLIN))
		{
			LOG_CSL_MSG("Client Socket ready to read data");
			if (!readData())
			{
				return;
			}
			_pollMgr.updateEvents(itrPollinFd, POLLOUT);
		}
	}
}

bool Client::readData()
{
	char buffer[ClientConfig::BUFFER_SIZE] = {0};
	int bytes_read = _clientSocket.readData(buffer, ClientConfig::BUFFER_SIZE);

	if (bytes_read > 0)
	{
		std::cout << buffer;
	}
	else if (bytes_read == 0)
	{
		LOG_MSG("Server disconnected.");
		return false;
	}
	else if (errno != EAGAIN && errno != EWOULDBLOCK)
	{
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
	command += "\r\n";
	_clientSocket.sendData(command);

	return true;
}

}  // namespace redislite