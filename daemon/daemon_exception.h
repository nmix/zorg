#ifndef _DAEMON_EXCEPTION_H_
#define _DAEMON_EXCEPTION_H_

#include <exception>
#include <string>

#define PIDFILE_EXISTS				1
#define PIDFILE_NOT_EXISTS			2
#define FORK_ERROR					3
#define SETSID_ERROR				4
#define CHDIR_ERROR					5
#define PIDFILE_CREATION_ERROR		6
#define PIDFILE_DELETING_ERROR		7
#define KILL_DAEMON_ERROR			8
#define SHARED_MEMORY_NOT_EXISTS	9
#define SHARED_MEMORY_CREATION_ERROR 10
#define CONFIG_FILE_NOT_EXISTS		11

class DaemonException : public std::exception 
{
public:
	DaemonException(std::string);
	DaemonException(int);
	const char* what() const throw();
private:
	std::string err_str(int);

	std::string err_message;
	int err_code;
};

#endif // _DAEMON_EXCEPTION_H_