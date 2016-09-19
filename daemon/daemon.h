#ifndef _DAEMON_H_
#define _DAEMON_H_

#include <string>

#include "logger.h"

class Daemon {

public:
  Daemon(int);
  void start();
  void stop();
  void restart();
  void status();
  int get_address();

protected:
  virtual void loop() = 0;
  virtual void init() {};
  void log(std::string);

private:
  int addr;
  bool started;
  Logger *logger;

};

#endif // _DAEMON_H_
