//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <client/ClientSocket.hpp>
#include <redislite/PollManager.hpp>
#include <memory>

namespace redislite
{
	class Client
	{
	public:
		explicit Client(const int iPort, const std::string& iSvrIP);
		void run();
		~Client() = default;

	private:
		std::unique_ptr<ClientSocket> _clientSocket;
		bool readData();
		bool sendData();
		PollManager _pollMgr;
	};
}  // namespace redislite