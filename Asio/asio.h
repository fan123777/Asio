#pragma once


#include <boost\bind.hpp>
#include <boost\asio.hpp>

namespace boost_asio
{
	// Getting Started with Boost.Asio
	namespace chapter1
	{
		void main();

		void synchronousClient();
		void synchronousServer();
		void asynchronousClient();
		void asynchronousServer();
		void signal();
		void serialPort();
		void deadlineOperation();
		// Single - thread with one io_service and one handler thread :
		void service1();
		// Multi-threaded with a single io_service instance and several handler threads :
		void service2();
		// Multi-threaded with several io_service instances and several threads:
		void service3();

		// What is Boost.Asio ?
		// In short, Boost.Asio is a cross - platform C++ library mainly for networking and some other low - level input / output programming.
		// Boost.Asio has successfully abstracted the concepts of input and output that work not just for networking but for COM serial ports, files, and so on.On top of these, you can do input or output programming synchronously or asynchronously :
		// read(stream, buffer [, extra options])
		// async_read(stream, buffer[, extra options], handler)
		// write(stream, buffer[, extra options])
		// async_write(stream, buffer[, extra options], handler)
		// The networking part was inspired by Berkeley Software Distribution(BSD) sockets.It provides an API that deals with Transmission Control Protocol(TCP) sockets, User Datagram Protocol(UDP) sockets, Internet Control Message Protocol(IMCP) sockets, and is extensible as you can adapt it to your own protocol if you wish.

		// History
		// The library has been tested on the following platforms and compilers:
		// - 32 - bit and 64 - bit Windows, using Visual C++ 7.1 and above
		// - Windows using MinGW
		// - Windows using Cygwin(make sure to define __USE_232_SOCKETS)
		// - Linux based on 2.4 and 2.6 kernels, using g++ 3.3 and above
		// - Solaris, using g++ 3.3 and above
		// - MAC OS X 10.4 + , using g++ 3.3 and above

		// Dependencies
		// Boost.Asio depends on the following libraries:
		// - Boost.System: This library provides operating system support for Boost libraries
		// - Boost.Regex: This library (optional) is used in case you're using the read_until() or async_read_until() overloads that take a boost::regex parameter
		// - Boost.DateTime: This library(optional) is used if you use Boost.Asio timers
		// - OpenSSL: This library (optional) is used if you decide to use the SSL support provided by Boost.Asio

		// Building Boost.Asio
		// - In only one of your files, by using #include <boost/asio/impl/src.hpp> (if you're using SSL, #include <boost/asio/ssl/impl/src.hpp> as well)
		// - By using #define BOOST_ASIO_SEPARATE_COMPILATION in all your source files

		// Important macros
		// Use BOOST_ASIO_DISABLE_THREADS if set; it disables threading support in Boost.Asio, regardless of whether Boost was compiled with threading support.

		// Synchronous versus asynchronous
		// In synchronous programming, you do the operations in sequential order, such as read(request) from socket S, then write(answer) to socket.Each operation is blocking.Since operations are blocking, in order not to interrupt the main program while you're reading from or writing to a socket, you'll usually create one or more threads that deal with socket's input/output. Thus, synchronous servers / clients are usually multi - threaded.
		// In contrast, asynchronous programming is event-driven. You start an operation, but you don't know when it will end; you supply a callback, which the API will call when the operation ends, together with the operation result. Thus, in asynchronous programming, you don't necessary need more than one thread.
		// You should decide early on in your project (preferably at the start) whether you go synchronous or asynchronous with networking, as switching midway will be very difficult and error - prone; not only will the API differ substantially, the semantic of your program will change completely(asynchronous networking is usually harder to test and debug than synchronous networking).You'll want to think of either going for blocking calls and multi - threading(synchronous, usually simpler) or less - threads and events(asynchronous, usually more complex).
		// Again, first your program needs at least one io_service instance. You then specify the port you're listening to, and create an acceptor, one object that accepts client connections.
		// In the following loop, you create a dummy socket and wait for a client to connect. Once a connection has been established, you create a thread that will deal with that connection.
		// In the client_session thread, read a client's request, interpret it, and answer back.
		// You then connect asynchronously to the address and port once the connection is complete(its completion handler), that is, connect_handler is called.
		// When connect_handler is called, check for the error code (ec), and if successful, you can write asynchronously to the server.
		// Note that the service.run() loop will run as long as there are asynchronous operations pending.In the preceding example, there's only one such operation, that is, the socket async_connect.After that, service.run() exits.

		// Exceptions versus error codes
		// All the synchronous functions have overloads that either throw in case of error or can return an error code.
		/*try {
			sock.connect(ep);
			}
			catch (boost::system::system_error e) {
			std::cout << e.code() << std::endl;
			}*/
		/*boost::system::error_code err;
		sock.connect(ep, err);
		if (err)
		std::cout << err << std::endl;*/
		// In case you're using asynchronous functions, they all return an error code, which you can examine in your callback.Asynchronous functions never throw an exception, as it would make no sense to do so.And who would catch it?
		// If your code is complex(socket read / write function calls), you should probably prefer exceptions and embody your reads / writes in the try {} catch block of a function.
		//void client_session(socket_ptr sock) {
		//	try {
		//		...
		//	}
		//	catch (boost::system::system_error e) {
		//		// handle the error
		//	}
		//}
		// If using error codes, you can very nicely see when the connection is closed, as shown in the following code snippet:
		//char data[512];
		//boost::system::error_code error;
		//size_t length = sock.read_some(buffer(data), error);
		//if (error == error::eof)
		//	return; // Connection closed
		// All Boost.Asio error codes are in namespace boost::asio::error(in case you want to create a big switch to check out the cause of the error).Just check out the boost/asio/error.hpp header for more details.

		// Threading in Boost.Asio
		// When it comes to threading in Boost.Asio, we will talk about:
		// - io_service: The io_service class is thread-safe. Several threads can call io_service::run().Most of the time you'll probably call io_service::run() from a single thread that function is blocking until all asynchronous operations complete.However, you can call io_ service::run() from several threads.This will block all threads that have called io_service::run().All callbacks will be called in the context of any of the threads that called io_service::run(); this also means that if you call io_service::run() in only one thread, all callbacks are called in the context of that thread.
		// - socket: The socket classes are not thread-safe. Thus, you should avoid doing such as reading from a socket in one thread and write to it in a different thread(this isn't recommended in general, let alone with Boost.Asio).
		// - utility: For the utility classes, it usually does not make sense to be used in several threads, nor are they thread - safe.Most of them are meant to just be used for a short time, then go out of scope.
		// The Boost.Asio library itself can use several threads besides your own, but it guarantees that from those threads, it will not call any of your code.This in turn means that callbacks are called only from the threads that have called io_service::run().

		// Not just networking
		// Boost.Asio allows waiting for signals, such as SIGTERM (software terminate), SIGINT (signal interrupt), SIGSEGV(segment violation), and so on.
		// Using Boost.Asio, you can easily connect to a serial port. The port name is COM7 on Windows, or / dev / ttyS0 on POSIX platforms :
		// Once opened, you can set some options, such as port's baud rate, parity, stop bits, as set in the following code snippet :
		// Once the port is open, you can treat the serial port as a stream, and on top of that use the free functions to read from and / or write to the serial port, such as, read async_read(), write, async_write(), as used in the following code snippet :
		// Boost.Asio also allows you to connect to Windows files, and again use the free functions, such as read(), asyn_read(), and so on, as used in the following code snippet :
		// You can do the same with POSIX file descriptors, such as pipes, standard I/O, various devices(but not with regular files), as done in the following code snippet :

