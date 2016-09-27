#include "exdaemon.h"

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
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
	debug((boost::format("Debug message %s # %d") % progname() % log_counter).str());
	sleep(1);
}