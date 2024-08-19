#include <utils/LoggerMacros.hpp>
#include <server/Server.hpp>

int main()
{
    try
    {
        redislite::Server server(8080);
        server.run();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Error: " << e.what());
        return 1;
    }
    return 0;
}
