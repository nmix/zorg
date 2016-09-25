#include "exdaemon2.h"

#include <iostream>
#include <exception>
#include <string>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: d2 start|stop|status" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string cmd = argv[1];
	ExDaemon2 d;
	try
	{
		d.exec(cmd);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}