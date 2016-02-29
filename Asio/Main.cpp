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
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

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
	/*thread t1(daytime_3::daytime);
	this_thread::sleep_for(seconds(1));
	thread t2(daytime_1::daytime);
	t2.join();
	t1.join();*/
	daytime_4::daytime();
}