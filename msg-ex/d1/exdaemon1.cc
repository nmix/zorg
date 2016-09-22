#include "exdaemon1.h"

#include <boost/format.hpp>
#include <unistd.h>

#define DAEMON_ADDRESS 101

ExDaemon1::ExDaemon1() : Daemon(DAEMON_ADDRESS) 
{
	loop_counter = 0;
	context = new zmq::context_t(DAEMON_ADDRESS);
	pub = new zmq::socket_t(*context, ZMQ_PUB);
	pub->bind("ipc://zorg.ipc");
}

ExDaemon1::~ExDaemon1()
{
	delete pub;
	delete context;
}

void ExDaemon1::init()
{
	debug("Daemon 1 initialization");
}

void ExDaemon1::loop()
{
	std::string s = "Hello subscriber!";
	zmq::message_t message(s.size());
	memcpy(message.data(), s.c_str(), s.size());
	// pub->send(message);
	// ---
	loop_counter += 1;
	sleep(1);
	debug((boost::format("Loop # %d") % loop_counter).str());
}
