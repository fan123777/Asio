#include <iostream>
#include "asio.h"
#include "Timer1.h"
#include "Timer2.h"
#include "Timer3.h"
#include "Timer4.h"
#include "Timer5.h"
#include "Daytime1.h"
#include "Daytime2.h"
#include "Daytime3.h"
#include "Daytime4.h"
#include "Testing.h"
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace testing;

void connect_handler(const boost::system::error_code & error)
{
	if (error)
	{
		log("connect_handler", error.message());
	}
	else
	{
		log("connect_handler", "succeded");
	}
}

void main()
{
	// boost_asio::chapter1::main();
	// boost_asio::chapter2::main();
	// timer_1::sync_timer();
	// timer_2::async_timer();
	// timer_3::async_timer();
	// timer_4::async_timer();
	// timer_5::async_timer();

	// Sync client/server
	if (false)
	{
		thread t1(daytime_2::daytime);
		this_thread::sleep_for(seconds(1));
		thread t2(daytime_1::daytime);
		t2.join();
		t1.join();
	}
	// daytime_4::daytime();
	
	if (false)
	{
		// server
		SynchronousServer server_connection(true);
		thread t(&SynchronousServer::run, server_connection, 2001);
		t.detach();
		this_thread::sleep_for(seconds(1));

		// client
		ClientConnection connection(true);
		if (connection.connect("127.0.0.1", 2001))
		{
			connection.write("hello");
			string reply;
			connection.read(reply);
			log("server replied: " + reply + "\n");
			connection.disconnect();
		}
	}

	if (false)
	{
		// server
		/*SynchronousServer server_connection(true);
		thread t(&SynchronousServer::run, server_connection, 2001);
		t.detach();
		this_thread::sleep_for(seconds(1));*/

		/*thread t1(daytime_3::daytime);
		t1.detach();
		this_thread::sleep_for(seconds(1));

		std::shared_ptr<Application> sp(new Application(true));
		thread t2(&Application::run, *sp);
		t2.join();*/
		// client
		// ClientConnection connection(true);
		// connection.async_connect("127.0.0.1", 2001, connect_handler);
	}

	// - wait some time for action to complete
	// - networking dispatcher
}