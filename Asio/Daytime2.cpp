#include "Daytime2.h"
#include "utils.h"
#include <iostream>
#include <boost/asio.hpp>

namespace daytime_2
{
	using boost::asio::ip::tcp;
	using namespace utils;

	void daytime()
	{
		try
		{
			boost::asio::io_service io_service;
			tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 2001));
			for (;;)
			{
				tcp::socket socket(io_service);
				acceptor.accept(socket);
				std::string message = make_daytime_string();
				boost::system::error_code ignored_error;
				boost::asio::write(socket, boost::asio::buffer(message),
					boost::asio::transfer_all(), ignored_error);
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

