#ifndef _MAIN_DAEMON_H_
#define _MAIN_DAEMON_H_

#include "master_node_daemon.h"

class MainDaemon : public MasterNodeDaemon
{
public:
	MainDaemon();	
private:
	void process_message(std::string);
	// ---
	void init();
	void loop();
	// void terminate();

};

#endif // _MAIN_DAEMON_H_