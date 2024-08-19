#pragma once

namespace redislite
{
    namespace ServerConfig
    {
        constexpr int BUFFER_SIZE = 1024; // max buffer size
        constexpr int BACKLOG = 10;       // how many pending cnx the queue can hold
        constexpr int POLLTIMEOUT = -1;   // time in ms that poll() should block waiting
                                          // for a fd to become ready. -1 = inf timeout
    }

    namespace ClientConfig
    {
        constexpr int BUFFER_SIZE = 1024;   // max buffer size
        constexpr int CNX_WAIT_TIME = 2000; // in milli seconds
        constexpr int POLLTIMEOUT = -1;     // time in ms that poll() should block waiting
                                            // for a fd to become ready. -1 = inf timeout
    }

}