//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <unistd.h>
#include <redislite/SocketHandler.hpp>
#include <exceptions/InternalError.hpp>

namespace redislite
{

SocketHandler::SocketHandler(int domain, int type, int protocol)
{
	_socketFd = socket(domain, type, protocol);
	if (_socketFd <= 0)
	{
		throw InternalError("Socket creation failed");
	}
}

SocketHandler::~SocketHandler()
{
	closeSocket();
}

void SocketHandler::setNonBlocking()
{
	int flags = fcntl(_socketFd, F_GETFL, 0);
	if (flags < 0 || fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		throw InternalError("Failed to set non-blocking mode");
	}
}

int SocketHandler::readData(char* buffer, int bufferSize)
{
	return read(_socketFd, buffer, bufferSize);
}

int SocketHandler::sendData(const std::string& data)
{
	return send(_socketFd, data.c_str(), data.size(), 0);
}

int SocketHandler::getFd() const
{
	return _socketFd;
}

void SocketHandler::closeSocket()
{
	if (_socketFd > 0)
	{
		close(_socketFd);
		_socketFd = -1;
	}
}

}  // namespace redislite