		// Timers
		// Some I / O operations can have a deadline to complete.You can apply this only to asynchronous operations(synchronous means blocking, thus, no deadline).For instance, the next message from your partner needs to reach you in 100 milliseconds:
		// In the preceding code snippet, if we read our data before the deadline, read is set to true, thus our partner reached us in time.Otherwise, when deadline_handler is called, read is still set to false, which means we did not meet our deadline.
		// Boost.Asio allows for synchronous timers as well, but they are usually equivalent to a simple sleep operation.The boost::this_thread::sleep(500); code and the following snippet of code accomplish the same thing :

		// The io_service class
		// The io_service is the most important class in the library; it deals with the operating system, waiting for all asynchronous operations to end, and then calling the completion handler for each such operation.
		// If you choose to create your application synchronously, you won't need to worry about what I'm about to show you in this section.
		// You can use io_service instances in several ways. In the following examples, we have three asynchronous operations, two socket connections and a timer wait :
		// The preceding code snippet makes no sense, because service_[1].run() would need service_[0].run() to complete first.Thus, all asynchronous operations handled by service_[1] would have to wait, which is not a good idea.
		// In all the three preceding scenarios, we're waiting for three asynchronous operations to complete.To explain the differences, we'll assume that, after a while, operation 1 completes, and just after that, operation 2 completes.We'll also assume that each completion handler takes a second to complete.
		// In the first case, we're waiting for all three operations to complete in one thread. Once operation 1 completes, we call its completion handler.Even though operation 2 completes just after, the completion handler for operation 2 will be called one second after the operation 1's handler completes.
		// In the second case, we're waiting for the three operations to complete in two threads. Once operation 1 completes, we call its completion handler in the first thread.Once operation 2 completes, just after, we'll call its completion handler instantly, in the second thread(while thread 1 is busy responding to operation 1 handler's, thread 2 is free to answer any incoming new operation).
		// In the third case, in case operation 1 is connect of sock1, and operation 2 is connect of sock2, the application will behave like in the second case.Thread 1 will handle connect of sock1 completion handler, and thread 2 will handle connect of sock2 completion handler.However, if connect of sock1 is operation 1, and timeout of deadline_timer t is operation 2, thread 1 will end up handling connect of sock1 completion handler.Therefore, timeout of deadline_timer t completion handler will have to wait until connect of sock1 completion handler ends(it will wait one second), since thread 1 handles both connection handler sock1 and timeout handler of t.
		// Here's what you should have learnt from the previous examples:
		// - Situation 1 is for very basic applications. You will always run into bottleneck problem if several handlers need to be called at the same time, as they will be called in a serial manner.If one handler takes too long to complete, all subsequent handlers will have to wait.
		// - Situation 2 is for most applications. It is very robust – if several handlers are to be called at the same time(this is possible) they will each be called in their own thread.The only bottleneck you can have is if all handler threads are busy and new handlers are to be called at that time.However, as a quick solution, just increase the number of handler threads.
		// - Situation 3 is the most complex and most flexible. You should use this only when situation 2 is not enough.That will probably be when you have thousands of concurrent(socket) connections.You can consider that each handler thread(thread running io_service::run()) has its own select / epoll loop; it waits for any socket, it monitors to have a read / write operation, and then once it finds such an operation, it executes it.In most cases, you don't need to worry about this, as you'll only need to worry if the number of sockets you're monitoring grows exponentially (greater than 1, 000 sockets).In that case, having several select / epoll loops can increase response times.
		// If, in your application, you think, you'll ever need to switch to situation 3, make sure that the monitor for operations code(the code that calls io_service::run()) is insulated from the rest of the application, so you can easily change it.
		// Finally, always remember that .run() will always end if there are no more operations to monitor, as given in the following code snippet :
		//io_service service_;
		//tcp::socket sock(service_);
		//sock.async_connect(ep, connect_handler);
		//service_.run();
		//The other way is to simulate some work for it, by using the following code snippet :
		//typedef boost::shared_ptr<io_service::work> work_ptr;
		//work_ptr dummy_work(new io_service::work(service_));
		//The preceding code will make sure that service_.run()never stops unless you either useservice_.stop() or dummy_work.reset(0); // destroy dummy_work.
	}

	// Boost.Asio Fundamentals
	namespace chapter2
	{
		void main();
		void connectingRelatedFunctions();
		void readWriteFunctions();
		void writeReadSyncTCP();
		void writeReadSyncUDP();
		void readAsyncUDPserver();
		void socketOptions();
		// In this chapter, we'll cover what you definitely need to know when using Boost.Asio.We'll delve deeper into asynchronous programming, which is trickier than synchronous and is much more fun.

		// The Network API

		// Boost.Asio namespaces
		// Everything in Boost.Asio resides in the boost::asio namespace, or a sub - namespace of that :
		// - boost::asio: This is where core classes and functions reside. The important classes are io_service and streambuf. Here, we also have the free functions, such as read, read_at, read_until, their asynchronous counterparts, and their write and asynchronous write counterparts.
		// - boost::asio::ip: This is where the networking part resides. The important classes are address, endpoint, tcp, udp, icmp, and the important free functions are connect and async_connect. Note that in the boost::asio::ip::tcp::socket name, socket is just a typedef keyword inside the boost::asio::ip::tcp class.
		// - boost::asio::error: This namespace contains the error codes you can get while calling I/O routines.
		// - boost::asio::ssl: This namespace contains classes dealing with SSL.
		// - boost::asio::local: This namespace contains POSIX-specific classes.
		// - boost::asio::windows: This namespace contains Windows-specific classes.

		// IP addresses
		// To deal with IP addresses, Boost.Asio provides the ip::address , ip::address_v4 and ip::address_v6 classes.
		// They offer quite a few functions. Here are the most important ones:
		// - ip::address(v4_or_v6_address): This function converts a v4 or v6 address to ip::address
		// - ip::address:from_string(str): This function creates an address from an IPv4 address(separated by dots) or from an IPv6(hexadecimal notation)
		// - ip::address::to_string(): This function returns the friendly representation of the address
		// - ip::address_v4::broadcast([addr, mask]): This function creates a broadcast address
		// - ip::address_v4::any(): This function returns an address that represents any address
		// - ip::address_v4::loopback(), ip_address_v6::loopback(): This function returns the loopback address(for v4 / v6 protocol)
		// - ip::host_name(): This function returns the name of the current host as string datatype
		// ip::address addr = ip::address::from_string("127.0.0.1");

		// Endpoints
		// Endpoint is an address you connect to, together with a port. Each different type of socket has its own endpoint class, such as ip::tcp::endpoint, ip::udp::endpoint, and ip::icmp::endpoint.
		// If you want to connect to localhost, port 80, here you go:
		// ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 80);
		// You can construct an endpoint in three ways:
		// - endpoint(): This is the default constructor and can be used sometimes for UDP / ICMP sockets
		// - endpoint(protocol, port): This is usually used on server sockets for accepting new connections
		// - endpoint(addr, port): This creates an endpoint to an address and a port

