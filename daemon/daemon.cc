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
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#define LOG_PREFIX          "ZORG-"
#define PIDFILE_DIRECTORY   "/tmp/zorg/"
#define PIDFILE_EXTENSION   ".pid"
#define SHARED_MEMORY_PREFIX "SHM-ZORG-"
#define SHARED_MEMORY_SIZE	2

#define TERM_FLAG		0x0001

Daemon::Daemon(uint address)
{
	addr = address;
	logger_title = LOG_PREFIX + std::to_string(addr) + " ";
	loop_context = false;
	pidfile_path = PIDFILE_DIRECTORY + std::to_string(get_address()) + PIDFILE_EXTENSION;
	shm_name = SHARED_MEMORY_PREFIX + std::to_string(get_address());
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
	// ---
	using namespace boost::interprocess;
	shared_memory_object::remove(shm_name.c_str());
	// ---
	info("Daemon stopped");
	delete logger;
}

void Daemon::start()
{
	if (pidfile_exists())
	{
		throw DaemonException(PIDFILE_EXISTS);
	}

	// --- create shared memory (if it not exists)
	if (!shared_memory_exists())
	{
		shared_memory_create();
	}
	shared_memory_reset();

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

	// --- set context flag
	loop_context = true;

	// --- logging
	info("Daemon " + std::to_string(get_address()) + " started");

	// --- main loop
	try
	{
		init();
		while (1) 
		{
			// --- check for terminate signal via shared memory
			if (shared_memory_get_flag(TERM_FLAG))
			{
				terminate();
			}
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
	// int pid = pidfile_pid();
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
		shared_memory_set_flag(TERM_FLAG);
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

void Daemon::kill_process()
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
		kill_process();
	}
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



bool Daemon::shared_memory_exists()
{
	using namespace boost::interprocess;
	try
	{
		shared_memory_object shm_obj(open_only, shm_name.c_str(), read_only);
	}
	catch (const std::exception& ex)
	{
		return false;
	}
	return true;
}

void Daemon::shared_memory_create()
{
	using namespace boost::interprocess;
	try
	{
		shared_memory_object shm_obj(create_only, shm_name.c_str(), read_write);
		shm_obj.truncate(SHARED_MEMORY_SIZE);
		mapped_region region(shm_obj, read_write);
		std::memset(region.get_address(), 0, region.get_size());
	}
	catch (const std::exception& ex)
	{
		throw DaemonException(SHARED_MEMORY_CREATION_ERROR);
	}
}

void Daemon::shared_memory_set_flag(ushort flag)
{
	using namespace boost::interprocess;
	if (!shared_memory_exists())
	{
		throw DaemonException(SHARED_MEMORY_NOT_EXISTS); 
	}
	shared_memory_object shm_obj(open_only, shm_name.c_str(), read_write);
	mapped_region region(shm_obj, read_write);
	ushort* flags_ptr = static_cast<ushort*>(region.get_address());
	*flags_ptr = *flags_ptr | flag;
}

void Daemon::shared_memory_drop_flag(ushort flag)
{
	using namespace boost::interprocess;
	if (!shared_memory_exists())
	{
		throw DaemonException(SHARED_MEMORY_NOT_EXISTS); 
	}
	shared_memory_object shm_obj(open_only, shm_name.c_str(), read_write);
	mapped_region region(shm_obj, read_write);
	ushort* flags_ptr = static_cast<ushort*>(region.get_address());
	*flags_ptr = *flags_ptr & ~flag;
}

bool Daemon::shared_memory_get_flag(ushort flag)
{
	using namespace boost::interprocess;
	if (!shared_memory_exists())
	{
		throw DaemonException(SHARED_MEMORY_NOT_EXISTS); 
	}
	shared_memory_object shm_obj(open_only, shm_name.c_str(), read_only);
	mapped_region region(shm_obj, read_only);
	ushort* flags_ptr = static_cast<ushort*>(region.get_address());
	if (*flags_ptr & flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Daemon::shared_memory_reset()
{
	using namespace boost::interprocess;
	if (!shared_memory_exists())
	{
		throw DaemonException(SHARED_MEMORY_NOT_EXISTS); 
	}
	shared_memory_object shm_obj(open_only, shm_name.c_str(), read_write);
	mapped_region region(shm_obj, read_write);
	std::memset(region.get_address(), 0, region.get_size());
}