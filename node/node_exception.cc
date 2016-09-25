#include "node_exception.h"

NodeException::NodeException(std::string s)
{
	err_message = s;
	err_code = 0;
}

NodeException::NodeException(int code)
{
	err_code = code;
	err_message = err_str(err_code);
}

const char* NodeException::what() const throw()
{
	return err_message.c_str();
}

std::string NodeException::err_str(int code)
{
	std::string msg = "Unknown error";
	switch(code) {
	case UNEXPECTED_SEND:
		msg = "Incorrect using send method for the current node type";
		break;
	}
	return msg;
}
