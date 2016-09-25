#ifndef _NODE_H_
#define _NODE_H_

// --- zmq
#include "zhelpers.hpp"
// ---
#include "node_exception.h"

class Node
{

public:
	Node();
	virtual bool has_message() = 0;
	virtual std::string recv() = 0;

protected:

	zmq::context_t ctx;
	std::string ipcfile_path;

private:



};

#endif // _NODE_H_