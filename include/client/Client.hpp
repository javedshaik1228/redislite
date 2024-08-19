#pragma once

namespace redislite
{
    class Client
    {
    public:
        explicit Client(const int iPort, const char *iSvrIP);
        void run();
        ~Client();

    private:
        int _clientFd;
        void setNonBlocking();
        void connectToServer(int iPort, const char *iSvrIP);
        bool readData();
        bool sendData();
    };
}