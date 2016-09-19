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
  log((boost::format("Debug message # %d") % log_counter).str());
  log_counter += 1;
  sleep(1);
}