		// If you want to connect to a hostname(not an IP address), here's what you do:
		//// outputs "87.248.122.122"
		//io_service service;
		//ip::tcp::resolver resolver(service);
		//ip::tcp::resolver::query query("www.yahoo.com", "80");
		//ip::tcp::resolver::iterator iter = resolver.resolve(query);
		//ip::tcp::endpoint ep = *iter;
		//std::cout << ep.address().to_string() << std::endl;
		// Given an endpoint, you can obtain its address, port, and IP protocol(v4 or v6) :
		// std::cout << ep.address().to_string() << ":" << ep.port() << "/" << ep.protocol() << std::endl;

		// Sockets
		// Boost.Asio comes with three types of socket classes: ip::tcp, ip::udp, and ip::icmp, and is of course extensible.You can create your own socket class, even though that is pretty complicated.In case you choose to do so, take a look at boost / asio / ip / tcp.hpp, boost / asio / ip / udp.hpp, and boost / asio / ip / icmp.hpp.They are all pretty small classes with internal typedef keywords.
		// The socket classes create a corresponding socket. You always pass the io_service instance at construction
		// io_service service;
		// ip::udp::socket sock(service)
		// sock.set_option(ip::udp::socket::reuse_address(true));

		// Synchronous error codes
		// All synchronous functions have overloads that either throw an exception or return an error code, as given in the following code snippet :
		//sync_func(arg1, arg2 ... argN); // throws
		//boost::system::error_code ec;
		//sync_func(arg1 arg2, ..., argN, ec); // returns error code

		// Socket member functions
		// Note that all asynchronous functions return immediately, while their synchronous counterparts will return only after the operation has been completed.

		// Connecting-related functions
		// - assign(protocol,socket): This function assigns a raw (native) socket to this socket instance.Use it when dealing with legacy code(that is, the native sockets are already created).
		// - open(protocol): This function opens a socket with the given IP protocol (v4 or v6).You'll use this mainly for UDP/ICMP sockets, or for server sockets.
		// - bind(endpoint): This function binds to this address.
		// - connect(endpoint): This function synchronously connects to the address.
		// - async_connect(endpoint): This function asynchronously connects to the address.
		// - is_open(): This function returns true if the socket is open.
		// - close(): This function closes the socket. Any asynchronous operations on this socket are canceled immediately and will complete with error::operation_aborted error code.
		// - shutdown(type_of_shutdown): This function disables send operations, receive operations, or both, starting now.
		// - cancel(): This function cancels all asynchronous operations on this socket. The asynchronous operations on this socket will all finish immediately with the error::operation_aborted error code.

		// Read / write functions
		// For asynchronous functions, the signature of the handler, void handler (const boost::system::error_code& e, size_t bytes), is the same :
		// - async_receive(buffer, [flags,] handler): This function starts the asynchronous receive operation of data from the socket.
		// - async_read_some(buffer,handler): This function is equivalent to async_receive(buffer, handler).
		// - async_receive_from(buffer, endpoint[, flags], handler): This function starts the asynchronous receive of data from a specific endpoint.
		// - async_send(buffer [, flags], handler): This function starts an asynchronous send function of the buffer' data.
		// - async_write_some(buffer, handler): This function is equivalent to async_send(buffer, handler).
		// - async_send_to(buffer, endpoint, handler): This function starts an asynchronous send function of the buffer' data to the specific endpoint.
		// - receive(buffer [, flags]): This function synchronously receives data in the given buffer.The function blocks until data is received, or an error occurs.
		// - read_some(buffer): This function is equivalent to receive(buffer).
		// - receive_from(buffer, endpoint [, flags]): This function synchronously receives data from a given endpoint into the given buffer.The function blocks until data is received, or an error occurs.
		// - send(buffer [, flags]): This function synchronously sends the buffer's data.The function blocks until data is successfully sent, or an error occurs.
		// - write_some(buffer): This function is equivalent to send(buffer).
		// - send_to(buffer, endpoint [, flags]): This function synchronously sends the buffer's data to a given endpoint. The function blocks until data is successfully sent or an error occurs.
		// - available(): This function returns how many bytes can be read synchronously without blocking.
		// We'll talk about buffers shortly. Let's examine the flags. The default value for flags is 0 but can be a combination of :
		// - ip::socket_type::socket::message_peek: This flag only peeks at the message.It will return the message, but the next call to read the message will re - read this message.
		// - ip::socket_type::socket::message_out_of_band: This flag processes out - of - band(OOB) data.OOB data is data that is flagged as more important than normal data.A discussion about OOB data is out of the scope of this book.
		// - ip::socket_type::socket::message_do_not_route: This flag specifies that the message should be sent without using routing tables.
		// - ip::socket_type::socket::message_end_of_record: This flag specifies that the data marks the end of a record.This is not supported on Windows.
		// Note that to read from a UDP socket using receive_from, you need a default - constructed endpoint

		// Socket control
		// These functions deal with the advanced socket options :
		// - get_io_service(): This function returns the io_service instance that was passed at construction
		// - get_option(option): This function returns a socket option
		// - set_option(option): This function sets a socket option
		// - io_control(cmd): This function executes an I/O command on the socket
		// The socket needs to be open for the previous features to work, otherwise, an exception is thrown.

		// TCP versus UDP versus ICMP
		// different functions work for different types.

		// Miscellaneous functions
		// - local_endpoint(): This function returns the address, where the socket is connected locally.
		// - remote_endpoint(): This function returns the remote address, where the socket is connected to.
		// - native_handle(): This function returns the handle of the raw socket. You only need this if you want to call a raw function not supplied by Boost.Asio.
		// - non_blocking(): This function returns true if the socket is non-blocking, false otherwise.
		// - native_non_blocking(): This function returns true if the socket is non - blocking, false otherwise.However, it will call the native API on the raw socket.Usually, you don't need this (non_blocking() already caches this result); you should only use it if you deal with the native_handle() directly yourself.
		// - at_mark(): This function returns true if the socket is about to read OOB data.You will very seldom need this.

		// Other considerations
		// As a final note, a socket instance cannot be copied, as the Copy constructor and operator = are inaccessible :
		// if you want to create copies, just use a shared pointer

		// Socket buffers
		// When reading from or writing to a socket, you'll need a buffer, one that will hold the incoming data or the outgoing data.The memory in the buffer must outlive the I / O operation; you have to make sure if it is not deallocated or goes out of scope as long as the I / O operation lasts.
		// This is extremely easy for synchronous operations
		// This is not so straightforward for asynchronous operations
		// After the call to async_receive(), buff will go out of scope, thus its memory will be deallocated.
		// - Use global buffers
		// - Create a buffer, and destroy it when the operation completes
		// - Have a connection object that maintains the socket, and additional data, such as, buffer(s)

		// The buffer function wrapper
		// char buff[512];
		// sock.async_receive(buffer(buff), on_read);
		// you can wrap any of the following into a buffer() function:
		// - A char[] const array
		// - A void* pointer and size in characters
		// - An std::string string
		// - An POD[] const array (POD stands for plain old data, meaning, constructor and destructor do nothing)
		// - An std::vector array of any POD
		// - A boost::array array of any POD
		// - An std::array array of any POD

