#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "logger.h"
#include "daemon_exception.h"

class Daemon {

public:
	Daemon(int);
	~Daemon();
	void start();
	void stop();
	void restart();
	void status();
	void exec(std::string);
	int get_address();

protected:
	virtual void loop() = 0;
	virtual void init() {};
	std::string get_pidfile_path();
	void log(Logger::Level, std::string);
	void info(std::string);
	void debug(std::string);
	void warning(std::string);
	void error(std::string);
	void fatal(std::string);

private:
	void pidfile_create(int);
	bool pidfile_exists();
	int pidfile_pid();
	bool loop_context;

	int addr;
	Logger *logger;

	std::string pidfile_path, logger_title;

};

#endif // _DAEMON_H_
