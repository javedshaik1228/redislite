//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <redislite/socket/ISocket.hpp>
#include <string>

namespace redislite
{
    class PosixSocket : public ISocket
    {
    public:
        explicit PosixSocket(int iSocketFd) : _socketFd(iSocketFd) {};

        int readData(char* buffer, int bufferSize) override;
        int sendData(const std::string& data) override;
        int getFd() const override;
        void closeSocket() override;

        virtual ~PosixSocket();
    private:
        PosixSocket() = delete;
        PosixSocket(const PosixSocket&) = delete;
        PosixSocket& operator=(const PosixSocket&) = delete;

    protected:
        int _socketFd;
    };
}  // namespace redislite