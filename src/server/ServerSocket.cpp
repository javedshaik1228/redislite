//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <server/ServerSocket.hpp>
#include <redislite/socket/SocketFdBuilder.hpp>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <exceptions/InternalError.hpp>
#include <utils/LoggerMacros.hpp>

namespace redislite
{

	void ServerSocket::bindAndListen(int port, int backlog)
	{
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);

		if (bind(_socketFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
		{
			throw InternalError("Bind failed");
		}

		if (listen(_socketFd, backlog) < 0)
		{
			throw InternalError("Listen failed");
		}
	}

	int ServerSocket::acceptClient()
	{
		sockaddr_in clientAddr{};
		socklen_t addrLen = sizeof(clientAddr);

		int clientFd = accept(_socketFd, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
		if (clientFd < 0)
		{
			throw InternalError("Accept failed");
		}

		char clientIP[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN) != nullptr)
		{
			LOG_CSL_MSG("Accepted connection from client IP: " << clientIP);
		}
		return clientFd;
	}

}  // namespace redislite
