//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////


#include <redislite/socket/SocketFdBuilder.hpp>
#include <redislite/socket/SocketFactory.hpp>
#include <exceptions/InternalError.hpp>

#include <stdexcept>

namespace redislite
{
    std::unique_ptr<ClientSocket> SocketFactory::createClientSocket(int domain, int type, int protocol)
    {
        int aSocketFd = SocketFdBuilder::createSocketFd(domain, type, protocol);
        return std::make_unique<ClientSocket>(aSocketFd);
    }

    std::unique_ptr<ServerSocket> SocketFactory::createServerSocket(int domain, int type, int protocol)
    {
        int aSocketFd = SocketFdBuilder::createSocketFd(domain, type, protocol);
        int opt = 1;
        if (setsockopt(aSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            throw InternalError("setsockopt reuse address failed");
        }
        return std::make_unique<ServerSocket>(aSocketFd);
    }
}