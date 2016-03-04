#include "Testing.h"
#include <thread>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <mutex>
#include "utils.h"

namespace testing
{
	using namespace std;
	using namespace boost::asio;
	using namespace utils;
	
	mutex mut;
	
	void log(const std::string& function, const std::string& message)
	{
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);
		lock_guard<mutex> lk(mut);
		cout << put_time(std::localtime(&now_c), "%c") << " log::" << function << " " << message << endl;
	}

	ClientConnection::ClientConnection(bool enableLogging)
		:m_Socket(m_Service),
		m_EnableLogging(enableLogging)
	{

	}

	bool ClientConnection::connect(const std::string& ip, int port)
	{
		try
		{
			log("connect", ip + ":" + to_string(port));
			ip::tcp::endpoint ep(ip::address::from_string(ip), port);
			m_Socket.connect(ep);
			return true;
		}
		catch (std::exception& e)
		{
			log("connect", e.what());
			return false;
		}
		
	}

	void ClientConnection::async_connect(const std::string& ip, int port, std::function<void(const boost::system::error_code & ec)> connectionHandler)
	{
		try
		{
			log("async_connect", ip + ":" + to_string(port));
			ip::tcp::endpoint ep(ip::address::from_string(ip), port);
			m_Socket.async_connect(ep, connectionHandler);
			m_Service.run();
//			return true;
		}
		catch (std::exception& e)
		{
			log("async_connect", e.what());
//			return false;
		}
	}

	void ClientConnection::disconnect()
	{
		log("disconnect");
		m_Socket.close();
	}

	void ClientConnection::run()
	{
		log("run");
		m_Service.run();
	}

	void ClientConnection::stop()
	{
		log("stop");
		m_Service.stop();
	}

	void ClientConnection::write(const std::string& message)
	{
		log("write", message);
		boost::asio::write(m_Socket, boost::asio::buffer(message));
	}

	bool ClientConnection::read(std::string& message)
	{
		boost::system::error_code error;
		char data[512];
		size_t len = boost::asio::read(m_Socket, buffer(data, 512), error);
		if (len > 0)
		{
			string msg(&data[0], &data[len]);
			message = msg;
		}
		if (error)
		{
			log("read", error.message());
			return false;
		}
		else
		{
			log("read", message);
			return true;
		}
	}

	/*void ClientConnection::async_write(const std::string& message, std::function<void(const boost::system::error_code&, size_t)> handle_write)
	{
		log("async_write", message);
		boost::asio::async_write(m_Socket, boost::asio::buffer(message), handle_write);
	}

	bool ClientConnection::async_read(std::string& message, std::function<void(const boost::system::error_code&, size_t)> handle_read)
	{
		log("async_read");
		boost::system::error_code error;
		char data[512];
		boost::asio::async_read(m_Socket, buffer(data, 512), boost::bind(handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
		if (error)
		{
			log("async_read", error.message());
			return false;
		}
		else
		{
			return true;
		}
	}*/

	void ClientConnection::log(const std::string& function, const std::string& message)
	{
		if (m_EnableLogging)
		{
			auto now = std::chrono::system_clock::now();
			auto now_c = std::chrono::system_clock::to_time_t(now);
			lock_guard<mutex> lk(mut);
			cout << put_time(std::localtime(&now_c), "%c") << " ClientConnection::" << function << " " << message << endl;
		}
	}

	SynchronousServer::SynchronousServer(bool enableLogging)
		:m_EnableLogging(enableLogging)
	{

	}

	void SynchronousServer::run(int port)
	{
		try
		{
			log("run", to_string(port));
			boost::asio::io_service io_service;
			ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), port));
			for (;;)
			{
				ip::tcp::socket socket(io_service);
				acceptor.accept(socket);
				log("run", "connection accepted");

				char data[512];
				size_t len = socket.read_some(buffer(data));
				if (len > 0)
				{
					string message(&data[0], &data[len]);
					log("client_session", message);
					write(socket, buffer("ok", 2));
				}

				/*std::string message = utils::make_daytime_string();
				boost::system::error_code ignored_error;
				boost::asio::write(socket, boost::asio::buffer(message),
					boost::asio::transfer_all(), ignored_error);*/
			}
			log("run", "stop running");
		}
		catch (std::exception& e)
		{
			// std::cerr << e.what() << std::endl;
			log("run", e.what());
		}

		// Torjo
		/*log("run", to_string(port));
		io_service service;
		ip::tcp::endpoint ep(ip::tcp::v4(), port);
		ip::tcp::acceptor acc(service, ep);
		while (true) {
			socket_ptr sock(new ip::tcp::socket(service));
			acc.accept(*sock);
			log("run", "connection accepted");
			thread t(boost::bind(&ServerConnection::client_session, this, sock));
			t.join();
		}*/
	}

	void SynchronousServer::client_session(socket_ptr sock)
	{
		char data[512];
		size_t len = sock->read_some(buffer(data));
		if (len > 0)
		{
			string message(&data[0], &data[len]);
			log("client_session", message);
			write(*sock, buffer("ok", 2));
		}
	}

	void SynchronousServer::log(const std::string& function, const std::string& message)
	{
		if (m_EnableLogging)
		{
			auto now = std::chrono::system_clock::now();
			auto now_c = std::chrono::system_clock::to_time_t(now);
			lock_guard<mutex> lk(mut);
			cout << put_time(std::localtime(&now_c), "%c") << " ServerConnection::" << function << " " << message << endl;
		}
	}

//	Application::Application(bool enableLogging)
//		:m_connection(enableLogging),
//		m_EnableLogging(enableLogging)
//	{
//
//	}
//
//	void Application::run()
//	{
//		m_connection.async_connect("127.0.0.1", 2001, &Application::connect_handler);
//	}
//
//	void Application::connect_handler(const boost::system::error_code & error)
//	{
//		if (error)
//		{
//			log("connect_handler", error.message());
//		}
//		else
//		{
//			log("connect_handler", "succeded");
//			log("connect_handler", "read time");
//			/*m_connection.async_read(m_message, boost::bind(&Application::write_data, this,
//				boost::asio::placeholders::error,
//				boost::asio::placeholders::bytes_transferred));*/
//		}
//	}
//
//	void Application::write_data(const boost::system::error_code& error, size_t len)
//	{
//		log("write_data", m_message);
//		m_message = "Hello";
////		m_connection.async_write(m_message, &Application::finish);
//	}
//
//	void Application::finish(const boost::system::error_code& error, size_t len)
//	{
//		log("finish", m_message);
//		m_connection.disconnect();
//	}
//
//	void Application::log(const std::string& function, const std::string& message)
//	{
//		if (m_EnableLogging)
//		{
//			auto now = std::chrono::system_clock::now();
//			auto now_c = std::chrono::system_clock::to_time_t(now);
//			lock_guard<mutex> lk(mut);
//			cout << put_time(std::localtime(&now_c), "%c") << " Application::" << function << " " << message << endl;
//		}
//	}
}