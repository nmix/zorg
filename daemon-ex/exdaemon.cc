#include "exdaemon.h"

#include <boost/format.hpp>

ExDaemon::ExDaemon(uint addr) : Daemon(addr) 
{
  log_counter = 0;
}

void ExDaemon::init()
{
	debug("init()");
	load_config();
	std::string val = config_tree.get<std::string>("foo");
	int test = config_tree.get("test", 0);
	debug("config file: foo = " + val + " test = " + std::to_string(test));
}

void ExDaemon::loop()
{
	log_counter += 1;
	debug((boost::format("Debug message %s # %d") % progname() % log_counter).str());
	if (log_counter > 9) stop();
	sleep(1);
}

void ExDaemon::finalize()
{
	debug("finalize()");
}