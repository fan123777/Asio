#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// Using a member function as a handler
namespace timer_4
{
	void async_timer();

	class printer
	{
	public:
		printer(boost::asio::io_service& io)
			: timer_(io, boost::posix_time::seconds(1)),
			count_(0)
		{
			timer_.async_wait(boost::bind(&printer::print, this));
		}

		~printer()
		{
			std::cout << "Final count is " << count_ << "\n";
		}

		void print()
		{
			if (count_ < 5)
			{
				std::cout << count_ << "\n";
				++count_;
				timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
				timer_.async_wait(boost::bind(&printer::print, this));
			}
		}

	private:
		boost::asio::deadline_timer timer_;
		int count_;
	};
}
