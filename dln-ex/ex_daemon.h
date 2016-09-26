#ifndef _EX_DAEMON_H_
#define _EX_DAEMON_H_

#include "slave_node_daemon.h"

class ExDaemon : public SlaveNodeDaemon
{
public:
	ExDaemon(uint);	
private:
	void process_message(std::string);
	// ---
	void init();
	void loop();
	// ---
	int loop_counter;
};

#endif // _EX_DAEMON_H_
