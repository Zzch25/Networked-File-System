/*
 *(c) Zachary Job
 *2/3/17
 *
 *defs_filesystem.h
 *
 * Documentation regarding a plethora of the included functions
 * is not crystal clear. This header requires testing to truly
 * determine its functionality. 
 *
 * While I could spend twice the time reading, the testing phase
 * will heavily expedite this process
 *
 * This does not have an expectation of being out of the box
 * friendly
 *
 *For use in reading from and writing to the filesystem
 */

#ifndef DEFS_FILESYSTEM
#define DEFS_FILESYSTEM

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#if defined _WIN32 || defined _WIN64 //Assume fileops is fairly uniform in nix for now
	#include <windows.h>
	#include <sys/stat.h>
#else
	#include <unistd.h>
	#include <sys/stat.h>
#endif

#include <dirent.h>

#include "defs_file.h"
#include "defs_debug.h"

#if defined _WIN32 || defined _WIN64
#define FILESYSTEM_STAT_STRUCTp _stat
#else
#define FILESYSTEM_STAT_STRUCTp stat
#endif

#define FILESYSTEM_HEAD_LOCAL_ERROR_COUNTp 5
#define FILESYSTEM_HEAD_WINDOWS_NODE_DELIMETERp '\\'
#define FILESYSTEM_HEAD_NIX_NODE_DELIMETERp '/'

//playing with macros
#if defined _WIN32 || defined _WIN64
	#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
	#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

using namespace std;

/*
 *This class is to provide a concise abstraction
 *of file management. It shall be able to traverse and
 *modify for tansparency.
 */
class filesystem_head : public debug_status
{
	private:
		
		bool
			filesystem_directory_known;

		char
			filesystem_node_delimiter;

		string
			filesystem_current_directory;	

		function<bool(char*)>
			filesystem_change_directory_function;
		
		function<char*(char*,size_t)>
			filesystem_get_directory_function;

		function<int(char*, struct FILESYSTEM_STAT_STRUCTp *)>
			filesystem_get_file_status_function;
	
		#if defined _WIN32 || defined _WIN64 //Assume fileops is fairly uniform in nix for now
			
			function<int(char*)>
				filesystem_create_directory_function;
		
		#else

			function<int(char*)>
				filesystem_create_directory_function;
		
		#endif

		function<int(char*)>
			filesystem_remove_directory_function;

		function<int(char*)>
			filesystem_remove_file_function;

		struct FILESYSTEM_STAT_STRUCTp
			filesystem_status_structure;

		enum class filesystem_local_errors_lookup
		{
			cannot_change_directory = 0,
			cannot_update_directory,
			cannot_open_directory,
			cannot_create_directory,
			cannot_remove_directory
		};

		const string
			filesystem_local_errors[FILESYSTEM_HEAD_LOCAL_ERROR_COUNTp] =
			{
				"Cannot change directory",
				"Cannot update directory",
				"Cannot open directory",
				"Cannot create directory",
				"Cannot create directory"
			};

	public:
		
		filesystem_head(string set_current_directory);
		~filesystem_head();

		inline bool fileExists(string file_path);
		inline bool directoryExists(string directory_path);

		bool changeDirectory(string directory);
		bool stepBackDirectory(int steps_backward);
		bool stepForwardDirectory(string target_node);
		void manuallyRectifyDirectory(string directory);

		shared_ptr<vector<pair<string, bool>>> listCurrentDirectory(string directory = "");

		bool makeDirectory(string directory);
		bool removeDirectory(string directory);

		bool createFile(string file_name, const char* content, bool is_binary_file);
		bool deleteFile(string file_name);
};

//FILESYSTEM HEAD////////////////////////////
/////////////////////////////////////////////

/*
 *A simple initializer for setting the targeted directory.
 *
 *@PARAM: The initial directory to start the head at
 */
