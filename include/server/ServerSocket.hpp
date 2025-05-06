//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <redislite/SocketHandler.hpp>

namespace redislite
{

class ServerSocket : public SocketHandler
{
   public:
	ServerSocket();
	void setReuseAddr();
	void bindAndListen(int port, int backlog);
	int acceptClient();
};

}  // namespace redislite