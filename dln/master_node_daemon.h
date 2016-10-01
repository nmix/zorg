#ifndef _MASTER_NODE_DAEMON_H_
#define _MASTER_NODE_DAEMON_H_

#include "node_daemon.h"
#include "master_node.h"

class MasterNodeDaemon : public NodeDaemon
{
public:
	MasterNodeDaemon(uint);
protected:
	virtual void process_message_from(uint, std::string) = 0;
	void init();
	void finalize();
	bool send_to(uint, std::string);
	uint awating_messages_count() { return node->awating_messages_count(); }
private:
	void check_messages();
	MasterNode* node;
};

#endif // _MASTER_NODE_DAEMON_H_