filesystem_head::filesystem_head(string set_current_directory):
	debug_status(filesystem_local_errors, debug_status::getMinClassError() + FILESYSTEM_HEAD_LOCAL_ERROR_COUNTp - 1),
	filesystem_current_directory(set_current_directory),
	filesystem_directory_known(true)
{
	#if defined _WIN32 || defined _WIN64 //Assume fileops is fairly uniform in nix for now
		
		filesystem_change_directory_function = _chdir;
		filesystem_get_directory_function = _getcwd;
		filesystem_get_file_status_function = _stat;
		filesystem_create_directory_function = _mkdir;
		filesystem_remove_directory_function = _rmdir;
		filesystem_remove_file_function = remove;

		node_delimeter = FILESYSTEM_HEAD_WINDOWS_NODE_DELIMETERp;

	#else

		filesystem_change_directory_function = chdir;
		filesystem_get_directory_function = getcwd;
		filesystem_get_file_status_function = stat;
		//TO be better updated during testing
		filesystem_create_directory_function = bind(mkdir,placeholders::_1, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		filesystem_remove_directory_function = rmdir;
		filesystem_remove_file_function = remove;

		filesystem_node_delimiter = FILESYSTEM_HEAD_NIX_NODE_DELIMETERp;

	#endif
}

/*
 *A placeholder in the event something requires cleaning
 */
filesystem_head::~filesystem_head()
{

}

/*
 *Regarding whether or not the desired file exists
 *
 *@PARAM: The path of the file specified 
 *@RETURN: Whether or not the file exists 
 */
bool filesystem_head::fileExists(string file_path)
{
	//in place manipulations?
	filesystem_get_file_status_function((char *)file_path.c_str(), &filesystem_status_structure);
	return S_ISREG(filesystem_status_structure.st_mode);
}

/*
 *Regarding whether or not the desired directory exists
 *
 *@PARAM: The path of the directory specified 
 *@RETURN: Whether or not the directory exists 
 */
bool filesystem_head::directoryExists(string directory_path)
{
	//in place manipulations?
	filesystem_get_file_status_function((char *)directory_path.c_str(), &filesystem_status_structure);
	return S_ISDIR(filesystem_status_structure.st_mode);
}

/*
 *Seek the new directory specified
 *
 * I am going to assume during testing I'll find this
 * solution was too good to be true...
 *
 *@PARAM: The directory specified for changing to
 *@RETURN: If the directory has changed or a change was not required
 */
bool filesystem_head::changeDirectory(string directory)
{
	bool
		result;

	char
		*new_working_directory;

	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);

	result = false;
	new_working_directory = nullptr;
	
	//in place manipultion?
	if(result = filesystem_change_directory_function((char *)directory.c_str())) //YES, -1 is non zero therefore true		
	{
		if((new_working_directory = filesystem_get_directory_function(0, 0)) != NULL)
		{
			filesystem_current_directory.assign(new_working_directory);
			free(new_working_directory);
		}
		else
		{
			result = false;
			filesystem_directory_known = false;
			setReturnCode((int)filesystem_local_errors_lookup::cannot_update_directory);
		}
	}
	else
		setReturnCode((int)filesystem_local_errors_lookup::cannot_change_directory);

	return result;
}

/*
 *Return upwards towards the parent directory from the current
 *directory
 *
 *@PARAM: Step backwards as many times as specified 
 *@RETURN: If the function was able to return to the desired parent directory
 */
bool filesystem_head::stepBackDirectory(int steps_backward = 0)
{
	bool
		result;

	char
		node_delimeter;

	int
		matched_delimeters;

	size_t
		string_index;

	string
		parent_directory;

	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);
	result = false;

	parent_directory = filesystem_current_directory;

	for(matched_delimeters = 0, string_index = parent_directory.size() - 1;
				matched_delimeters < steps_backward && string_index >= 0;
					++matched_delimeters, --string_index)
		matched_delimeters += parent_directory[string_index] == filesystem_node_delimiter;

	if(matched_delimeters == steps_backward)
	{
		parent_directory.resize(string_index + 1);
		result = changeDirectory(parent_directory);
	}

	//Error set in the changeDirectory call if needed

	return result;
}

/*
 *Move forward into a child directory starting from the current
 *directory
 *
 * READ interior comment... May end up deprecated before
 * it gets to see the light of day! So then does that make
 * it deprecated? Technically yes.
 *
 *@PARAM: The desired child directory
 *@RETURN: If the function was able to step forward into a specified directory
 */
