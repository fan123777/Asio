#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <string>

namespace testing
{
	void log(const std::string& function, const std::string& message = "");

	class ClientConnection
	{
	public:
		ClientConnection(bool enableLogging = false);
		bool connect(const std::string& ip, int port);
		void async_connect(const std::string& ip, int port, std::function<void(const boost::system::error_code & ec)> connectionHandler);
		void disconnect();
		void run();
		void stop();
		void write(const std::string& message);
		// void async_write(const std::string& message, std::function<void(const boost::system::error_code&, size_t)> handle_write);
		bool read(std::string& message);
		// bool async_read(std::string& message, std::function<void(const boost::system::error_code&, size_t)> handle_read);

	private:
		void log(const std::string& function, const std::string& message = "");
		boost::asio::io_service m_Service;
		boost::asio::ip::tcp::socket m_Socket;
		bool m_EnableLogging;
	};

	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

	class SynchronousServer
	{
	public:
		SynchronousServer(bool enableLogging);
		void run(int port);
		void client_session(socket_ptr sock);
	private:
		void log(const std::string& function, const std::string& message = "");
		bool m_EnableLogging;
	};

	//class Application
	//{
	//public:
	//	Application(bool enableLogging);
	//	void run();

	//private:
	//	// Networking part
	//	void connect_handler(const boost::system::error_code& error);
	//	void write_data(const boost::system::error_code& error, size_t len);
	//	void finish(const boost::system::error_code& error, size_t len);

	//	// Logging
	//	void log(const std::string& function, const std::string& message = "");

	//	ClientConnection m_connection;
	//	bool m_EnableLogging;
	//	std::string m_message;
	//};

}

