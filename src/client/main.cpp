#include <utils/LoggerMacros.hpp>
#include <client/Client.hpp>

int main()
{
    try
    {
        redislite::Client client(8080, "127.0.0.1");
        client.run();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Client error: " << e.what());
        return 1;
    }
    return 0;
}
