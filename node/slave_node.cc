#include "slave_node.h"

SlaveNode::SlaveNode(uint addr) : Node(), sub(ctx, ZMQ_SUB), pusher(ctx, ZMQ_PUSH), address { addr }
{
	address_s = std::to_string(address);
	sub.connect(ipcfile_0_path);
	sub.setsockopt(ZMQ_SUBSCRIBE, address_s.c_str(), address_s.size());
	pusher.connect(ipcfile_1_path);
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

bool SlaveNode::send(std::string message_data)
{
	return s_send(pusher, format_message(address, message_data));
}
