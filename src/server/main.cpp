#include <utils/LoggerMacros.hpp>
#include <server/Server.hpp>

void usage()
{
    std::cout << "Usage: ./redislite_server [Port]" << std::endl;
}

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        usage();
        return 1;
    }

    try
    {
        int port = std::stoi(argv[1]);

        redislite::Server server(port);
        server.run();
    }
    catch (const std::invalid_argument &ie)
    {
        LOG_CSL_ERR("Invalid args: " << ie.what());
        usage();
        return 1;
    }
    catch (const std::exception &e)
    {
        LOG_CSL_ERR("Error: " << e.what());
        return 1;
    }
    return 0;
}
