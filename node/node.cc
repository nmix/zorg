#include "node.h"
// --- std
#include <iostream>
#include <string>
// --- boost
#include <boost/filesystem.hpp>
// ---


#define IPCFILE_DIRECTORY   "/tmp/zorg/"
#define IPCFILE_0 "0.ipc"
#define IPCFILE_1 "1.ipc"

Node::Node() : ctx(1)
{
	if (!boost::filesystem::exists(IPCFILE_DIRECTORY))
	{
		boost::filesystem::create_directory(IPCFILE_DIRECTORY);
	}
	// ---
	std::string ipcfile_dir{ IPCFILE_DIRECTORY };
	std::string ipcfile_0 { IPCFILE_0 };
	std::string ipcfile_1 { IPCFILE_1 };
	ipcfile_0_path = "ipc://" + ipcfile_dir + ipcfile_0;
	ipcfile_1_path = "ipc://" + ipcfile_dir + ipcfile_1;
	
}

std::string Node::recv()
{
	if (messages_queue.size() == 0)
	{
		return "";
	}
	std::string message = messages_queue.front();
	messages_queue.pop();
	return message;
}

bool Node::has_messages()
{
	if (messages_queue.size() > 0)
	{
		return true;
	}
	return false;
}

std::string Node::format_message(uint addr, std::string data)
{
	return std::to_string(addr) + "#" + data;
}
