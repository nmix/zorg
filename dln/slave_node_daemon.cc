#include "slave_node_daemon.h"

SlaveNodeDaemon::SlaveNodeDaemon(uint addr) : NodeDaemon(addr)
{
}

void SlaveNodeDaemon::init()
{
	node = new SlaveNode(get_address());
}

void SlaveNodeDaemon::finalize()
{
	delete node;
}

void SlaveNodeDaemon::check_messages()
{
	node->check_messages();
	while (node->has_messages())
	{
		std::string s = node->recv();
		info(" msg in: " + s);
		process_message(s);
	}
}


bool SlaveNodeDaemon::send(std::string message_data)
{
	info("msg out: " + message_data);
	return node->send(message_data);
}
