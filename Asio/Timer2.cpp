#include "Timer2.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// Using a timer asynchronously
namespace timer_2
{
	void print(const boost::system::error_code& /*e*/)
	{
		std::cout << "Hello, world!\n";
	}

	void async_timer()
	{
		boost::asio::io_service io;
		boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
		t.async_wait(print);
		io.run();
	}
}

