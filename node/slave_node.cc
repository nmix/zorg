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

bool SlaveNode::check_messages()
{
	if (outgoing_messages_queue.size() > 0)
	{
		std::string message = outgoing_messages_queue.front();
		outgoing_messages_queue.pop();
		s_send(pusher, message);
	}
	std::string message = s_recv(sub);
	if (!message.empty())
	{
		ingoing_messages_queue.push(message);
	}
	return has_messages();
}

bool SlaveNode::send(std::string message_data)
{
	outgoing_messages_queue.push(format_message(address, message_data));
	check_messages();
	return true;
}
