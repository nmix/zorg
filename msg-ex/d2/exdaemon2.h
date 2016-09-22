#ifndef _EXDAEMON2_H_
#define _EXDAEMON2_H_

#include "daemon.h"
#include "zmq.hpp"

class ExDaemon2 : public Daemon {

public:
	ExDaemon2();
	~ExDaemon2();

protected:
	void loop();
	void init();
private:
	int loop_counter;
	zmq::context_t* context;
	zmq::socket_t* sub;
	zmq::pollitem_t pollitems[1];

};

#endif // _EXDAEMON2_H_