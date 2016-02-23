#include <boost/asio.hpp>

using namespace boost::asio;
using namespace std;

void connect_handler(const boost::system::error_code & ec) {
	// here we know we connected successfully
	// if ec indicates success
	int i = 0;
	i++;
}

void asynchronous_client()
{
	io_service service;
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
	ip::tcp::socket sock(service);
	sock.async_connect(ep, connect_handler);
	service.run();


}

void main()
{
	asynchronous_client();
}