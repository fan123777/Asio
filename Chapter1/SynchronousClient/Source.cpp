#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace boost::asio;

void synchronous_client()
{
	try
	{
		io_service service;
		ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
		ip::tcp::socket sock(service);
		sock.connect(ep);
		cout << "connection succes\n";

		write(sock, buffer("hello", 5));

		char data[512];
		size_t len = sock.read_some(buffer(data));
		cout << data << endl;
	}
	catch (...)
	{
		cout << "connection fail\n";
	}
	
}

void main()
{
	synchronous_client();
}