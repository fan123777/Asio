#include <boost/asio.hpp>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;
using namespace boost::asio;

namespace sync_ten_threads
{
	struct client1 {
		ip::tcp::socket sock;
		char buff[1024]; // each msg is at maximum this size
		int already_read; // how much have we already read?
		bool set_reading() {
			lock_guard<mutex> lk(cs_);
			if (is_reading_) return false; // already reading
			else { is_reading_ = true; return true; }
		}
		void unset_reading() {
			lock_guard<mutex> lk(cs_);
			is_reading_ = false;
		}
	private:
		mutex cs_;
		bool is_reading_;
	};
	std::vector<client1> clients1;

	void on_read1(client1 & c);
	void on_read_msg1(client1 & c, const std::string & msg);

	void handle_clients1() {
		for (int i = 0; i < 10; ++i)
			thread(handle_clients_thread);
	}
	void handle_clients_thread1() {
		while (true)
		for (int i = 0; i < clients1.size(); ++i)
		if (clients1[i].sock.available())
		if (clients1[i].set_reading()) {
			on_read1(clients1[i]);
			clients1[i].unset_reading();
		}
	}
	void on_read1(client1 & c) {
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
			on_read_msg1(c, msg);
		}
	}
	void on_read_msg1(client1 & c, const std::string & msg) {
		// analyze message, and write back
		if (msg == "request_login")
			write(c.sock, buffer("request_ok\n"));
	}
}
