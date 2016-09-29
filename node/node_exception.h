#ifndef _NODE_EXCEPTION_H_
#define _NODE_EXCEPTION_H_

#include <exception>
#include <string>

#define NO_DATA_AVAILABLE		1


class NodeException : public std::exception
{
public:
	NodeException(std::string);
	NodeException(int);
	const char* what() const throw();
	int code() { return err_code; }
private:
	std::string err_str(int);
	
	std::string err_message;
	int err_code;
};

#endif // _NODE_EXCEPTION_H_