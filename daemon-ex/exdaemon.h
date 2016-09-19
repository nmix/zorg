#ifndef _EXDAEMON_H_
#define _EXDAEMON_H_

#include "daemon.h"

class ExDaemon : public Daemon {

public:
  ExDaemon(int addr);

protected:
  void loop();
  void init();
private:
  int log_counter;

};

#endif // _EXDAEMON_H_