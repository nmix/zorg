#include "master_node.h"

MasterNode::MasterNode() : Node(), pub(ctx, ZMQ_PUB), puller(ctx, ZMQ_PULL)
{
	pub.bind(ipcfile_0_path);
	puller.bind(ipcfile_1_path);
}

bool MasterNode::check_messages()
{
	if (outgoing_messages_queue.size() > 0)
	{
		std::string message = outgoing_messages_queue.front();
		outgoing_messages_queue.pop();
		s_send(pub, message);
	}
	std::string message = s_recv(puller);
	if (!message.empty())
	{
		ingoing_messages_queue.push(message);
	}
	return has_messages();
}

bool MasterNode::send_to(uint addr, std::string data)
{
	outgoing_messages_queue.push(format_message(addr, data));
	check_messages();
	return true;
}
