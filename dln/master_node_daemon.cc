#include "master_node_daemon.h"

#define SEND_TO_DELAY	10000 // microseconds

MasterNodeDaemon::MasterNodeDaemon(uint addr) : NodeDaemon(addr)
{
}

void MasterNodeDaemon::init()
{
	node = new MasterNode();
	sleep(1);
}

void MasterNodeDaemon::finalize()
{
	delete node;
}

void MasterNodeDaemon::check_messages()
{
	node->check_messages();
	while (node->has_messages())
	{
		std::string s = node->recv();
		info(" msg in: " + s);
		Message message;
		node->format_message(message, s);
		process_message_from(message.addr, message.data);
	}
}

bool MasterNodeDaemon::send_to(uint addr, std::string data)
{
	std::string s = node->format_message(addr, data);
	info("msg out: " + s);
	bool rc = node->send_to(addr, data);
	usleep(SEND_TO_DELAY);
	return rc;
}
