#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "logger.h"
#include "daemon_exception.h"

class Daemon {

public:
	Daemon(uint);
	~Daemon();
	void start();
	void stop();
	void restart();
	void status();
	void kill_process();
	void exec(std::string);
	uint get_address() { return addr; };
	// ---
	void set_config(std::string);
	bool config_exists();
	void load_config();

protected:
	void check_stop();
	virtual void check_messages() {};
	// ---
	virtual void loop() = 0;
	virtual void init() {};
	virtual void finalize() {};
	// ---
	std::string progname();
	std::string get_config_path() { return config_path; }


	std::string get_pidfile_path();
	void log(Logger::Level, std::string);
	void info(std::string);
	void debug(std::string);
	void warning(std::string);
	void error(std::string);
	void fatal(std::string);

private:
	void init_log();
	void terminate();
	// --- pidfile
	void pidfile_create(int);
	void pidfile_remove();
	bool pidfile_exists();
	int pidfile_pid();
	std::string pidfile_path;
	// ---
	bool shared_memory_exists();
	void shared_memory_create();
	void shared_memory_set_flag(ushort);	
	void shared_memory_drop_flag(ushort);	
	bool shared_memory_get_flag(ushort);
	void shared_memory_reset();
	std::string shm_name;
	// ---
	Logger *logger;
	std::string logger_title;
	// ---
	bool loop_context;
	uint addr;
	std::string exe_dir, config_path;

};

#endif // _DAEMON_H_