		// The read/write/connect free functions

		// The connect functions
		// These functions connect the socket to an endpoint:
		// - connect(socket, begin [, end] [, condition]): This function synchronously connects by trying each endpoint in the sequence begin and end.The begin iterator is the result of a socket_type::resolver::query call(you might want to check out the Endpoints section again).Specifying the end iterator is optional; you can forget about it.You can supply a condition function that is called before each connection attempt.Its signature is Iterator connect_condition(const boost::system::error_code & err, Iterator next); .You can choose to return a different iterator than next, allowing you to skip over some endpoints.
		// - async_connect(socket, begin [, end] [, condition], handler): This function executes the connection asynchronously, and at the end, it calls the completion handler.The handler's signature is void handler(const boost::system::error_code & err, Iterator iterator); .The second parameter passed to the handler is the successfully connected endpoint (or the end iterator otherwise).

		// The read/write functions
		// These functions read from or write to a stream (which can be a socket, or any other class that behaves like a stream):
		// - async_read(stream, buffer [, completion] ,handler): This function asynchronously reads from a stream.On completion, the handler is called. The handler's signature is void handler(const boost::system::error_ code & err, size_t bytes); .You can optionally specify a completion function.The completion function is called after each successful read, and tells Boost.Asio if the async_read operation is complete(if not, it will continue to read).Its signature is size_t completion(const boost::system::error_code& err, size_t bytes_transfered).When this completion function returns 0, we consider the read operation complete; if it returns a non - zero value, it indicates the maximum number of bytes to be read on the next call to the stream's async_read_some operation. An example will follow to clear up this.
		// - async_write(stream, buffer [, completion], handler): This function asynchronously writes to a stream.The meaning of the arguments is similar to async_read.
		// - read(stream, buffer [, completion]): This function synchronously reads from a stream.The meaning of the arguments is similar to async_read.
		// - write(stream, buffer [, completion]): This function synchronously writes to a stream.The meaning of the arguments is similar to async_read :
		// First, note that instead of socket, the first argument is a stream. This includes sockets but is not limited.For instance, instead of a socket, you can use a Windows file handle.
		// Each read or write operation will end when one of these conditions occur:
		// - The supplied buffer is full (for read) or all the data in the buffer has been written(for write)
		// - The completion function returns 0 (if you supplied one such function)
		// - An error occurs
		// Boost.Asio comes with a few helper completion functors as well:
		// - transfer_at_least(n)
		// - transfer_exactly(n)
		// - transfer_all()
		// char buff[512];
		// void on_read(const boost::system::error_code &, size_t) {}
		// read exactly 32 bytes
		// async_read(sock, buffer(buff), transfer_exactly(32), on_read);
		// we can also read from file.

		// The read_until/async_read_until functions
		// These functions read until a condition is met:
		// - async_read_until(stream, stream_buffer, delim, handler): This function starts an asynchronous read operation.The read operation will stop when a delimeter is met.The delimeter can be any of a character, std::string or boost::regex.The handler's signature is void handler(const boost::system::error_code & err, size_t bytes);
		// - async_read_until(stream, stream_buffer, completion, handler): This function is the same as the previous one, but instead of a delimeter, we have a completion function.The completion's signature is pair<iterator, bool> completion(iterator begin, iterator end); , where iterator = is buffers_iterator<streambuf::const_buffers_type>. What you need to remember is that the iterator is of type random - accessiterator. You scan the range(begin, end), and decide if the read operation should stop or not.You will return a pair; the first member will be an iterator passed at the end of the last character consumed by the function; the second member is true if the read operation should stop, or false otherwise.
		// - read_until(stream, stream_buffer, delim): This function performs a synchronous read operation.The parameters' meaning is same as in async_read_until.
		// - read_until(stream, stream_buffer, completion): This function performs a synchronous read operation.The parameters' meaning is same as in async_read_until.

		// The *_at functions
		// These functions do random read/write operations on a stream. You specify where the read or write operation is to start from(the offset) :
		// - async_read_at(stream, offset, buffer [, completion], handler): This function starts an asynchronous read operation starting at offset, on the given stream.When the operation completes, it will call the handler.The handler's signature is void handler(const boost::system::error_code& err, size_t bytes); .The buffer can be the usual buffer() wrapper or a streambuf function.If you specify a completion function, it is called after each successful read, and tells Boost.Asio if the async_read_at operation is complete(if not, it will continue to read).Its signature is size_t completion(const boost::system::error_code& err, size_t bytes); . When this completion function returns 0, we consider the read operation complete; if it returns a non - zero value, it indicates the maximum number of bytes to be read on the next call to the stream's async_read_some_at.
		// - async_write_at(stream, offset, buffer [, completion], handler): This function starts an asynchronous write operation.The parameters' meaning is the same as async_read_at.
		// - read_at(stream, offset, buffer [, completion]): This function reads at offset, on the given stream.The parameters' meaning is the same as async_read_at.
		// - write_at(stream, offset, buffer [, completion]): This function writes at offset, on the given stream.The parameters' meaning is the same as async_read_at.
		// it is for random access streams, like files, not for sockets.

		// Asynchronous programming
		// The need for going asynchronous

		// Asynchronous run(), run_one(), poll(), poll_one()
		// To implement the listening loop, io_service class provides four functions, such as run(), run_one(), poll(), and poll_one().While most of the time you'll be happy with service.run().You'll learn here what the other functions accomplish.

		// Running forever
		// Once again, run() will run as long as there are pending operations to be executed or you manually call io_service::stop().To keep the io_service instance running, usually you add one or more asynchronous operations, and when they are executed, you keep adding asynchronous operations on and on, as in the following code :

		// The run_one(), poll(), poll_one() functions
		// The run_one() function will execute and dispatch at most one asynchronous operation:
		// - If there are no pending operations, function returns immediately, and returns 0
		// - If there are pending operations, function blocks until first operation is executed, and then returns 1
		// The poll_one function runs at most one pending operation that is ready to run, without blocking :
		// - If there is at least one operation pending, and that is ready to be run without blocking, the poll_one function runs it and returns 1
		// - Otherwise, the function returns immediately and returns 0

		//Operation pending, ready to be ran without blocking, usually means any of :
		// - A timer that has expired, and its async_wait handler needs to be called
		// - An I/O operation that has completed (such as, async_read), and its handler needs to be called
		// - A custom function handler that was previously added to io_services instance's queue (this is explained in detail in the following section)
		// You can use poll_one to make sure all handlers of completed I/O operations ran, and then do some other coding as well :
		//io_service service;
		//while (true) {
		//	// run all handlers of completed IO operations
		//	while (service.poll_one());
		//	// ... do other work here ...
		//}

		// Asynchronous work
		// Asynchronous work is not just about asynchronously accepting clients connecting to a server, asynchronous reads from or writes to sockets.It encompasses any operation that can execute asynchronously.
		// In the preceding example, service.post(some_function) adds an asynchronous function call.This function returns immediately, after requesting the io_service instance to invoke the given some_function, in one of the threads that called service.run().
		// There will be times when you want to order some of the asynchronous handler functions.Say, you have to go to restaurant(go_to_restaurant), order(order), and eat(eat).You'll want to go to the restaurant first, then order, and finally eat. For this, you'll use io_service::strand, which will order your asynchronous handler calls.

