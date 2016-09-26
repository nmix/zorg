#ifndef _MASTER_NODE_DAEMON_H_
#define _MASTER_NODE_DAEMON_H_

#include "node_daemon.h"
#include "master_node.h"

class MasterNodeDaemon : public NodeDaemon
{
public:
	MasterNodeDaemon(uint);
protected:
	void init();
	void finalize();
	bool send_to(uint, std::string);
private:
	void check_messages();
	MasterNode* node;
};

#endif // _MASTER_NODE_DAEMON_H_
