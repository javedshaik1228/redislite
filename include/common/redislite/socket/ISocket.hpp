//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

namespace redislite
{
    class ISocket
    {
    public:
        virtual int readData(char* buffer, int bufferSize) = 0;
        virtual int sendData(const std::string& data) = 0;
        virtual int getFd() const = 0;
        virtual void closeSocket() = 0;
        virtual ~ISocket() = default;
    };
}  // namespace redislite