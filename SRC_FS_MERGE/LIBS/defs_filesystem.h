/*
 *(c) Zachary Job
 *2/3/17
 *
 *defs_filesystem.h
 *
 *For use in reading from and writing to the filesystem
 */

#ifndef DEFS_FILESYSTEM
#define DEFS_FILESYSTEM

#include <iostream>
#include <string>
#include <memory>
#if defined _WIN32 || defined _WIN64 //Assume fileops is fairly uniform in nix for now
	#include <windows.h>
#else
	#include <sys/stat.h>
#endif

#include "defs_file.h"
#include "defs_debug.h"

#define FILESYSTEM_MANAGER_LOCAL_ERROR_COUNTp 1

using namespace std;

/*
 *This class is to provide a concise abstraction
 *of file management. It shall be able to traverse and
 *modify for tansparency.
 */
class filesystem_head : public debug_status
{
	private:
		
		string filesystem_current_directory;	

		
		enum class filesystem_local_errors_lookup
		{
			some_generic_placeholder = 0
		};

		const string
			filesystem_local_errors[CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp] =
			{
				"Some generic placeholder"
			}
		;
	public:
		
		filesystem_head(string initial_directory);
		~filesystem_head();

		bool changeDirectory(string directory);
		bool stepBackDirectory(string target_node);
		bool stepForwardDirectory(string target_node);

		shared_ptr<vector<pair<string, bool>>> listCurrentDirectory(string directory = ".");

		bool makeDirectory(string directory);
		bool removeDirectory(string directory);

		bool touchFile(string file);
		bool createFile(string file_name, unsigned char* content, bool is_binary_file);
		bool deleteFile(string file_name);
};

//FILESYSTEM HEAD////////////////////////////
/////////////////////////////////////////////

/*
 *A simple initializer for setting the targeted directory.
 *
 *@PARAM: The initial directory to start the head at
 */
filesystem_head::filesystem_head(string initial_directory):
	debug_status(filesystem_local_errors, debug_status::getMinClassError() + FILESYSTEM_MANAGER_LOCAL_ERROR_COUNTp - 1)
{

}

/*
 *A placeholder in the event something requires cleaning
 */
filesystem_head::~filesystem_head()
{

}

/*
 *Seek the new directory specified
 *
 *@PARAM: The directory specified for changing to
 *@RETURN: If the directory has changed or a change was not required
 */
bool filesystem_head::changeDirectory(string directory)
{

	return 0;
}

/*
 *Return upwards towards the parent directory from the current
 *directory
 *
 *@PARAM: Step backwards to the specified directory else only
				to the parent starting from the current directory
 *@RETURN: If the function was able to return to a parent directory
 */
bool filesystem_head::stepBackDirectory(string target_node = "..")
{

	return 0;
}

/*
 *Move forward into a child directory starting from the current
 *directory
 *
 *@PARAM: The desired child directory
 *@RETURN: If the function was able to step forward into a specified directory
 */
bool filesystem_head::stepForwardDirectory(string target_node)
{

	return 0;
}

/*
 *List the contents of the head's location in the file structure
 *if not otherwise specified starting from the current directory
 *
 *@PARAM: The specified directory else it will default to the current directory
 *@RETURN: The list of contents and whether or not they are
 *				a directory or file
 */
shared_ptr<vector<pair<string, bool>>> listCurrentDirectory(string directory = ".")
{

	return nullptr;
}

/*
 *Create a directory as specified by input starting from
 *the current directory
 *
 *@PARAM: The directory to create
 *@RETURN: If the directory was wholly created
 */
bool filesystem_head::makeDirectory(string directory)
{

	return 0;
}

/*
 *Remove the specified directory else default to the current directory
 *directory starting from the current directory
 *
 *@PARAM: The directory to remove if specified else default to the current
 *@RETURN: If the desired directory was removed
 */
bool filesystem_head::removeDirectory(string directory = ".")
{

	return 0;
}

/*
 *Create a null file starting at the current directory
 *
 *@PARAM: The file to be created
 *@RETURN: If the file was successfully created
 */
bool filesystem_head::touchFile(string file)
{

	return 0;
}

/*
 *Create a file with specified contents in both plaintext and
 *binary formats starting at the current directory
 *
 *@PARAM: The file to be created 
 *@PARAM: The content of the file
 *@PARAM: If the file is binary
 *@RETURN: If the file was successfully created 
 */
bool filesystem_head::createFile(string file_name, unsigned char* content, bool is_binary_file)
{

	return 0;
}

/*
 *Delete the specified file starting at the current directory
 * 
 *@PARAM: The file to delete
 *@RETURN: If the file was able to delete
 */
bool filesystem_head::deleteFile(string file_name)
{

	return 0;
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void filesystem_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
