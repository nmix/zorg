#ifndef _NODE_DAEMON_
#define _NODE_DAEMON_

#include "daemon.h"
#include "node.h"

class NodeDaemon : public Daemon
{
public:
	NodeDaemon(int);
protected:
	virtual void check_messages() = 0;
private:
};

#endif // _NODE_DAEMON_