		// Asynchronous post() versus dispatch() versus wrap()
		// Boost.Asio provides three ways to add your function handler to be asynchronously called :
		// - service.post(handler): This function guarantees that it returns immediately after it has requested the io_service instance to invoke the given function handler.The handler will be called later in one of the threads that has called service.run().
		// - service.dispatch(handler): This requests the io_service instance to invoke the given function handler, but in addition, it can execute the handler inside the function if the current thread has called service.run().
		// - service.wrap(handler): This function creates a wrapper function that when called will call service.dispatch(handler).This is a bit confusing; I'll explain shortly what this means.

		// Staying alive
		// When using socket buffers, you can have a buffer instance outlive an asynchronous call(make use of boost::shared_array<>).We can use the same principle here by creating a class that internally holds the socket and its read / write buffers.Then, for all asynchronous calls, I will pass a boost::bind functor with a shared pointer :
	}

	// 3 Echo Server/Clients
	namespace chapter3
	{
		void main();

		// TCP Echo server / clients
		// You'll notice that, for reading, I've used the free function read(), because I want to read everything up to '\n'.The sock.read_some() function would not be enough, since that would only read what's available, which is not necessarily the whole message.
		// The third argument to the read() function is a completion handler.It will return 0 when it's read the full message. Otherwise, it will return the maximum buffer it can read in the next step(until read is complete).In our case, this is always 1, because we never want to mistakenly read more than we need.
		// Notice that since we're synchronous, there's no need to call service.run().

		// TCP synchronous server
	}


	// Christopher Kohlhoff

	class http_connection
	{
		void start()
		{
			boost::asio::async_read_until(socket_, data_, "\r\n",
			boost::bind(&http_connection::handle_request_line, this, _1));
		}

		void handle_request_line(boost::system::error_code ec)
		{
			if (!ec)
			{
				std::string method, uri, version;
				char sp1, sp2, cr, lf;
				std::istream is(&data_);
				is.unsetf(std::ios_base::skipws);
				is >> method >> sp1 >> uri >> sp2 >> version >> cr >> lf;
			}
		}
			boost::asio::ip::tcp::socket socket_;
			boost::asio::streambuf data_;
	};

	class match_char
	{
	public:
		explicit match_char(char c) : c_(c) {}
		template <typename Iterator>
		std::pair<Iterator, bool> operator()(
			Iterator begin, Iterator end) const
		{
			Iterator i = begin;
			while (i != end)
			if (c_ == *i++)
				return std::make_pair(i, true);
			return std::make_pair(i, false);
		}
	private:
		char c_;
	};

	// 1. Overview
	// Rationale
	// The Boost.Asio library is intended for programmers using C++ for systems programming, where access to operating system functionality such as networking is often required.In particular, Boost.Asio addresses the following goals:
	// - Portability. The library should support a range of commonly used operating systems, and provide consistent behaviour across these operating systems.
	// - Scalability. The library should facilitate the development of network applications that scale to thousands of concurrent connections. The library implementation for each operating system should use the mechanism that best enables this scalability.
	// - Efficiency. The library should support techniques such as scatter-gather I/O, and allow programs to minimise data copying.
	// - Model concepts from established APIs, such as BSD sockets. The BSD socket API is widely implemented and understood, and is covered in much literature.Other programming languages often use a similar interface for networking APIs.As far as is reasonable, Boost.Asio should leverage existing practice.
	// - Ease of use. The library should provide a lower entry barrier for new users by taking a toolkit, rather than framework, approach. That is, it should try to minimise the up - front investment in time to just learning a few basic rules and guidelines.After that, a library user should only need to understand the specific functions that are being used.
	// - Basis for further abstraction. The library should permit the development of other libraries that provide higher levels of abstraction. For example, implementations of commonly used protocols such as HTTP.
	
	// Core Concepts and Functionality
	// Basic Boost.Asio Anatomy
	// Your program will have at least one io_service object. The io_service represents your program's link to the operating system's I/O services.
	// boost::asio::io_service io_service;
	// To perform I/O operations your program will need an I/O object such as a TCP socket:
	// boost::asio::ip::tcp::socket socket(io_service);
	// When a synchronous connect operation is performed, the following sequence of events occurs:
	// 1. Your program initiates the connect operation by calling the I/O object
	// socket.connect(server_endpoint);
	// 2. The I/O object forwards the request to the io_service.
	// 3. The io_service calls on the operating system to perform the connect operation.
	// 4. The operating system returns the result of the operation to the io_service.
	// 5. The io_service translates any error resulting from the operation into a boost::system::error_code. An error_code may be compared with specific values, or tested as a boolean(where a false result means that no error occurred).The result is then forwarded back up to the I / O object.
	// 6. The I/O object throws an exception of type boost::system::system_error if the operation failed. If the code to initiate the operation had instead been written as :
	// boost::system::error_code ec;
	// socket.connect(server_endpoint, ec);
	// then the error_code variable ec would be set to the result of the operation, and no exception would be thrown.
	// When an asynchronous operation is used, a different sequence of events occurs.
	// 1. Your program initiates the connect operation by calling the I/O object.
	// socket.async_connect(server_endpoint, your_completion_handler);
	// where your_completion_handler is a function or function object with the signature:
	// void your_completion_handler(const boost::system::error_code& ec);
	// 2. The I/O object forwards the request to the io_service.
	// 3. The io_service signals to the operating system that it should start an asynchronous connect.
	// Time passes. (In the synchronous case this wait would have been contained entirely within the duration of the connect operation.)
	// 4. The operating system indicates that the connect operation has completed by placing the result on a queue, ready to be picked up by the io_service.
	// 5. Your program must make a call to io_service::run() (or to one of the similar io_service member functions) in order for the result to be retrieved.A call to io_service::run() blocks while there are unfinished asynchronous operations, so you would typically call it as soon as you have started your first asynchronous operation.
	// 6. While inside the call to io_service::run(), the io_service dequeues the result of the operation, translates it into an error_code, and then passes it to your completion handler.
	
	// The Proactor Design Pattern: Concurrency Without Threads
	// The asynchronous support is based on the Proactor design pattern.
	// Proactor and Boost.Asio
	// Asynchronous Operation - Defines an operation that is executed asynchronously, such as an asynchronous read or write on a socket.
	// Asynchronous Operation Processor - Executes asynchronous operations and queues events on a completion event queue when operations complete. From a high - level point of view, services like stream_socket_service are asynchronous operation processors.
	// Completion Event Queue - Buffers completion events until they are dequeued by an asynchronous event demultiplexer.
	// Completion Handler - Processes the result of an asynchronous operation. These are function objects, often created using boost::bind.
	// Asynchronous Event Demultiplexer - Blocks waiting for events to occur on the completion event queue, and returns a completed event to its caller.
	// Proactor - Calls the asynchronous event demultiplexer to dequeue events, and dispatches the completion handler (i.e. invokes the function object) associated with the event.This abstraction is represented by the io_service class.
	// Initiator - Application-specific code that starts asynchronous operations. The initiator interacts with an asynchronous operation processor via a high - level interface such as basic_stream_socket, which in turn delegates to a service like stream_socket_service.

