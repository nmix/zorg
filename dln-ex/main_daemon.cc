#include "main_daemon.h"
#include <boost/format.hpp>

#define ADDRESS 		101

MainDaemon::MainDaemon() : MasterNodeDaemon(ADDRESS)
{

}

void MainDaemon::init()
{
	MasterNodeDaemon::init();
	loop_counter = 0;
}

void MainDaemon::loop()
{
	sleep(1);
	std::stringstream ss;
	ss << boost::format("loop %d messages_queue [%d]") % loop_counter % awating_messages_count();
	debug(ss.str());
	if (loop_counter < 10)
	{
		send_to(105, "hello 105 " + std::to_string(loop_counter));
		send_to(106, "hello 106 " + std::to_string(loop_counter));
	}
	loop_counter += 1;

}

void MainDaemon::process_message(std::string message)
{
	debug("income message >>>>>>>>  " + message);
}
