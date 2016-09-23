#ifndef _EXDAEMON1_H_
#define _EXDAEMON1_H_

#include "daemon.h"
#include "zmq.hpp"

class ExDaemon1 : public Daemon {

public:
	ExDaemon1();

protected:
	void loop();
	void init();
	void finalize();
private:
	zmq::context_t ctx;//(1);
	zmq::socket_t pub;//(ctx, ZMQ_PUB);
	int loop_counter;


};

#endif // _EXDAEMON1_H_