#include "Daytime3.h"
#include "utils.h"
#include <iostream>

// An asynchronous TCP daytime server
namespace daytime_3
{
	using namespace utils;

	void daytime()
	{
		try
		{
			boost::asio::io_service io_service;
			tcp_server server(io_service, 2001);
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

