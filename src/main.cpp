// C includes
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/statvfs.h>

// C++ includes
#include <iostream>
#include <thread>
#include <chrono>
#include <experimental/filesystem>

// Local includes
#include "webHook.h"

/*
 =======================================================================
 * Program:
 * 		FTC Backup Utility
 * 
 * Use:
 * 		Manage backups for FTC server data
 * 
 * Author:
 * 		Nac/Noah.
 *
 * Init Date:
 *		2019-08-17
 *
 =======================================================================
 */
 
 // Global var declarations
namespace fs = std::experimental::filesystem;
const unsigned int GB = (1024 * 1024) * 1024;
using std::string;
using std::cout;

long dirSize;

// Get date for culling existing backups
void dateCheck()
{
	int currentTime = time(NULL);
	int prunedate =  currentTime - 432000;
	cout << "Pruning files from before " << prunedate << " UTC\n";
}

// Copy files for backup
void copy(string from, string to)
{
	fs::copy(from, to, fs::copy_options::recursive);
}

// Get drive storage info
struct statvfs getDriveInfo(const char* dir)
{
	struct statvfs stat;
	statvfs(dir, &stat);
	return stat;
}

// Get total data in DIR
void getTotal(char* dir)
{
	long size = 0;
	for(auto& p: fs::recursive_directory_iterator(dir))
	{
		if(!fs::is_directory(p))
		{
			size = size + fs::file_size(p);
		}
	}
	dirSize = size;
}

// APPLICATION START
int main(uint8_t argc, char* argv[])
{
	struct statvfs frominfo;
	struct statvfs toinfo;
	
	cout << "=====FTC INTERNAL BACKUP UTILITY=====\n";
	cout << "    Written by Nac.\n";
	string from = "UNSET";
	string to = "UNSET";
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i],"-f") == 0)
		{
			if(fs::exists(argv[i+1]))
			{
				from = argv[i+1];
				frominfo = getDriveInfo(argv[i+1]);
				getTotal(argv[i+1]); 
			}
			i++;
		}
		else if(strcmp(argv[i],"-t") == 0)
		{
			if(fs::exists(argv[i+1]))
			{
				to = argv[i+1];
				toinfo = getDriveInfo(argv[i+1]);
			}
			i++;
		}
		else if(strcmp(argv[i],"-l") == 0)
		{
			setLink(argv[i+1]);
			enableLog();
			i++;
		}
		else if(strcmp(argv[i],"-v") == 0)
		{
			curlVerbose();
		}
	}
	relayMsg("Starting backups...");
	int ctime = time(NULL);
	string stime = std::to_string(ctime);
	if(from.compare("UNSET") != 0 && to.compare("UNSET") != 0)
	{
		int tototal = (toinfo.f_bsize * toinfo.f_blocks) / GB;
		int tospace = (toinfo.f_bsize * toinfo.f_bfree) / GB;
		
		int fromtotal = (frominfo.f_bsize * frominfo.f_blocks) / GB;
		int fromspace = (frominfo.f_bsize * frominfo.f_bfree) / GB;
		
		if(tospace < dirSize / GB)
		{
			cout << "Not enough space for copy.\n";
			relayMsg("Insufficient space for backups.");
			relayMsg((
				std::to_string(dirSize / GB) + 
				" GB required but only " + 
				std::to_string(tospace) + 
				" GB available."
			).c_str());
			return 0;
		}
		relayMsg("Copying files...");
		cout << "|Running backups for UTC-" << stime << "\n";
		to = to + stime;
		cout << "=Copying from '" << from << "' to '" << to << "'\n";
		copy(from, to);
		cout << "=Copy complete.\n";
		cout << dirSize << "\n";
		relayMsg("Backup's complete.");
		relayMsg((
			"Remaining space: " + 
			std::to_string(tospace) + 
			"/" + 
			std::to_string(tototal) + 
			" GB."
		).c_str());
	}
	else
	{
		cout << "|Invalid date's provided.\n";
		cout << "-TO:\n'" << to << "'\n";
		cout << "-FROM:\n'" << from << "'\n";
		relayMsg("Backup failed due to improperly set directories.");
	}
	return 0;
}
