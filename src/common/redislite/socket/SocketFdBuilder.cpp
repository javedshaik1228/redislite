//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <redislite/socket/SocketFdBuilder.hpp>
#include <redislite/socket/PosixSocket.hpp>
#include <exceptions/InternalError.hpp>
#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>

namespace redislite
{
    int SocketFdBuilder::createSocketFd(int domain, int type, int protocol)
    {
        int aSocketFd = socket(domain, type, protocol);
        if (aSocketFd < 0)
        {
            throw InternalError("Socket creation failed");
        }
        setNonBlocking(aSocketFd);
        return aSocketFd;
    }

    void SocketFdBuilder::setNonBlocking(int socketFd)
    {
        int flags = fcntl(socketFd, F_GETFL, 0);
        if (flags < 0 || fcntl(socketFd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            throw InternalError("Failed to set non-blocking mode");
        }
    }
}