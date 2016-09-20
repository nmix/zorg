#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "logger.h"
#include "daemon_exception.h"

class Daemon {

public:
  Daemon(int);
  void start();
  void stop();
  void restart();
  void status();
  void exec(std::string);
  int get_address();

protected:
  virtual void loop() = 0;
  virtual void init() {};
  void log(Logger::Level, std::string);
  void debug(std::string);

private:

  void pidfile_create(int);
  bool pidfile_exists();
  int pidfile_pid();

  int addr;
  bool started;
  Logger *logger;

  std::string pidfile_path;

};

#endif // _DAEMON_H_
