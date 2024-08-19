#include <utils/LoggerMacros.hpp>
#include <client/Client.hpp>

void usage()
{
    std::cout << "Usage: ./redislite_client [Server IP] [Port]" << std::endl;
}

int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        usage();
        return 1;
    }

    try
    {
        std::string ip = argv[1];
        int port = std::stoi(argv[2]);

        redislite::Client client(port, ip.c_str());
        client.run();
    }
    catch (const std::invalid_argument &ie)
    {
        LOG_CSL_ERR("Invalid args: " << ie.what());
        usage();
        return 1;
    }
    catch (const std::exception &e)
    {
        LOG_CSL_ERR("Client error: " << e.what());
        return 1;
    }
    return 0;
}
