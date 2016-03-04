#include "Daytime7.h"
#include "Daytime6.h"
#include "Daytime3.h"
#include <boost/asio.hpp>

using namespace daytime_3;
using namespace daytime_6;

// A combined TCP/UDP asynchronous server
namespace daytime_7
{
	void daytime()
	{
		try
		{
			boost::asio::io_service io_service;
			tcp_server server1(io_service, 2001);
			udp_server server2(io_service);
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
