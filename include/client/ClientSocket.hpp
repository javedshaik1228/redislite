//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <redislite/socket/PosixSocket.hpp>
#include <string>

namespace redislite
{

	class ClientSocket : public PosixSocket
	{
	public:
		explicit ClientSocket(int iSocketFd) : PosixSocket(iSocketFd) {};
		ClientSocket() = delete;
		void connectToServer(const std::string& ip, int port, int timeoutMs);
	};

}  // namespace redislite