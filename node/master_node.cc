#include "master_node.h"

#include <unistd.h>

MasterNode::MasterNode() : Node(), pub(ctx, ZMQ_PUB)
{
	pub.bind(ipcfile_path);
	sleep(1); // time for init ipc file
}

MasterNode::~MasterNode()
{
}

bool MasterNode::send_to(int addr, std::string data)
{
	// --- send address
	if (!s_sendmore(pub, std::to_string(addr)))
	{
		return false;
	}
	// // --- send data
	if (!s_send(pub, data))
	{
		return false;
	}
	// ---
	return true;
}

bool MasterNode::has_message()
{
	return false;
}

std::string MasterNode::recv()
{
	return "";
}
