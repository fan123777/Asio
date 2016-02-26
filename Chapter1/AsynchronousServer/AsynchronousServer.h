#pragma once

#include <boost/asio.hpp>
#include <memory>

typedef std::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class AsynchronousServer
{
public:
	AsynchronousServer();
	void run();
private:
	void start_accept(socket_ptr sock);
	void handle_accept(socket_ptr sock);

	std::shared_ptr<boost::asio::io_service> mService;
	std::shared_ptr<boost::asio::ip::tcp::acceptor> mAcc;
};

