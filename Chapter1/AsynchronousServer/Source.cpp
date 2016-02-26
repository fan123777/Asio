#include "AsynchronousServer.h"
#include <iostream>
#include <boost/bind.hpp>

using namespace boost::asio;
using namespace std;

io_service service;
ip::tcp::endpoint ep(ip::tcp::v4(), 2001); // listen on 2001
ip::tcp::acceptor acc(service, ep);
socket_ptr sock(new ip::tcp::socket(service));

void handle_accept(socket_ptr sock, const boost::system::error_code & err);

void start_accept(socket_ptr sock)
{
	acc.async_accept(*sock, boost::bind(handle_accept, sock, _1));
}

void handle_accept(socket_ptr sock, const boost::system::error_code & err)
{
	if (err) return;
	// at this point, you can read/write to the socket
	socket_ptr sock1(new ip::tcp::socket(service));
	start_accept(sock1);
}

void main()
{
	start_accept(sock);
	service.run();

	//cout << "hello";
	//AsynchronousServer server;
	//server.run();
}