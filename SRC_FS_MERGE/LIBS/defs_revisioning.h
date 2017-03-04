/*
 *(c) Zachary Job
 *
 *defs_revisioning.h
 *
 *For tracking the filesystem and monitoring
 *activity
 */

#ifndef DEFS_REVISIONING
#define DEFS_REVISIONING

//
//
//
//	DESIGN DRAWN - OPTIMIZING
// TIED WITH REVISIONING UPDATE
//
//
//

#include "defs_debug.h"
#include "defs_internal.h"

#define REVISIONING_FILE_LOCAL_ERROR_COUNTp 1
#define REVISIONING_FILESYSTEM_LOCAL_ERROR_COUNTp 1
#define REVISIONING_MANAGER_LOCAL_ERROR_COUNTp 1

using namespace std;

/*
 *Monitoring for modifications of a file providing
 *functions to strip updates, apply updates, and
 *provide other required file manipulations
 */
class revisioning_file : public debug_status
{
	private:
		
		enum class revisioning_file_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			revisioning_file_local_errors[REVISIONING_FILE_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};

	public:

		revisioning_file();
		~revisioning_file();
};

/*
 *Monitoring for modifications of a filesystem providing
 *functions to strip updates, apply updates, and
 *provide other required file manipulations
 */
class revisioning_filesystem : public debug_status
{
	private:
		
		enum class revisioning_filesystem_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			revisioning_filesystem_local_errors[REVISIONING_FILESYSTEM_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};

	public:

		revisioning_filesystem();
		~revisioning_filesystem();
};

/*
 *Wrap the supporting revisioning class functionalities
 *to provide useful management functions. For example
 *building of a representation of the working filesystem
 */
class revisioning_manager : public debug_status
{
	private:
		
		enum class revisioning_manager_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			revisioning_manager_local_errors[REVISIONING_MANAGER_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};

	public:

		revisioning_manager();
		~revisioning_manager();
};

//REVISIONING FILE///////////////////////////
/////////////////////////////////////////////

revisioning_file::revisioning_file():
	debug_status(revisioning_file_local_errors, debug_status::getMinClassError() + REVISIONING_FILE_LOCAL_ERROR_COUNTp - 1)
{
}

revisioning_file::~revisioning_file()
{
}

//REVISIONING FILESYSTEM/////////////////////
/////////////////////////////////////////////

revisioning_filesystem::revisioning_filesystem():
	debug_status(revisioning_filesystem_local_errors, debug_status::getMinClassError() + REVISIONING_FILESYSTEM_LOCAL_ERROR_COUNTp - 1)
{
}

revisioning_filesystem::~revisioning_filesystem()
{
}

//REVISIONING MANAGER////////////////////////
/////////////////////////////////////////////

revisioning_manager::revisioning_manager():
	debug_status(revisioning_manager_local_errors, debug_status::getMinClassError() + REVISIONING_MANAGER_LOCAL_ERROR_COUNTp - 1)
{
}

revisioning_manager::~revisioning_manager()
{
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void revisioning_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
