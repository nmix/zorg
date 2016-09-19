#include "logger.h"

#include <syslog.h>

Logger::Logger(std::string n)
{
  name = n;
  openlog (name.c_str(), LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0);
}

Logger::~Logger()
{
  closelog();
}

void Logger::log(Logger::Level level, std::string message)
{
  syslog(LOG_INFO, "%s", message.c_str());
}
