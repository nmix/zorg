#include "main_daemon.h"

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
	debug("loop " + std::to_string(loop_counter));
	send_to(105, "hello 105");
	send_to(106, "hello 106");
	loop_counter += 1;
	sleep(1);
}

void MainDaemon::process_message(std::string)
{

}
