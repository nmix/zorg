#ifndef _MASTER_NODE_H_
#define _MASTER_NODE_H_

#include "node.h"

class MasterNode : public Node
{
public:
	MasterNode();
	bool check_messages();
	bool send_to(uint, std::string);
protected:

private:
	zmq::socket_t pub, puller;

};

#endif // _MASTER_NODE_H_