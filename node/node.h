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
	virtual bool check_messages() = 0;
	virtual bool send_to(uint, std::string) {};
	std::string recv();
	bool has_messages();
	uint awating_messages_count() { return ingoing_messages_queue.size(); }
	std::string format_message(uint, std::string);

protected:
	zmq::context_t ctx;
	std::string ipcfile_0_path;
	std::string ipcfile_1_path;
	std::queue<std::string> ingoing_messages_queue;
	std::queue<std::string> outgoing_messages_queue;

private:

};

#endif // _NODE_H_