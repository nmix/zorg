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
	if (loop_counter < 3)
	{
		send_to(105, "hello 105 " + std::to_string(loop_counter));
		send_to(106, "hello 106 " + std::to_string(loop_counter));
		send_to(111, "hello 111 " + std::to_string(loop_counter));
	}
	loop_counter += 1;
}

void MainDaemon::process_message(std::string message)
{
}