	// Implementation Using Reactor
	// On many platforms, Boost.Asio implements the Proactor design pattern in terms of a Reactor, such as select, epoll or kqueue. This implementation approach corresponds to the Proactor design pattern as follows :
	// Asynchronous Operation Processor - A reactor implemented using select, epoll or kqueue. When the reactor indicates that the resource is ready to perform the operation, the processor executes the asynchronous operation and enqueues the associated completion handler on the completion event queue.
	// Completion Event Queue - A linked list of completion handlers (i.e. function objects).
	// Asynchronous Event Demultiplexer - This is implemented by waiting on an event or condition variable until a completion handler is available in the completion event queue.

	// Implementation Using Windows Overlapped I/O
	// Asynchronous Operation Processor - This is implemented by the operating system. Operations are initiated by calling an overlapped function such as AcceptEx.
	// Completion Event Queue - This is implemented by the operating system, and is associated with an I/O completion port. There is one I/O completion port for each io_service instance.
	// Asynchronous Event Demultiplexer - Called by Boost.Asio to dequeue events and their associated completion handlers.

	// Advantages
	// Portability - Many operating systems offer a native asynchronous I/O API (such as overlapped I/O on Windows) as the preferred option for developing high performance network applications.The library may be implemented in terms of native asynchronous I / O.However, if native support is not available, the library may also be implemented using synchronous event demultiplexors that typify the Reactor pattern, such as POSIX select().
	// Decoupling threading from concurrency - Long-duration operations are performed asynchronously by the implementation on behalf of the application. Consequently applications do not need to spawn many threads in order to increase concurrency.
	// Performance and scalability - Implementation strategies such as thread-per-connection (which a synchronous-only approach would require) can degrade system performance, due to increased context switching, synchronisation and data movement among CPUs.With asynchronous operations it is possible to avoid the cost of context switching by minimising the number of operating system threads — typically a limited resource — and only activating the logical threads of control that have events to process.
	// Simplified application synchronisation - Asynchronous operation completion handlers can be written as though they exist in a single-threaded environment, and so application logic can be developed with little or no concern for synchronisation issues.
	// Function composition
	// Function composition refers to the implementation of functions to provide a higher-level operation, such as sending a message in a particular format.Each function is implemented in terms of multiple calls to lower - level read or write operations.
	// For example, consider a protocol where each message consists of a fixed-length header followed by a variable length body, where the length of the body is specified in the header.A hypothetical read_message operation could be implemented using two lower - level reads, the first to receive the header and, once the length is known, the second to receive the body.
	// To compose functions in an asynchronous model, asynchronous operations can be chained together. That is, a completion handler for one operation can initiate the next.Starting the first call in the chain can be encapsulated so that the caller need not be aware that the higher - level operation is implemented as a chain of asynchronous operations.
	// The ability to compose new operations in this way simplifies the development of higher levels of abstraction above a networking library, such as functions to support a specific protocol.
	
	// Disadvantages
	// Program complexity - It is more difficult to develop applications using asynchronous mechanisms due to the separation in time and space between operation initiation and completion.Applications may also be harder to debug due to the inverted flow of control.
	// Memory usage - Buffer space must be committed for the duration of a read or write operation, which may continue indefinitely, and a separate buffer is required for each concurrent operation.The Reactor pattern, on the other hand, does not require buffer space until a socket is ready for reading or writing.

	// Threads and Boost.Asio
	// Thread Safety
	// In general, it is safe to make concurrent use of distinct objects, but unsafe to make concurrent use of a single object. However, types such as io_service provide a stronger guarantee that it is safe to use a single object concurrently.
	// Thread Pools
	// Multiple threads may call io_service::run() to set up a pool of threads from which completion handlers may be invoked. This approach may also be used with io_service::post() to use a means to perform any computational tasks across a thread pool.
	// Note that all threads that have joined an io_service's pool are considered equivalent, and the io_service may distribute work across them in an arbitrary fashion.
	// Internal Threads
	// The implementation of this library for a particular platform may make use of one or more internal threads to emulate asynchronicity. As far as possible, these threads must be invisible to the library user.In particular, the threads:
	// - must not call the user's code directly; and
	// - must block all signals.
	// NOTE
	// The implementation currently violates the first of these rules for the following functions:
	// - ip::basic_resolver::async_resolve() on all platforms.
	// - basic_socket::async_connect() on Windows.
	// - Any operation involving null_buffers() on Windows, other than an asynchronous read performed on a streamoriented socket.
	// This approach is complemented by the following guarantee:
	// - Asynchronous completion handlers will only be called from threads that are currently calling io_service::run().
	// Consequently, it is the library user's responsibility to create and manage all threads to which the notifications will be delivered.
	// The reasons for this approach include :
	// - By only calling io_service::run() from a single thread, the user's code can avoid the development complexity associated with synchronisation.For example, a library user can implement scalable servers that are single - threaded(from the user's point of view).
	// - A library user may need to perform initialisation in a thread shortly after the thread starts and before any other application code is executed.For example, users of Microsoft's COM must call CoInitializeEx before any other COM operations can be called from that thread.
	// - The library interface is decoupled from interfaces for thread creation and management, and permits implementations on platforms where threads are not available.
	
	// Strands: Use Threads Without Explicit Locking
	// A strand is defined as a strictly sequential invocation of event handlers (i.e. no concurrent invocation). Use of strands allows execution of code in a multithreaded program without the need for explicit locking
	// Strands may be either implicit or explicit, as illustrated by the following alternative approaches:
	// - Calling io_service::run() from only one thread means all event handlers execute in an implicit strand, due to the io_service's guarantee that handlers are only invoked from inside run().
	// - Where there is a single chain of asynchronous operations associated with a connection (e.g. in a half duplex protocol implementation like HTTP) there is no possibility of concurrent execution of the handlers.This is an implicit strand.
	// - An explicit strand is an instance of io_service::strand. All event handler function objects need to be wrapped using io_service::strand::wrap() or otherwise posted / dispatched through the io_service::strand object.
	// In the case of composed asynchronous operations, such as async_read() or async_read_until(), if a completion handler goes through a strand, then all intermediate handlers should also go through the same strand.This is needed to ensure thread safe access for any objects that are shared between the caller and the composed operation(in the case of async_read() it's the socket, which the caller can close() to cancel the operation).This is done by having hook functions for all intermediate handlers which forward the calls to the customisable hook associated with the final handler:
	
