#include "main_daemon.h"
#include "ex_daemon.h"

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
	ExDaemon d1(105);
	ExDaemon d2(106);
	ExDaemon d3(111);
	try
	{
		d.exec(cmd);
		d1.exec(cmd);
		d2.exec(cmd);
		d3.exec(cmd);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
