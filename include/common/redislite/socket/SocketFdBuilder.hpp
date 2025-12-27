//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <redislite/socket/PosixSocket.hpp>
#include <memory>

namespace redislite
{
    class SocketFdBuilder
    {
    public:
        static int createSocketFd(int domain, int type, int protocol);
    private:
        static void setNonBlocking(int socketFd);
    };
}  // namespace redislite