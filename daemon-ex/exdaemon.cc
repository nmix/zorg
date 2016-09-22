#include "exdaemon.h"

#include <boost/format.hpp>
#include <unistd.h>

ExDaemon::ExDaemon(int addr) : Daemon(addr) 
{
  log_counter = 0;
}

void ExDaemon::init()
{
}

void ExDaemon::loop()
{
	log_counter += 1;
	log(Logger::Level::debug, (boost::format("Debug message # %d") % log_counter).str());
	sleep(1);
	// if (log_counter == 5) throw DaemonException("some error");
}