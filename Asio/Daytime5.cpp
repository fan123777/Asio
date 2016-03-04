#include "Daytime5.h"
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "utils.h"

// A synchronous UDP daytime server
namespace daytime_5
{
	using boost::asio::ip::udp;

	void daytime()
	{
		try
		{
			boost::asio::io_service io_service;
			udp::socket socket(io_service, udp::endpoint(udp::v4(), 13));
			for (;;)
			{
				boost::array<char, 1> recv_buf;
				udp::endpoint remote_endpoint;
				boost::system::error_code error;
				socket.receive_from(boost::asio::buffer(recv_buf),
					remote_endpoint, 0, error);
				if (error && error != boost::asio::error::message_size)
					throw boost::system::system_error(error);
				std::string message = utils::make_daytime_string();
				boost::system::error_code ignored_error;
				socket.send_to(boost::asio::buffer(message),
					remote_endpoint, 0, ignored_error);
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
