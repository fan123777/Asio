#include <boost/asio.hpp>
#include <algorithm>

using namespace boost::asio;

namespace sync_one_thread
{
	struct client {
		ip::tcp::socket sock;
		char buff[1024]; // each msg is at maximum this size
		int already_read; // how much have we already read?
	};

	std::vector<client> clients;

	void on_read(client & c);
	void on_read_msg(client & c, const std::string & msg);

	void handle_clients() {
		while (true)
		for (int i = 0; i < clients.size(); ++i)
		if (clients[i].sock.available()) on_read(clients[i]);
	}

	void on_read(client & c)
	{
		int to_read = std::min(1024 - c.already_read, (int)c.sock.available());
		c.sock.read_some(buffer(c.buff + c.already_read, to_read));
		c.already_read += to_read;
		if (std::find(c.buff, c.buff + c.already_read, '\n') < c.buff +
			c.already_read) {
			int pos = std::find(c.buff, c.buff + c.already_read, '\n') -
				c.buff;
			std::string msg(c.buff, c.buff + pos);
			std::copy(c.buff + pos, c.buff + 1024, c.buff);
			c.already_read -= pos;
			on_read_msg(c, msg);
		}
	}

	void on_read_msg(client & c, const std::string & msg)
	{
		// analyze message, and write back
		if (msg == "request_login")
			write(c.sock, buffer("request_ok\n"));
	}
}
