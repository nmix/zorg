#include "master_node_daemon.h"

MasterNodeDaemon::MasterNodeDaemon(int addr) : NodeDaemon(addr)
{

}

void MasterNodeDaemon::check_messages()
{
}

bool MasterNodeDaemon::send_to(int addr, std::string data)
{
	// node.send_to(addr, data);
	return true;
}