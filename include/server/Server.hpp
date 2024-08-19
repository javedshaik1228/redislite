#pragma once
#include <string>
#include <poll.h>
#include <vector>

namespace redislite
{
    class Server
    {
    public:
        explicit Server(const int iPort);
        void run();
        ~Server();

    private:
        int _serverFd;
        void handleClient(int iClientFd);
        void acceptNewClient();
        void setNonBlocking();
        void removeClient(size_t iIdx);

        std::vector<struct pollfd> _pollFds;
    };

} // namespace redistlite
