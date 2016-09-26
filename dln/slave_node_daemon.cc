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
	if (node->has_messages())
	{
		process_message(recv());
	}
}

std::string SlaveNodeDaemon::recv()
{
	return node->recv();
}

bool SlaveNodeDaemon::send(std::string message_data)
{
	return node->send(message_data);
}