	// Buffers
	// Fundamentally, I/O involves the transfer of data to and from contiguous regions of memory, called buffers. These buffers can be simply expressed as a tuple consisting of a pointer and a size in bytes.However, to allow the development of efficient network applications, Boost.Asio includes support for scatter - gather operations.These operations involve one or more buffers :
	// - A scatter-read receives data into multiple buffers.
	// - A gather-write transmits multiple buffers.
	// typedef std::pair<void*, std::size_t> mutable_buffer;
	// typedef std::pair<const void*, std::size_t> const_buffer;
	// However, Boost.Asio does not use the above definitions as - is, but instead defines two classes : mutable_buffer and const_buffer. The goal of these is to provide an opaque representation of contiguous memory, where :
	// - Types behave as std::pair would in conversions. That is, a mutable_buffer is convertible to a const_buffer, but the opposite conversion is disallowed.
	// - There is protection against buffer overruns. Given a buffer instance, a user can only create another buffer representing the same range of memory or a sub - range of it.To provide further safety, the library also includes mechanisms for automatically determining the size of a buffer from an array, boost::array or std::vector of POD elements, or from a std::string.
	// - Type safety violations must be explicitly requested using the buffer_cast function. In general an application should never need to do this, but it is required by the library implementation to pass the raw memory to the underlying operating system functions.
	// Finally, multiple buffers can be passed to scatter-gather operations (such as read() or write()) by putting the buffer objects into a container.The MutableBufferSequence and ConstBufferSequence concepts have been defined so that containers such as std::vector, std::list, std::vector or boost::array can be used.

	// Streambuf for Integration with Iostreams
	// The class boost::asio::basic_streambuf is derived from std::basic_streambuf to associate the input sequence and output sequence with one or more objects of some character array type, whose elements store arbitrary values.These character array objects are internal to the streambuf object, but direct access to the array elements is provided to permit them to be used with I / O operations, such as the send or receive operations of a socket :
	// - The input sequence of the streambuf is accessible via the data() member function. The return type of this function meets the ConstBufferSequence requirements.
	// - The output sequence of the streambuf is accessible via the prepare() member function. The return type of this function meets the MutableBufferSequence requirements.
	// - Data is transferred from the front of the output sequence to the back of the input sequence by calling the commit() member function.
	// - Data is removed from the front of the input sequence by calling the consume() member function.

	// Bytewise Traversal of Buffer Sequences
	// Buffer Debugging
	// When you call an asynchronous read or write you need to ensure that the buffers for the operation are valid until the completion handler is called.In the above example, the buffer is the std::string variable msg.This variable is on the stack, and so it goes out of scope before the asynchronous operation completes. If you're lucky then the application will crash, but random failures are more likely.
	// When buffer debugging is enabled, Boost.Asio stores an iterator into the string until the asynchronous operation completes, and then dereferences it to check its validity.In the above example you would observe an assertion failure just before Boost.Asio tries to call the completion handler.
	// This feature is automatically made available for Microsoft Visual Studio 8.0 or later and for GCC when _GLIBCXX_DEBUG is defined. There is a performance cost to this checking, so buffer debugging is only enabled in debug builds.For other compilers it may be enabled by defining BOOST_ASIO_ENABLE_BUFFER_DEBUGGING.It can also be explicitly disabled by defining BOOST_ASIO_DISABLE_BUFFER_DEBUGGING.

	// Streams, Short Reads and Short Writes
	// Many I/O objects in Boost.Asio are stream-oriented. This means that:
	// - There are no message boundaries. The data being transferred is a continuous sequence of bytes.
	// - Read or write operations may transfer fewer bytes than requested. This is referred to as a short read or short write.
	// Objects that provide stream-oriented I/O model one or more of the following type requirements:
	// - SyncReadStream, where synchronous read operations are performed using a member function called read_some().
	// - AsyncReadStream, where asynchronous read operations are performed using a member function called async_read_some().
	// - SyncWriteStream, where synchronous write operations are performed using a member function called write_some().
	// - AsyncWriteStream, where synchronous write operations are performed using a member function called async_write_some().
	// Examples of stream-oriented I/O objects include ip::tcp::socket, ssl::stream<>, posix::stream_descriptor, windows::stream_handle, etc.
	// Programs typically want to transfer an exact number of bytes. When a short read or short write occurs the program must restart the operation, and continue to do so until the required number of bytes has been transferred.Boost.Asio provides generic functions that do this automatically: read(), async_read(), write() and async_write().

	// Why EOF is an Error
	// - The end of a stream can cause read, async_read, read_until or async_read_until functions to violate their contract. E.g. a read of N bytes may finish early due to EOF.
	// - An EOF error may be used to distinguish the end of a stream from a successful read of size 0.

	// Reactor-Style Operations
	// Sometimes a program must be integrated with a third-party library that wants to perform the I/O operations itself. To facilitate this, Boost.Asio includes a null_buffers type that can be used with both read and write operations.
	
	// Line-Based Operations
	// Many commonly - used internet protocols are line - based, which means that they have protocol elements that are delimited by the character sequence "\r\n".Examples include HTTP, SMTP and FTP.To more easily permit the implementation of line - based protocols, as well as other protocols that use delimiters, Boost.Asio includes the functions read_until() and async_read_until().

	// Custom Memory Allocation
	// Furthermore, programs typically contain easily identifiable chains of asynchronous operations. A half duplex protocol implementation (e.g.an HTTP server) would have a single chain of operations per client(receives followed by sends).A full duplex protocol implementation would have two chains executing in parallel.Programs should be able to leverage this knowledge to reuse memory for all asynchronous operations in a chain.
	// Custom memory allocation support is currently implemented for all asynchronous operations with the following exceptions:
	// - ip::basic_resolver::async_resolve() on all platforms.
	// - basic_socket::async_connect() on Windows.
	// - Any operation involving null_buffers() on Windows, other than an asynchronous read performed on a stream-oriented socket.

	// Networking
	// TCP, UDP and ICMP
	// TCP Clients
	// Hostname resolution is performed using a resolver, where host and service names are looked up and converted into one or more endpoints:
	// Data may be read from or written to a connected TCP socket using the receive(), async_receive(), send() or async_send() member functions.However, as these could result in short writes or reads, an application will typically use the following operations instead : read(), async_read(), write() and async_write().
	// TCP Servers
	// A program uses an acceptor to accept incoming TCP connections.
	// UDP
	// UDP hostname resolution is also performed using a resolver.
	// Data may be read from or written to an unconnected UDP socket using the receive_from(), async_receive_from(), send_to() or async_send_to() member functions.For a connected UDP socket, use the receive(), async_receive(), send() or async_send() member functions.
	// ICMP
	// Data may be read from or written to an unconnected ICMP socket using the receive_from(), async_receive_from(), send_to() or async_send_to() member functions.For a connected ICMP socket, use the receive(), async_receive(), send() or async_send() member functions.

	// Socket Iostreams

	//The BSD Socket API and Boost.Asio

	// Timers
	// Long running I/O operations will often have a deadline by which they must have completed. These deadlines may be expressed as absolute times, but are often calculated relative to the current time.
	// The deadline associated with a timer may be also be obtained as a relative time:
	// boost::posix_time::time_duration time_until_expiry = t.expires_from_now();
	// or as an absolute time to allow composition of timers:
	// deadline_timer t2(i);
	// t2.expires_at(t.expires_at() + boost::posix_time::seconds(30));

	// Serial Ports
	// Boost.Asio includes classes for creating and manipulating serial ports in a portable manner.
	// Once opened the serial port may be used as a stream. This means the objects can be used with any of the read(), async_read(), write(), async_write(), read_until() or async_read_until() free functions.

