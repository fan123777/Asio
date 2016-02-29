#include "Daytime4.h"
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace daytime_4
{
	using boost::asio::ip::udp;

	void daytime()
	{
		try
		{
			boost::asio::io_service io_service;
			udp::resolver resolver(io_service);
			udp::resolver::query query(udp::v4(), "localhost", "daytime");
			udp::endpoint receiver_endpoint = *resolver.resolve(query);
			udp::socket socket(io_service);
			socket.open(udp::v4());
			boost::array<char, 1> send_buf = { 0 };
			socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
			boost::array<char, 128> recv_buf;
			udp::endpoint sender_endpoint;
			size_t len = socket.receive_from(
				boost::asio::buffer(recv_buf), sender_endpoint);
			std::cout.write(recv_buf.data(), len);
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
