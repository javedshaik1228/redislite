//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <sys/socket.h>
#include <string>

namespace redislite
{

class SocketHandler
{
   public:
	explicit SocketHandler(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	virtual ~SocketHandler();

	void setNonBlocking();
	int readData(char* buffer, int bufferSize);
	int sendData(const std::string& data);
	int getFd() const;
	void closeSocket();

   protected:
	int _socketFd;
};

}  // namespace redislite