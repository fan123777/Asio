#include "Timer5.h"
#include <thread>

namespace timer_5
{
	void async_timer()
	{
		boost::asio::io_service io;
		printer p(io);
		std::thread t(boost::bind(&boost::asio::io_service::run, &io));
		io.run();
		t.join();
	}
}
