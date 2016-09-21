#include "daemon.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#define LOG_PREFIX          "ZORG-"
#define PIDFILE_DIRECTORY   "/tmp/zorg/"
#define PIDFILE_EXTENSION   ".pid"

Daemon::Daemon(int address)
{
	addr = address;
	started = false;
	std::string sname = LOG_PREFIX + std::to_string(addr) + " ";
	logger = new Logger(sname);
	pidfile_path = PIDFILE_DIRECTORY + std::to_string(get_address()) + PIDFILE_EXTENSION;
}

int Daemon::get_address() 
{
	return addr;
}

void Daemon::log(Logger::Level level, std::string message) 
{
	logger->log(level, message);
}

void Daemon::debug(std::string message)
{
	logger->log(Logger::Level::debug, message);
}

bool Daemon::pidfile_exists()
{
	if (boost::filesystem::exists(pidfile_path))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Daemon::pidfile_create(int pid)
{
	if (!boost::filesystem::exists(PIDFILE_DIRECTORY))
	{
		boost::filesystem::create_directory(PIDFILE_DIRECTORY);
	}
	std::ofstream file(pidfile_path);
	file << pid;
	file.close();
	if (!pidfile_exists())
	{
		throw DaemonException(PIDFILE_CREATION_ERROR);
	}
}

int Daemon::pidfile_pid()
{
	if (!pidfile_exists())
	{
		throw DaemonException(PIDFILE_CREATION_ERROR);
	}
	std::ifstream file(pidfile_path);
	std::string pid_s( (std::istreambuf_iterator<char>(file) ),
                       (std::istreambuf_iterator<char>()) );
	return stoi(pid_s);
}

void Daemon::start() 
{
	if (pidfile_exists())
	{
		throw DaemonException(PIDFILE_EXISTS);
	}

	/* Our process ID and Session ID */
  	pid_t pid, sid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) 
	{
		throw DaemonException(FORK_ERROR);
	}

	/* If we got a good PID, then
	 we can exit the parent process. */
	if (pid > 0) 
	{
		// --- create pidfile
		pidfile_create(pid);
		return;
	}

	/* Change the file mode mask */
	umask(0);

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) 
	{
		throw DaemonException(SETSID_ERROR);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) 
	{
		throw DaemonException(CHDIR_ERROR);
	}

	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* The Big Loop */
	log(Logger::Level::info, "Daemon started");
	init();
	while (1) 
	{
		loop();
	}
}

void Daemon::stop()
{
	if (!pidfile_exists()) 
	{
		throw DaemonException(PIDFILE_NOT_EXISTS);
	}
	int pid = pidfile_pid();
	// --- trying to kill the daemon
	int kill_counter = 3;
	bool killed = false;
	while ((kill(pid, 0) == 0) && (kill_counter != 0))
	{
		kill(pid, SIGTERM);
		kill_counter -= 1;
		sleep(1);
	}
	// --- kill daemon error
	if (kill_counter == 0)
	{
		throw DaemonException(KILL_DAEMON_ERROR);
	}
	// --- remove pidfile
	remove(pidfile_path.c_str());
	if (pidfile_exists())
	{
		throw DaemonException(PIDFILE_DELETING_ERROR);
	}
	log(Logger::Level::info, "Daemon stopped");
}

void Daemon::restart()
{
	stop();
	start();
}

void Daemon::status()
{
	if (pidfile_exists())
	{
		int pid = pidfile_pid();
		std::cout << "Daemon started [" << pid << "]" << std::endl;
	}
	else
	{
		std::cout << "Daemon stopped" << std::endl;
	}
}

void Daemon::exec(std::string cmd)
{
	if (cmd == "start")
	{
		start();
	}
	else if (cmd == "stop")
	{
		stop();
	}
	else if (cmd == "status")
	{
		status();
	}
}