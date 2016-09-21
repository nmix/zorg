
#include "daemon_exception.h"

DaemonException::DaemonException(std::string s)
{
	err_message = s;
	err_code = 0;
}

DaemonException::DaemonException(int code)
{
	err_code = code;
	err_message = err_str(err_code);
}

const char* DaemonException::what() const throw()
{
	return err_message.c_str();
}

std::string DaemonException::err_str(int code)
{
	std::string msg = "Unknown error";
	switch(code) {
	case PIDFILE_EXISTS:
		msg = "The pidfile already exists";
		break;
	case PIDFILE_NOT_EXISTS:
		msg = "The pidfile not exists";
		break;
	case FORK_ERROR:
		msg = "Can't create the subprocess";
		break;
	case SETSID_ERROR:
		msg = "Can't set sid for subprocess";
		break;
	case CHDIR_ERROR:
		msg = "Can't change the working directory";
		break;
	case PIDFILE_CREATION_ERROR:
		msg = "The pidfile creating error";
		break;
	case PIDFILE_DELETING_ERROR:
		msg = "The pidfile deleting error";
		break;
	case KILL_DAEMON_ERROR:
		msg = "Can't kill daemon";
		break;
	}
	return msg;
}
