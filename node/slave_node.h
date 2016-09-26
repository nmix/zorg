#ifndef _SLAVE_NODE_H_
#define _SLAVE_NODE_H_

#include "node.h"

class SlaveNode : public Node
{
public:
	SlaveNode(uint);
	void check_messages();

private:
	uint address;
	std::string address_s;
	zmq::pollitem_t poll[1];
	zmq::socket_t sub;

};

#endif // _SLAVE_NODE_H_