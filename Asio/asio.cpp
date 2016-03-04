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
			// !!!
			// practice with all of material.
		}

		void connectingRelatedFunctions()
		{
			io_service service;
			ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 80);
			ip::tcp::socket sock(service);
			sock.open(ip::tcp::v4());
			sock.connect(ep);
			sock.write_some(buffer("GET /index.html\r\n"));
			char buff[1024]; sock.read_some(buffer(buff, 1024));
			sock.shutdown(ip::tcp::socket::shutdown_receive);
			sock.close();
		}

		void readWriteFunctions()
		{
			io_service service;
			ip::tcp::socket sock(service);
			char buff[1024];
			sock.receive(buffer(buff), ip::tcp::socket::message_peek);
			memset(buff, 1024, 0);
			// re-reads what was previously read
			sock.receive(buffer(buff));
		}

		void writeReadSyncTCP()
		{
			io_service service;
			ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 80);
			ip::tcp::socket sock(service);
			sock.connect(ep);
			sock.write_some(buffer("GET /index.html\r\n"));
			std::cout << "bytes available " << sock.available() << std::endl;
			char buff[512];
			size_t read = sock.read_some(buffer(buff));
		}

		void writeReadSyncUDP()
		{
			io_service service;
			ip::udp::socket sock(service);
			sock.open(ip::udp::v4());
			ip::udp::endpoint receiver_ep(ip::address::from_string("87.248.112.181"), 80);
			sock.send_to(buffer("testing\n"), receiver_ep);
			char buff[512];
			ip::udp::endpoint sender_ep;
			sock.receive_from(buffer(buff), sender_ep);
		}

		io_service service;
		ip::udp::socket sock(service);
		boost::asio::ip::udp::endpoint sender_ep;
		char buff[512];

		void on_read(const boost::system::error_code & err, std::size_t read_bytes) {
			std::cout << "read " << read_bytes << std::endl;
			sock.async_receive_from(buffer(buff), sender_ep, on_read);
		}

		void readAsyncUDPserver()
		{
			ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
			sock.open(ep.protocol());
			sock.set_option(boost::asio::ip::udp::socket::reuse_address(true));
			sock.bind(ep);
			sock.async_receive_from(buffer(buff, 512), sender_ep, on_read);
			service.run();
		}

		void socketOptions()
		{
			ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 80);
			ip::tcp::socket sock(service);
			sock.connect(ep);
			// TCP socket can reuse address
			ip::tcp::socket::reuse_address ra(true);
			sock.set_option(ra);
			// get sock receive buffer size
			ip::tcp::socket::receive_buffer_size rbs;
			sock.get_option(rbs);
			std::cout << rbs.value() << std::endl;
			// set sock's buffer size to 8192
			ip::tcp::socket::send_buffer_size sbs(8192);
			sock.set_option(sbs);
		}
	}

	// 3 Echo Server/Clients
	namespace chapter3
	{
		void main()
		{

		}
	}

	void your_completion_handler(const boost::system::error_code& ec)
	{

	}


	// Christopher Kohlhoff
	struct my_handler
	{
		void operator()()
		{
		}
	};

	template<class F>
	void asio_handler_invoke(F f, my_handler*)
	{
		// Do custom invocation here.
		// Default implementation calls f();
	}

	void dont_do_this()
	{
		std::string msg = "Hello, world!";
		// boost::asio::async_write(sock, boost::asio::buffer(msg), my_handler);
		// When buffer debugging is enabled, Boost.Asio stores an iterator into the string until the asynchronous operation completes, and then dereferences it to check its validity.In the above example you would observe an assertion failure just before Boost.Asio tries to call the completion handler.
	}

	typedef boost::asio::buffers_iterator< boost::asio::streambuf::const_buffers_type> iterator;
	std::pair<iterator, bool>
		match_whitespace(iterator begin, iterator end)
	{
		iterator i = begin;
		while (i != end)
		if (std::isspace(*i++))
			return std::make_pair(i, true);
		return std::make_pair(i, false);
	}

	void handler(boost::system::error_code ec)
	{ 

	}

	void main()
	{
		// Core Concepts and Functionality
		// synchronous
		{
			boost::asio::io_service io_service;
			boost::asio::ip::tcp::socket socket(io_service);
			ip::tcp::endpoint server_endpoint(ip::address::from_string("127.0.0.1"), 2001);
			socket.connect(server_endpoint);
			boost::system::error_code ec;
			// socket.connect(server_endpoint, ec);
		}
		// asynchronous
		{
			boost::asio::io_service io_service;
			boost::asio::ip::tcp::socket socket(io_service);
			ip::tcp::endpoint server_endpoint(ip::address::from_string("127.0.0.1"), 2001);
			socket.async_connect(server_endpoint, your_completion_handler);
		}
		// Strands
		// Buffers
		{
			// to read a single line from a socket and into a std::string
			boost::asio::streambuf sb;

			boost::asio::io_service io_service;
			boost::asio::ip::tcp::socket socket(io_service);
			std::size_t n = boost::asio::read_until(socket, sb, '\n');
			boost::asio::streambuf::const_buffers_type bufs = sb.data();
			std::string line(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + n);
			// buffer debugging
		}
		// non-blocking read
		{
			/*ip::tcp::socket socket(my_io_service);
			ip::tcp::socket::non_blocking nb(true);
			socket.io_control(nb);
			socket.async_read_some(null_buffers(), read_handler);
			void read_handler(boost::system::error_code ec)
			{
			if (!ec)
			{
			std::vector<char> buf(socket.available());
			socket.read_some(buffer(buf));
			}
			}*/
		}
		// Line-Based Operations
		{
			// The following example illustrates the use of async_read_until() in an HTTP server, to receive the first line of an HTTP request from a client:

			// boost::asio::streambuf b;
			// boost::asio::read_until(s, b, match_whitespace);

			/*boost::asio::streambuf b;
		boost::asio::read_until(s, b, match_char('a'));*/
		}
		// Custom Memory Allocation
		{
			// void* pointer = asio_handler_allocate(size, &h);
			// asio_handler_deallocate(pointer, size, &h);
		}
		// TCP Clients
		{
			boost::asio::io_service io_service;
			ip::tcp::resolver resolver(io_service);
			ip::tcp::resolver::query query("www.boost.org", "http");
			ip::tcp::resolver::iterator iter = resolver.resolve(query);
			ip::tcp::resolver::iterator end; // End marker.
			while (iter != end)
			{
				ip::tcp::endpoint endpoint = *iter++;
				std::cout << endpoint << std::endl;
			}
			// When an endpoint is available, a socket can be created and connected :
			ip::tcp::socket socket(io_service);
			ip::tcp::endpoint endpoint;
			socket.connect(endpoint);
		}
		// TCP Servers
		{
			boost::asio::io_service io_service;
			ip::tcp::endpoint endpoint;
			ip::tcp::acceptor acceptor(io_service, endpoint);
			ip::tcp::socket socket(io_service);
			acceptor.accept(socket);
		}
		// UDP
		{
			boost::asio::io_service io_service;
			ip::udp::resolver resolver(io_service);
			ip::udp::resolver::query query("localhost", "daytime");
			ip::udp::resolver::iterator iter = resolver.resolve(query);

			ip::udp::endpoint endpoint(ip::udp::v4(), 12345);
			ip::udp::socket socket(io_service, endpoint);
		}
		// ICMP
		{
			boost::asio::io_service io_service;
			ip::icmp::resolver resolver(io_service);
			ip::icmp::resolver::query query("localhost", "daytime");
			ip::icmp::resolver::iterator iter = resolver.resolve(query);
		}
		// Socket Iostreams
		{
			ip::tcp::iostream stream("www.boost.org", "http");
			if (!stream)
			{
				// Can't connect.
			}

			io_service ios;
			ip::tcp::endpoint endpoint(ip::tcp::v4(), 80);
			ip::tcp::acceptor acceptor(ios, endpoint);
			for (;;)
			{
				ip::tcp::iostream stream;
				acceptor.accept(*stream.rdbuf());
			}
		}
		// Timers
		{
			// a synchronous wait operation on a timer using a relative time
			{
				io_service i;
				deadline_timer t(i);
				t.expires_from_now(boost::posix_time::seconds(5));
				t.wait();
			}
			// More commonly, a program will perform an asynchronous wait operation on a timer :
			{
			io_service i;
			deadline_timer t(i);
			t.expires_from_now(boost::posix_time::milliseconds(400));
			t.async_wait(handler);
			i.run();
		}
		}
		// Serial Ports
		{
			io_service my_io_service;
			// serial_port port(my_io_service, name);
			// where name is something like "COM1" on Windows, and "/dev/ttyS0" on POSIX platforms.
		}
		// UNIX Domain Sockets
		{
			/*local::stream_protocol::socket socket1(my_io_service);
		local::stream_protocol::socket socket2(my_io_service);
		local::connect_pair(socket1, socket2);*/

			/*local::datagram_protocol::socket socket1(my_io_service);
		local::datagram_protocol::socket socket2(my_io_service);
		local::connect_pair(socket1, socket2);*/

			// A UNIX domain socket server
			//::unlink("/tmp/foobar"); // Remove previous binding.
			//local::stream_protocol::endpoint ep("/tmp/foobar");
			//local::stream_protocol::acceptor acceptor(my_io_service, ep);
			//local::stream_protocol::socket socket(my_io_service);
			//acceptor.accept(socket);

			// A client that connects to this server might look like:
			/*local::stream_protocol::endpoint ep("/tmp/foobar");
		local::stream_protocol::socket socket(my_io_service);
		socket.connect(ep);*/
		}
		// Stream-Oriented File Descriptors
		{
			/*posix::stream_descriptor in(my_io_service, ::dup(STDIN_FILENO));
		posix::stream_descriptor out(my_io_service, ::dup(STDOUT_FILENO));*/
		}
		// Stream-Oriented HANDLEs
		{
			/*HANDLE handle = ::CreateFile(...);
		windows::stream_handle pipe(my_io_service, handle);*/
		}
		// Random-Access HANDLEs
		{
			/*HANDLE handle = ::CreateFile(...);
		windows::random_access_handle file(my_io_service, handle);*/
		}
		// SSL
		{
			//ssl::context ctx(my_io_service, ssl::context::sslv23);
			//ctx.set_verify_mode(ssl::context::verify_peer);
			//ctx.load_verify_file("ca.pem");
			//// To use SSL with a TCP socket, one may write:
			//ssl::stream<ip::tcp::socket> ssl_sock(my_io_service, ctx);
			//// To perform socket-specific operations, such as establishing an outbound connection or accepting an incoming one, the underlying socket must first be obtained using the ssl::stream template's lowest_layer() member function:
			//ip::tcp::socket::lowest_layer_type& sock = ssl_sock.lowest_layer();
			//sock.connect(my_endpoint);
			//// In some use cases the underlying stream object will need to have a longer lifetime than the SSL stream, in which case the template parameter should be a reference to the stream type :
			//ip::tcp::socket sock(my_io_service);
			//ssl::stream<ip::tcp::socket&> ssl_sock(sock, ctx);
		}
	}
}

