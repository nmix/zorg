#include "ex_daemon.h"

ExDaemon::ExDaemon(uint addr) : SlaveNodeDaemon(addr)
{

}

void ExDaemon::init()
{
	SlaveNodeDaemon::init();
	loop_counter = 0;
}

void ExDaemon::loop()
{
}

void ExDaemon::process_message(std::string message)
{
	debug("income message---->  " + message);
	send("world! " + message);
}
