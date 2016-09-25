#ifndef _NODE_DAEMON_
#define _NODE_DAEMON_

#include "daemon.h"

class NodeDaemon : public Daemon
{
public:
	NodeDaemon(int);
protected:
	virtual void process_message(std::string) = 0;
	virtual void check_messages() = 0;
	void init();
private:
	void main_loop();
};

#endif // _NODE_DAEMON_