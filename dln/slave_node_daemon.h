#ifndef _SLAVE_NODE_DAEMON_H_
#define _SLAVE_NODE_DAEMON_H_

#include "node_daemon.h"
#include "slave_node.h"

class SlaveNodeDaemon : public NodeDaemon
{
public:
	SlaveNodeDaemon(uint);
protected:
	void init();
	void finalize();
	std::string recv();
	uint awating_messages_count() { return node->awating_messages_count(); }
	bool send(std::string);
private:
	void check_messages();
	SlaveNode* node;
};

#endif // _SLAVE_NODE_DAEMON_H_
