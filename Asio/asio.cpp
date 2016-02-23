#include "asio.h"
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <thread>
#include <iostream>

using namespace boost::asio;
using namespace std;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;


namespace boost_asio
{
	// Getting Started with Boost.Asio
	namespace chapter1
	{
		void main()
		{
			// synchronousServer();
			// synchronousClient();


			// !!!
			// - async server
			// - 3 different examples
		}

		void synchronousClient()
		{
			try
			{
				io_service service;
				ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
				ip::tcp::socket sock(service);
				sock.connect(ep);
			}
			catch (...)
			{

			}
		}

		void client_session(socket_ptr sock) {
			while (true) {
				char data[512];
				size_t len = sock->read_some(buffer(data));
				if (len > 0)
					write(*sock, buffer("ok", 2));
			}
		}

		void synchronousServer()
		{
			try
			{
				io_service service;
				ip::tcp::endpoint ep(ip::tcp::v4(), 2001); // listen on 2001
				ip::tcp::acceptor acc(service, ep);
				while (true) {
					socket_ptr sock(new ip::tcp::socket(service));
					acc.accept(*sock);
					thread(std::bind(client_session, sock));
				}
			}
			catch (...)
			{

			}
		}

		void connect_handler(const boost::system::error_code & ec) {
			// here we know we connected successfully
			// if ec indicates success
		}

		void asynchronousClient()
		{
			io_service service;
			ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
			ip::tcp::socket sock(service);
			sock.async_connect(ep, connect_handler);
			service.run();
		}

		void start_accept(socket_ptr sock)
		{
			// acc.async_accept(*sock, bind(handle_accept, sock, _1));
		}

		void handle_accept(socket_ptr sock, const boost::system::error_code & err) {
			if (err) return;
			// at this point, you can read/write to the socket
			// socket_ptr sock(new ip::tcp::socket(service));
			start_accept(sock);
		}

		void asynchronousServer()
		{
			io_service service;
			ip::tcp::endpoint ep(ip::tcp::v4(), 2001); // listen on 2001
			ip::tcp::acceptor acc(service, ep);
			socket_ptr sock(new ip::tcp::socket(service));
			start_accept(sock);
			service.run();
		}

		void signal_handler(const boost::system::error_code & err, int signal)
		{
			// log this, and terminate application
		}

		void signal()
		{
			io_service service;
			boost::asio::signal_set sig(service, SIGINT, SIGTERM);
			sig.async_wait(signal_handler);
		}

		void serialPort()
		{
			io_service service;
			serial_port sp(service, "COM7");

			serial_port::baud_rate rate(9600);
			sp.set_option(rate);

			char data[512];
			read(sp, buffer(data, 512));

			HANDLE h;/* = ::OpenFile(...);*/
			windows::stream_handle sh(service, h);
			
			// read(h, buffer(data, 512));

			/*posix::stream_descriptor sd_in(service, ::dup(STDIN_FILENO));
			char data[512];
			read(sd_in, buffer(data, 512));*/
		}

		void deadline_handler(const boost::system::error_code &) {
			// std::cout << (mRead ? "read successfully" : "read failed") << std::endl;
		}

		void read_handler(const boost::system::error_code &) {
			// mRead = true;
		}

		void deadlineOperation()
		{
			io_service service;

			ip::tcp::socket sock(service);
			// mRead = false;
			char data[512];

			// sock.async_read_some(buffer(data, 512), read_handler);
			deadline_timer t(service, boost::posix_time::milliseconds(100));
			t.async_wait(&deadline_handler);
			service.run();

			// sync timer
			deadline_timer t1(service, boost::posix_time::milliseconds(500));
			t1.wait();
		}

		void service1()
		{
			io_service service_;
			// all the socket operations are handled by service_
			ip::tcp::socket sock1(service_);
			// all the socket operations are handled by service_
			ip::tcp::socket sock2(service_);
			ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
			sock1.async_connect(ep, connect_handler);
			sock2.async_connect(ep, connect_handler);
			deadline_timer t(service_, boost::posix_time::seconds(5));
			// t.async_wait(timeout_handler);
			service_.run();
		}

		void service2()
		{
			io_service service_;
			ip::tcp::socket sock1(service_);
			ip::tcp::socket sock2(service_);
			ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
			sock1.async_connect(ep, connect_handler);
			sock2.async_connect(ep, connect_handler);
			deadline_timer t(service_, boost::posix_time::seconds(5));
			// t.async_wait(timeout_handler);
			for (int i = 0; i < 5; ++i)
				thread(run_service);
			service_.run();
		}

		void run_service(int idx)
		{

		}

		void service3()
		{
			io_service service_[2];
			ip::tcp::socket sock1(service_[0]);
			ip::tcp::socket sock2(service_[1]);
			ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
			sock1.async_connect(ep, connect_handler);
			sock2.async_connect(ep, connect_handler);
			deadline_timer t(service_[0], boost::posix_time::seconds(5));
			// t.async_wait(timeout_handler);
			for (int i = 0; i < 2; ++i)
				thread(bind(run_service, i));
		}
	}

	// Boost.Asio Fundamentals
	namespace chapter2
	{
		void main()
		{

		}
	}
}

