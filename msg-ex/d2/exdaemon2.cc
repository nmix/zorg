#include "exdaemon2.h"

#include <boost/format.hpp>
#include <unistd.h>

#define DAEMON_ADDRESS 201

ExDaemon2::ExDaemon2() : Daemon(DAEMON_ADDRESS) 
{
	loop_counter = 0;
	// ---
}

ExDaemon2::~ExDaemon2()
{
}


void ExDaemon2::init()
{
	debug("Daemon 2 initialization");
	// debug((boost::format("Debug message # %d") % i).str() );
	context = new zmq::context_t(DAEMON_ADDRESS);
	sub = new zmq::socket_t(*context, ZMQ_SUB);
	sub->connect("ipc://zorg.ipc");
	// ---
	pollitems[0].socket = *sub;
	pollitems[0].events = ZMQ_POLLIN;

}

void ExDaemon2::loop()
{
	zmq::message_t message;
	zmq::poll(pollitems, 1, 1);
	if (pollitems[0].revents & ZMQ_POLLIN)
	{
		sub->recv(&message);
		// std::string md{message.data(), message.size()};
		debug("something receive");
	}
	// ---
	loop_counter += 1;
	sleep(2);
	debug((boost::format("Loop # %d") % loop_counter).str());
}
