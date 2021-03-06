#include "Timer1.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// Using a timer synchronously
namespace timer_1
{
	void sync_timer()
	{
		boost::asio::io_service io;
		boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
		t.wait();
		std::cout << "Hello, world!\n";
	}
}
