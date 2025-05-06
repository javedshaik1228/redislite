//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <redislite/SocketHandler.hpp>
#include <string>

namespace redislite
{

class ClientSocket : public SocketHandler
{
   public:
	ClientSocket();
	void connectToServer(const std::string& ip, int port, int timeoutMs);
};

}  // namespace redislite