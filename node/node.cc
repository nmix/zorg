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
	boost::filesystem::path ipc_dir(IPCFILE_DIRECTORY);
	if (!boost::filesystem::exists(ipc_dir))
	{
		boost::filesystem::create_directory(ipc_dir);
	}
	// ---
	boost::filesystem::path ipcfile_0(IPCFILE_0);
	boost::filesystem::path ipcfile_1(IPCFILE_1);
	ipcfile_0_path = "ipc://" + (ipc_dir / ipcfile_0).string();
	ipcfile_1_path = "ipc://" + (ipc_dir / ipcfile_1).string();	
}

std::string Node::recv()
{
	if (ingoing_messages_queue.size() == 0)
	{
		throw NodeException(NO_DATA_AVAILABLE);
	}
	std::string message = ingoing_messages_queue.front();
	ingoing_messages_queue.pop();
	return message;
}

bool Node::has_messages()
{
	if (ingoing_messages_queue.size() > 0)
	{
		return true;
	}
	return false;
}

std::string Node::format_message(uint addr, std::string data)
{
	return std::to_string(addr) + "#" + data;
}
