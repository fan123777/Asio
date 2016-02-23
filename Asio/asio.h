#pragma once

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
		// In this chapter, we'll cover what you definitely need to know when using Boost.Asio.We'll delve deeper into asynchronous programming, which is trickier than synchronous and is much more fun.
		
		// The Network API
		
		// Boost.Asio namespaces

	}
}
