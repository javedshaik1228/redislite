#pragma once

#include <string>

namespace redislite
{
    class Client
    {
    public:
        explicit Client(const int iPort, const std::string &iSvrIP);
        void run();
        ~Client();

    private:
        int _clientFd;
        void setNonBlocking();
        void connectToServer(int iPort, const std::string &iSvrIP);
        bool readData();
        bool sendData();
    };
}