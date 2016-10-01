#ifndef _MAIN_DAEMON_H_
#define _MAIN_DAEMON_H_

#include "master_node_daemon.h"

class MainDaemon : public MasterNodeDaemon
{
public:
	MainDaemon();
private:
	void process_message_from(uint, std::string);
	// ---
	void init();
	void loop();
	// ---
	int loop_counter;
};

#endif // _MAIN_DAEMON_H_