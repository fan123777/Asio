#include "AsynchronousServer.h"
#include <boost/bind.hpp>:

using namespace boost::asio;
using namespace std;
//typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

AsynchronousServer::AsynchronousServer()
{
	io_service* service = new io_service();
	mService.reset(service);
	ip::tcp::endpoint ep(ip::tcp::v4(), 2001); // listen on 2001
	ip::tcp::acceptor* acc = new ip::tcp::acceptor(*mService, ep);
	mAcc.reset(acc);
}

void AsynchronousServer::run()
{
	socket_ptr sock(new ip::tcp::socket(*mService));
	start_accept(sock);
	mService->run();
}

void AsynchronousServer::start_accept(socket_ptr sock)
{
	// mAcc->async_accept(*sock, boost::bind(handle_accept, sock, _1));
	mAcc->async_accept(*sock, bind(&AsynchronousServer::handle_accept, this, sock));
}

void AsynchronousServer::handle_accept(socket_ptr sock)
{

	// at this point, you can read/write to the socket
	// socket_ptr sock(new ip::tcp::socket(*mService));
	// start_accept(sock);
}

