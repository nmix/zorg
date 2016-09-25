#include "exdaemon2.h"

#include <boost/format.hpp>
#include <unistd.h>

#define DAEMON_ADDRESS 201

ExDaemon2::ExDaemon2() : Daemon(DAEMON_ADDRESS) 
{
}


void ExDaemon2::init()
{
	debug("Daemon 2 initialization");
	loop_counter = 0;
	context = new zmq::context_t(1);
	sub = new zmq::socket_t(*context, ZMQ_SUB);
	sub->connect("ipc:///tmp/zorg/0.ipc");
	sub->setsockopt( ZMQ_SUBSCRIBE, "", 0 );
	// ---
	pollitems[0].socket = *sub;
	pollitems[0].events = ZMQ_POLLIN;
}


void ExDaemon2::finalize()
{
	debug("Daemon " + std::to_string(DAEMON_ADDRESS) + " finalization");
	delete sub;
	delete context;
}

void ExDaemon2::loop()
{
	zmq::message_t message;
	// ---
	zmq::poll(pollitems, 1, 0);
	if (pollitems[0].revents & ZMQ_POLLIN)
	{
		if (sub->recv(&message, ZMQ_DONTWAIT))
		{
			// std::string md{message.data(), message.size()};
			// debug("something receive");
			int size = message.size();
        	std::string s(static_cast<char*>(message.data()), size);
        	info(s);
		}
	}
	// ---
	loop_counter += 1;
	sleep(1);
	debug((boost::format("Loop # %d") % loop_counter).str());
}
