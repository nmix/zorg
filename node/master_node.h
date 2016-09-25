#ifndef _MASTER_NODE_H_
#define _MASTER_NODE_H_

#include "node.h"

class MasterNode : public Node
{
public:
	MasterNode();
	~MasterNode();
	bool send_to(int, std::string);
	bool has_message();
	std::string recv();
protected:

private:
	zmq::socket_t pub;

};

#endif // _MASTER_NODE_H_