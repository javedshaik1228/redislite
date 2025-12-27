//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <client/ClientSocket.hpp>
#include <server/ServerSocket.hpp>
#include <sys/socket.h>
#include <memory>

namespace redislite
{
    class SocketFactory
    {
    public:
        static std::unique_ptr<ClientSocket> createClientSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        static std::unique_ptr<ServerSocket> createServerSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
    };

}