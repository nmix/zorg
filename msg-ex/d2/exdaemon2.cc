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
	// debug((boost::format("Debug message # %d") % i).str() );
	context = new zmq::context_t(1);
	sub = new zmq::socket_t(*context, ZMQ_SUB);
	// sub->connect("tcp://localhost:5555");
	sub->connect("ipc:///tmp/zorg/0.ipc");
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
		sub->recv(&message);
		// std::string md{message.data(), message.size()};
		debug("something receive");
	}
	// if (sub->recv(&message, ZMQ_NOBLOCK) != 0)
	// {
	// 	error("sub recv: " + std::string(strerror(errno)));
	// }
	// else
	// {
	// 	debug("something receive");
	// }
	// debug(std::to_string(sub->recv(&message, ZMQ_NOBLOCK)));
	// ---
	loop_counter += 1;
	sleep(1);
	debug((boost::format("Loop # %d") % loop_counter).str());
}
