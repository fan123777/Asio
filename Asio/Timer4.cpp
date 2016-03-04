#include "Timer4.h"

// Using a member function as a handler
namespace timer_4
{
	void async_timer()
	{
		boost::asio::io_service io;
		printer p(io);
		io.run();
	}
}
