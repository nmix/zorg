#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>

class Logger {
public:
  Logger(std::string);
  ~Logger();
  enum class Level { debug = 0, info = 1, warning = 2, error = 3, fatal = 4 };
  void log(Logger::Level, std::string);
private:
  std::string name;
};

#endif // _LOGGER_H_