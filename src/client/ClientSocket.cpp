//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <client/ClientSocket.hpp>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <exceptions/InternalError.hpp>

namespace redislite
{
	void ClientSocket::connectToServer(const std::string& ip, int port, int timeoutMs)
	{
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
		{
			throw InternalError("Invalid IP address");
		}

		int result = connect(_socketFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
		if (result < 0 && errno != EINPROGRESS)
		{
			throw InternalError("Connection failed: " + std::string(strerror(errno)));
		}
		else if (errno == EINPROGRESS)
		{
			// _clientFd socket flag O_NONBLOCK is set, meaning it is a non blocking socket.
			// Hence the connection is not immediate and we should wait till it's connected.
			// ref: https://man7.org/linux/man-pages/man2/connect.2.html -> EINPROGRESS

			pollfd pfd = { _socketFd, POLLOUT, 0 };
			int pollRes = poll(&pfd, 1, timeoutMs);
			if (pollRes <= 0 || !(pfd.revents & POLLOUT))
			{
				throw InternalError("Connection timeout or error");
			}
		}
	}

}  // namespace redislite
