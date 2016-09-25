#include "main_daemon.h"

#include <iostream>
#include <exception>
#include <string>

#include <unistd.h>


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: dln start|stop|status" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string cmd = argv[1];
	MainDaemon d;
	try
	{
		d.exec(cmd);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
