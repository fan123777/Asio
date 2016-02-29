#include "Daytime6.h"
#include <iostream>

namespace daytime_6
{
	void daytime()
	{
		try
		{
			boost::asio::io_service io_service;
			udp_server server(io_service);
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
