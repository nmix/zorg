	#include <iostream>
#include <exception>
#include <string>

#include <stdlib.h>

int main(int argc, char* argv[])
{
	std::string cmd = "status";
	if (argc == 2)
	{
		cmd = argv[1];
	}
	if (cmd == "start")
	{
		std::cout << " 1 start" << std::endl;
		system("d1/d1 start");
		std::cout << " 2 start" << std::endl;
		system("d2/d2 start");
	}
	else if (cmd == "stop")
	{
		std::cout << " 1 stop" << std::endl;
		system("d1/d1 stop");
		std::cout << " 2 stop" << std::endl;
		system("d2/d2 stop");
	}
	else if (cmd == "status")
	{
		std::cout << " 1 status:" << std::endl;
		system("d1/d1 status");
		std::cout << " 2 status:" << std::endl;
		system("d2/d2 status");
	}
	exit(EXIT_SUCCESS);
}
