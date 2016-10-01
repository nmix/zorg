#ifndef _SLAVE_NODE_DAEMON_H_
#define _SLAVE_NODE_DAEMON_H_

#include "node_daemon.h"
#include "slave_node.h"

class SlaveNodeDaemon : public NodeDaemon
{
public:
	SlaveNodeDaemon(uint);
protected:
	virtual void process_message(std::string) = 0;
	void init();
	void finalize();
	uint awating_messages_count() { return node->awating_messages_count(); }
	bool send(std::string);
private:
	void check_messages();
	SlaveNode* node;
};

#endif // _SLAVE_NODE_DAEMON_H_
