#include "node_daemon.h"

NodeDaemon::NodeDaemon(int addr) : Daemon(addr)
{
}

void NodeDaemon::init()
{
	debug("NodeDaemon::init");
}

void NodeDaemon::main_loop()
{
	try
	{
		init();
		while (1) 
		{
			// --- check for terminate signal via shared memory
			check_stop();
			// --- messages...
			check_messages();
			// --- descendant loop
			loop();
		}
	}
	catch (const std::exception& exc)
	{
		fatal(exc.what());
		stop();
	}
}