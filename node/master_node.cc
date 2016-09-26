#include "master_node.h"

MasterNode::MasterNode() : Node(), pub(ctx, ZMQ_PUB)
{
	pub.bind(ipcfile_path);
}

bool MasterNode::send_to(uint addr, std::string data)
{
	// --- send message
	if (!s_send(pub, std::to_string(addr) + "#" + data))
	{
		return false;
	}
	// ---
	return true;
}
