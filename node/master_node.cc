#include "master_node.h"

MasterNode::MasterNode() : Node(), pub(ctx, ZMQ_PUB), puller(ctx, ZMQ_PULL)
{
	pub.bind(ipcfile_0_path);
	puller.bind(ipcfile_1_path);
}

bool MasterNode::send_to(uint addr, std::string data)
{
	return s_send(pub, format_message(addr, data));
}

void MasterNode::check_messages()
{
	std::string message = s_recv(puller);
	if (!message.empty())
	{
		messages_queue.push(message);
	}
}