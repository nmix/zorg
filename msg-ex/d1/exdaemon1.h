#ifndef _EXDAEMON1_H_
#define _EXDAEMON1_H_

#include "daemon.h"
#include "zmq.hpp"

class ExDaemon1 : public Daemon {

public:
	ExDaemon1();
	~ExDaemon1();

protected:
	void loop();
	void init();
private:
	zmq::context_t* context;
	zmq::socket_t* pub;
	int loop_counter;


};

#endif // _EXDAEMON1_H_