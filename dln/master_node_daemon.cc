#include "master_node_daemon.h"

MasterNodeDaemon::MasterNodeDaemon(uint addr) : NodeDaemon(addr)
{
}

void MasterNodeDaemon::init()
{
	node = new MasterNode();
}

void MasterNodeDaemon::finalize()
{
	delete node;
}

void MasterNodeDaemon::check_messages()
{
	node->check_messages();
	// if (node->has_messages())
	// {
	// 	process_message(node->recv());
	// }
	while (node->has_messages())
	{
		process_message(node->recv());
	}
}

bool MasterNodeDaemon::send_to(uint addr, std::string data)
{
	node->send_to(addr, data);
	return true;
}