bool filesystem_head::stepForwardDirectory(string target_node)
{
	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);

	//From what I read this should work... Until then I'm leaving this
	//comment! Modified directory expected from the working one. IF
	//it is so similar... This function may be deprecated unless
	//found syntactically pleasing
	changeDirectory(target_node);

	return 0;
}

/*
 *On inability to update directory, allow manual rectification
 *THIS is not permanent, I expect to test and understand any
 *issues relevant so that I may update the code
 *
 *@PARAM: The new directory value
 */
void filesystem_head::manuallyRectifyDirectory(string directory)
{
	filesystem_directory_known = true;
	filesystem_current_directory = directory;
}

/*
 *List the contents of the head's location in the file structure
 *if not otherwise specified starting from the current directory
 *
 *@PARAM: The specified directory else it will default to the current directory
 *@RETURN: The list of contents and whether or not they are
 *				a directory or file
 */
shared_ptr<vector<pair<string, bool>>> filesystem_head::listCurrentDirectory(string directory)
{
	bool
		type_is_file;

	string
		current_directory,
		target_node;

	DIR
		*directory_descriptor;

	struct dirent
		*directory_data_structure;

	pair<string, bool>
		to_append;

	vector<pair<string, bool>>
		files_and_directories;

	shared_ptr<vector<pair<string, bool>>>
		result;

	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);

	current_directory = directory;

	result = nullptr;

	//I honestly am having a slip and forget if this is valid in CMD...I'll find out soon
	if(current_directory == "")
	{
		current_directory.assign(".");
		current_directory += filesystem_node_delimiter;
	}
	else if(current_directory.back() != filesystem_node_delimiter)
		current_directory += filesystem_node_delimiter;

	if((directory_descriptor = opendir(current_directory.c_str())) != NULL)
	{
		while((directory_data_structure = readdir(directory_descriptor)) != NULL)
		{
			to_append.first = directory_data_structure->d_name;

			target_node = current_directory;
			target_node.append(to_append.first);

			type_is_file = false;

			if(directoryExists(target_node) || (type_is_file = fileExists(target_node)))
			{
				to_append.second = type_is_file;
				files_and_directories.emplace_back(to_append.first, to_append.second);
			}
		}

		closedir(directory_descriptor);
	}
	else
		setReturnCode((int)filesystem_local_errors_lookup::cannot_open_directory);

	if(!files_and_directories.empty())
		result = make_shared<vector<pair<string, bool>>>(files_and_directories);

	return result;
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
	bool
		result;

	result = false;
	
	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);

	//in place manipulation?
	if(!(result = filesystem_create_directory_function((char*)directory.c_str()) == 0))
		setReturnCode((int)filesystem_local_errors_lookup::cannot_create_directory);

	return result;
}

/*
 *Remove the empty specified directory else default to the current directory
 *directory starting from the current directory
 *
 *@PARAM: The directory to remove if specified else default to the current
 *@RETURN: If the desired directory was removed
 */
bool filesystem_head::removeDirectory(string directory = ".")
{
	bool
		result;

	result = false;

	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);

	//In place manipulation?
	if(!(result = filesystem_remove_directory_function((char*)directory.c_str()) == 0))
		setReturnCode((int)filesystem_local_errors_lookup::cannot_remove_directory);

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
bool filesystem_head::createFile(string file_name, const char* content, bool is_binary_file)
{
	bool
		result;

	result = true; //MODIFY !!!

	file_descriptor
		file_to_write(file_name, (file_descriptor::file_mode_e)((int)file_descriptor::file_mode_e::in + (int)is_binary_file));

	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);

	file_to_write.write_file(content, strlen(content));	

	return result; // MODIFY
}

/*
 *Delete the specified file starting at the current directory
 * 
 *@PARAM: The file to delete
 *@RETURN: If the file was able to delete
 */
bool filesystem_head::deleteFile(string file_name)
{
	bool
		result;

	//Again this is aimed to be more of a temporary measure until
	//the software is tested in its desired enviorments
	assert(filesystem_directory_known);

	result = false;

	//manipulation in place?
	if(filesystem_remove_file_function((char*)file_name.c_str()) == 0)
		result = true;

	return result;
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void filesystem_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
