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
	if (argc == 3)
	{
		address = std::stoi(argv[2]);
	}
	ExDaemon d1(address);
	d1.exec(cmd);
}