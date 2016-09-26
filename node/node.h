#ifndef _NODE_H_
#define _NODE_H_

// --- zmq
#include "zmqh.hpp"
// ---
#include "node_exception.h"
// ---
#include <queue> 

class Node
{

public:
	Node();
	virtual void check_messages() = 0;
	virtual bool send_to(uint, std::string) {};
	std::string recv();
	bool has_messages();
	uint awating_messages_count() { return messages_queue.size(); }

protected:
	zmq::context_t ctx;
	std::string ipcfile_path;
	std::queue<std::string> messages_queue;

private:

};

#endif // _NODE_H_