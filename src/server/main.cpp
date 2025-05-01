//////////////////////////////////////////////////////////////////////////
// RedisLite - An In-Memory Database
// Author: Javed Shaik
// Description: A lightweight, in-memory key-value store similar to Redis.
//////////////////////////////////////////////////////////////////////////

#include <exceptions/InternalError.hpp>
#include <server/Server.hpp>
#include <utils/LoggerMacros.hpp>

void Usage()
{
	std::cout << "Usage: ./redislite_server [Port]" << std::endl;
}

int main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		Usage();
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
		Usage();
		return 1;
	}
	catch (const redislite::InternalError &e)
	{
		LOG_CSL_ERR("Internal error: " << e.what());
	}
	catch (const std::exception &e)
	{
		LOG_CSL_ERR("Error: " << e.what());
		return 1;
	}
	return 0;
}
