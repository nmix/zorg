#include "slave_node.h"

SlaveNode::SlaveNode(uint addr) : Node(), sub(ctx, ZMQ_SUB), address { addr }
{
	address_s = std::to_string(address);
	sub.connect(ipcfile_path);
	sub.setsockopt(ZMQ_SUBSCRIBE, address_s.c_str(), address_s.size());
	// ---
	poll[0] = { sub, 0, ZMQ_POLLIN, 0 };
}

void SlaveNode::check_messages()
{
	zmq::poll(&poll[0], 1, 0);
	if (poll[0].revents & ZMQ_POLLIN)
	{
		std::string message_data = s_recv(sub).substr(address_s.size());
		messages_queue.push(message_data);
	}
}
