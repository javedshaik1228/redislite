//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <client/Client.hpp>
#include <exceptions/InternalError.hpp>
#include <utils/LoggerMacros.hpp>

void Usage()
{
	std::cout << "Usage: ./redislite_client [Server IP] [Port]" << std::endl;
}

int main(int argc, const char *argv[])
{
	if (argc != 3)
	{
		Usage();
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
		Usage();
		return 1;
	}
	catch (const redislite::InternalError &e)
	{
		LOG_CSL_ERR("Internal error: " << e.what());
	}
	catch (const std::exception &e)
	{
		LOG_CSL_ERR("Client error: " << e.what());
		return 1;
	}
	return 0;
}
