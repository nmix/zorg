#include "daemon.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include <csignal>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#define LOG_PREFIX          "ZORG-"
#define PIDFILE_DIRECTORY   "/tmp/zorg/"
#define PIDFILE_EXTENSION   ".pid"

Daemon::Daemon(int address)
{
	addr = address;
	logger_title = LOG_PREFIX + std::to_string(addr) + " ";
	loop_context = false;
	pidfile_path = PIDFILE_DIRECTORY + std::to_string(get_address()) + PIDFILE_EXTENSION;
}

Daemon::~Daemon()
{
}

void Daemon::init_log()
{
	logger = new Logger(logger_title);
}

void Daemon::terminate()
{
	finalize();
	info("Daemon stopped (code 1)");
}

int Daemon::get_address() 
{
	return addr;
}

std::string Daemon::get_pidfile_path()
{
	return pidfile_path;
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

void Daemon::pidfile_remove()
{
	if (!pidfile_exists()) 
	{
		throw DaemonException(PIDFILE_NOT_EXISTS);
	}
	// --- remove pidfile
	remove(pidfile_path.c_str());
	// --- check if file removed
	if (pidfile_exists())
	{
		throw DaemonException(PIDFILE_DELETING_ERROR);
	}
}


int Daemon::pidfile_pid()
{
	if (!pidfile_exists())
	{
		throw DaemonException(PIDFILE_NOT_EXISTS);
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

	// --- initialize logger
	init_log();


	/* The Big Loop */
	loop_context = true;
	info("Daemon started");
	init();
	try
	{
		while (1) 
		{
			// --- check for terminate signal via shared memory
			// ...
			// --- check for income messages
			// ...
			// --- main loop
			loop();
		}
	}
	catch (const std::exception& exc)
	{
		fatal(exc.what());
		stop();
	}
}

void Daemon::stop()
{
	int pid = pidfile_pid();
	pidfile_remove();
	// --- trying to kill the daemon
	// --- if in loop context - terminate() and exit
	if (loop_context)
	{
		terminate();
		exit(EXIT_SUCCESS);
	}
	else
	{
		// --- send terminate signal to process via shared memory
		// ...
	}
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

void Daemon::cut()
{
	int pid = pidfile_pid();
	pidfile_remove();
	// ---
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
	// ---
	Logger lg(logger_title);
	lg.log(Logger::Level::info, "Daemon killed [" + std::to_string(pid) + "]");
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
	else if (cmd == "kill")
	{
		cut();
	}
}


void Daemon::log(Logger::Level level, std::string message) 
{
	logger->log(level, message);
}

void Daemon::debug(std::string message)
{
	log(Logger::Level::debug, message);
}

void Daemon::info(std::string message)
{
	log(Logger::Level::info, message);
}

void Daemon::warning(std::string message)
{
	log(Logger::Level::warning, message);
}

void Daemon::error(std::string message)
{
	log(Logger::Level::error, message);
}

void Daemon::fatal(std::string message)
{
	log(Logger::Level::fatal, message);
}
