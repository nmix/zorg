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
	// load_config();
	// std::string val = config_tree.get<std::string>("foo");
	// int test = config_tree.get("test", 0);
	// debug("condif file: foo = " + val + " test = " + std::to_string(test));
}

void ExDaemon::loop()
{
	log_counter += 1;
	debug((boost::format("Debug message %s # %d") % progname() % log_counter).str());
	sleep(1);
}