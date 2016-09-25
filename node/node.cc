#include "node.h"
// --- std
#include <iostream>
#include <string>
// --- boost
#include <boost/filesystem.hpp>
// ---


#define IPCFILE_DIRECTORY   "/tmp/zorg/"
#define IPCFILE "0.ipc"

Node::Node() : ctx(1)
{
	if (!boost::filesystem::exists(IPCFILE_DIRECTORY))
	{
		boost::filesystem::create_directory(IPCFILE_DIRECTORY);
	}
	// ---
	std::string ipcfile_dir{ IPCFILE_DIRECTORY };
	std::string ipcfile { IPCFILE };
	ipcfile_path = "ipc://" + ipcfile_dir + ipcfile;
	
}