#include "exdaemon.h"

#include <iostream>
#include <exception>
#include <string>

int main(int argc, char* argv[])
{
	int address = 1;
	std::string cmd = "start";
	if (argc == 2)
	{
		cmd = argv[1];
	}
	else if (argc == 3)
	{
		cmd = argv[1];
		address = std::stoi(argv[2]);
	}
	ExDaemon d1(address);
	try
	{
		d1.exec(cmd);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}