#include "exdaemon1.h"

#include <boost/format.hpp>
#include <unistd.h>
#include <time.h>

#define DAEMON_ADDRESS 101

ExDaemon1::ExDaemon1() : Daemon(DAEMON_ADDRESS)
{
}

void ExDaemon1::init()
{
	debug("Daemon " + std::to_string(DAEMON_ADDRESS) + " initialization");
	loop_counter = 0;
	ctx = new zmq::context_t(1);
	pub = new zmq::socket_t(*ctx, ZMQ_PUB);
	pub->bind("ipc:///tmp/zorg/0.ipc");
}

void ExDaemon1::finalize()
{
	debug("Daemon " + std::to_string(DAEMON_ADDRESS) + " finalization");
	delete pub;
	delete ctx;
}

void ExDaemon1::loop()
{
	std::string s = "Hello subscriber!";
	// zmq::message_t message(s.size());
	// memcpy(message.data(), s.c_str(), s.size());
	// if (pub->send(message, ZMQ_NOBLOCK) != 0)
	if (!s_send(*pub, s))
	{
		error("pub send: " + std::string(strerror(errno)));
	}
	// ---
	loop_counter += 1;
	sleep(1);
	debug((boost::format("Loop # %d") % loop_counter).str());
}
