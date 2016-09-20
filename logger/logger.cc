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
  const char* format = "%9s : %s";
  switch(level) 
  {
    case Logger::Level::debug:
      syslog(LOG_DEBUG, format, "debug", message.c_str()); 
      break;
    case Logger::Level::info:
      syslog(LOG_INFO, format, "info", message.c_str());
      break;
    case Logger::Level::warning:
      syslog(LOG_WARNING, format, "warning", message.c_str());
      break;
    case Logger::Level::error:
      syslog(LOG_ERR, format, "error", message.c_str());
      break;
    case Logger::Level::fatal:
      syslog(LOG_CRIT, format, "fatal", message.c_str());
      break;
  };
}