	// POSIX-Specific Functionality
	// UNIX Domain Sockets
	// NOTE
	// UNIX domain sockets are only available at compile time if supported by the target operating system. A program may test for the macro BOOST_ASIO_HAS_LOCAL_SOCKETS to determine whether they are supported.
	// Stream-Oriented File Descriptors
	// Boost.Asio includes classes added to permit synchronous and asynchronous read and write operations to be performed on POSIX file descriptors, such as pipes, standard input and output, and various devices(but not regular files).
	// These are then used as synchronous or asynchronous read and write streams. This means the objects can be used with any of the read(), async_read(), write(), async_write(), read_until() or async_read_until() free functions.
	// NOTE
	// POSIX stream descriptors are only available at compile time if supported by the target operating system. A program may test for the macro BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR to determine whether they are supported.
	// Windows - Specific Functionality
	// Stream-Oriented HANDLEs
	// NOTE
	// Windows stream HANDLEs are only available at compile time when targeting Windows and only when the I/O completion port backend is used(which is the default).A program may test for the macro BOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE to determine whether they are supported.
	// Random-Access HANDLEs
	// SSL
	// Boost.Asio contains classes and class templates for basic SSL support. These classes allow encrypted communication to be layered on top of an existing stream, such as a TCP socket.
	// Before creating an encrypted stream, an application must construct an SSL context object. This object is used to set SSL options such as verification mode, certificate files, and so on.As an illustration, client - side initialisation may look something like :
	// SSL handshaking must be performed prior to transmitting or receiving data over an encrypted connection. This is accomplished using the ssl::stream template's handshake() or async_handshake() member functions.
	// OpenSSL is required to make use of Boost.Asio's SSL support. When an application needs to use OpenSSL functionality that is not wrapped by Boost.Asio, the underlying OpenSSL types may be obtained by calling ssl::context::impl() or ssl::stream::impl().

	// Platform-Specific Implementation Notes
	
	// 2. Using Boost.Asio
	// Supported Platforms
	// Dependencies
	// - Boost.System for the boost::system::error_code and boost::system::system_error classes.
	// - Boost.Regex (optional) if you use any of the read_until() or async_read_until() overloads that take a boost::regex parameter.
	// - OpenSSL (optional) if you use Boost.Asio's SSL support.
	// Furthermore, some of the examples also require the Boost.Thread, Boost.Date_Time or Boost.Serialization libraries.
	// Building Boost Libraries
	// Macros
	
	// 3. Tutorial
	// Basic Skills - The tutorial programs in this first section introduce the fundamental concepts required to use the asio toolkit. Before plunging into the complex world of network programming, these tutorial programs illustrate the basic skills using simple asynchronous timers.
	// Introduction to Sockets - The tutorial programs in this section show how to use asio to develop simple client and server programs. These tutorial programs are based around the daytime protocol, which supports both TCP and UDP.
	
	// Timer.1 - Using a timer synchronously
	// Timer.2 - Using a timer asynchronously
	// Using asio's asynchronous functionality means having a callback function that will be called when an asynchronous operation completes. In this program we define a function called print to be called when the asynchronous wait finishes.
	// Next, instead of doing a blocking wait as in tutorial Timer.1, we call the deadline_timer::async_wait() function to perform an asynchronous wait.When calling this function we pass the print callback handler that was defined above.
	// Finally, we must call the io_service::run() member function on the io_service object.
	// The asio library provides a guarantee that callback handlers will only be called from threads that are currently calling io_service::run(). Therefore unless the io_service::run() function is called the callback for the asynchronous wait completion will never be invoked.
	// The io_service::run() function will also continue to run while there is still "work" to do. In this example, the work is the asynchronous wait on the timer, so the call will not return until the timer has expired and the callback has completed.
	// It is important to remember to give the io_service some work to do before calling io_service::run(). For example, if we had omitted the above call to deadline_timer::async_wait(), the io_service would not have had any work to do, and consequently io_service::run() would have returned immediately.
	// Timer.3 - Binding arguments to a handler
	// To implement a repeating timer using asio you need to change the timer's expiry time in your callback function, and to then start a new asynchronous wait.Obviously this means that the callback function will need to be able to access the timer object.To this end we add two new parameters to the print function :
	// - A pointer to a timer object.
	// - A counter so that we can stop the program when the timer fires for the sixth time.
	// As mentioned above, this tutorial program uses a counter to stop running when the timer fires for the sixth time. However you will observe that there is no explicit call to ask the io_service to stop.Recall that in tutorial Timer.2 we learnt that the io_service::run() function completes when there is no more "work" to do. By not starting a new asynchronous wait on the timer when count reaches 5, the io_service will run out of work and stop running.
	// Next we move the expiry time for the timer along by one second from the previous expiry time. By calculating the new expiry time relative to the old, we can ensure that the timer does not drift away from the whole - second mark due to any delays in processing the handler.
	// Then we start a new asynchronous wait on the timer. As you can see, the boost::bind() function is used to associate the extra parameters with your callback handler.The deadline_timer::async_wait() function expects a handler function(or function object) with the signature void(const boost::system::error_code&).Binding the additional parameters converts your print function into a function object that matches the signature correctly.
	// A new count variable is added so that we can stop the program when the timer fires for the sixth time.
	// Timer.4 - Using a member function as a handler
	// The boost::bind() function works just as well with class member functions as with free functions. Since all non-static class member functions have an implicit this parameter, we need to bind this to the function.
	// Timer.5 - Synchronising handlers in multithreaded programs
	// The previous four tutorials avoided the issue of handler synchronisation by calling the io_service::run() function from one thread only.As you already know, the asio library provides a guarantee that callback handlers will only be called from threads that are currently calling io_service::run().Consequently, calling io_service::run() from only one thread ensures that callback handlers cannot run concurrently.
	// The single threaded approach is usually the best place to start when developing applications using asio. The downside is the limitations it places on programs, particularly servers, including:
	// - Poor responsiveness when handlers can take a long time to complete.
	// - An inability to scale on multiprocessor systems.
	// If you find yourself running into these limitations, an alternative approach is to have a pool of threads calling io_service::run(). However, as this allows handlers to execute concurrently, we need a method of synchronisation when handlers might be accessing a shared, thread - unsafe resource.
	// In addition to initialising a pair of boost::asio::deadline_timer members, the constructor initialises the strand_ member, an object of type boost::asio::strand.
	// An boost::asio::strand guarantees that, for those handlers that are dispatched through it, an executing handler will be allowed to complete before the next one is started.This is guaranteed irrespective of the number of threads that are calling io_service::run(). Of course, the handlers may still execute concurrently with other handlers that were not dispatched through an boost::asio::strand, or were dispatched through a different boost::asio::strand object.
	// When initiating the asynchronous operations, each callback handler is "wrapped" using the boost::asio::strand object. The strand::wrap() function returns a new handler that automatically dispatches its contained handler through the boost::asio::strand object.By wrapping the handlers using the same boost::asio::strand, we are ensuring that they cannot execute concurrently.
	// In a multithreaded program, the handlers for asynchronous operations should be synchronised if they access shared resources.In this tutorial, the shared resources used by the handlers(print1 and print2) are std::cout and the count_ data member.
	// The main function now causes io_service::run() to be called from two threads: the main thread and one additional thread. This is accomplished using an boost::thread object.
	// Just as it would with a call from a single thread, concurrent calls to io_service::run() will continue to execute while there is "work" left to do.The background thread will not exit until all asynchronous operations have completed.

}