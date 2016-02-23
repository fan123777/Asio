#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>

using namespace std;
using namespace boost::asio;
typedef shared_ptr<ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock) {
	while (true) {
		char data[512];
		size_t len = sock->read_some(buffer(data));
		if (len > 0)
		{
			cout << data << endl;
			write(*sock, buffer("ok", 2));
		}
			
	}
}

void synchronous_server()
{
	io_service service;
	ip::tcp::endpoint ep(ip::tcp::v4(), 2001); // listen on 2001
	ip::tcp::acceptor acc(service, ep);
	while (true) {
		socket_ptr sock(new ip::tcp::socket(service));
		acc.accept(*sock);
		thread t(bind(client_session, sock));
		t.detach();
	}
}

void main()
{
	synchronous_server();
}