#ifndef _SLAVE_NODE_H_
#define _SLAVE_NODE_H_

#include "node.h"

class SlaveNode : public Node
{
public:
	SlaveNode(uint);
	bool check_messages();
	bool send(std::string);

private:
	uint address;
	std::string address_s;
	zmq::pollitem_t poll[1];
	zmq::socket_t sub, pusher;
};

#endif // _SLAVE_NODE_H_