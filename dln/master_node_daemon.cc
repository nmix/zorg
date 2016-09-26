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
}

bool MasterNodeDaemon::send_to(uint addr, std::string data)
{
	node->send_to(addr, data);
	return true;
}
