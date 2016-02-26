#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <vector>
#include <thread>

using namespace boost::asio;
using namespace std;

namespace running_forever
{
	io_service service;
	ip::tcp::socket sock(service);
	char buff_read[1024], buff_write[1024] = "ok";
	void on_read(const boost::system::error_code &err, std::size_t bytes)
		;
	void on_write(const boost::system::error_code &err, std::size_t bytes)
	{
		sock.async_read_some(buffer(buff_read), on_read);
	}
	void on_read(const boost::system::error_code &err, std::size_t bytes)
	{
		// ... process the read ...
		sock.async_write_some(buffer(buff_write, 3), on_write);
	}
	void on_connect(const boost::system::error_code &err) {
		sock.async_read_some(buffer(buff_read), on_read);
	}
	int main(int argc, char* argv[]) {
		ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),
			2001);
		sock.async_connect(ep, on_connect);
		service.run();
		return 0;
	}
}