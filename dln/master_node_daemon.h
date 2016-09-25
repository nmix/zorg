#ifndef _MASTER_NODE_DAEMON_H_
#define _MASTER_NODE_DAEMON_H_

#include "node_daemon.h"
#include "master_node.h"

class MasterNodeDaemon : public NodeDaemon
{
public:
	MasterNodeDaemon(int);
protected:
	bool send_to(int, std::string);
private:
	void check_messages();

};

#endif // _MASTER_NODE_DAEMON_H_
