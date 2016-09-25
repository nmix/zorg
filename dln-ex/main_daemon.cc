#include "main_daemon.h"

#define ADDRESS 		101

MainDaemon::MainDaemon() : MasterNodeDaemon(ADDRESS)
{

}

void MainDaemon::init()
{
	NodeDaemon::init();
	debug("MainDaemon::init");
}

void MainDaemon::loop()
{
	debug("loop");
	// send_to(105, "hello 105");
	sleep(1);
}

void MainDaemon::process_message(std::string)
{

}
