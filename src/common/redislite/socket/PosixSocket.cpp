//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <redislite/socket/PosixSocket.hpp>

namespace redislite
{
	PosixSocket::~PosixSocket()
	{
		closeSocket();
	}

	int PosixSocket::readData(char* buffer, int bufferSize)
	{
		return read(_socketFd, buffer, bufferSize);
	}

	int PosixSocket::sendData(const std::string& data)
	{
		return send(_socketFd, data.c_str(), data.size(), 0);
	}

	int PosixSocket::getFd() const
	{
		return _socketFd;
	}

	void PosixSocket::closeSocket()
	{
		if (_socketFd >= 0)
		{
			close(_socketFd);
			_socketFd = -1;
		}
	}

}  // namespace redislite