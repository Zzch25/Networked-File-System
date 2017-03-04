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

//_DEL_
//Three parameter conflicts are in need of
//resolution. I'll decide after developing
//the tree, handler, and scheduler classes
//
//This is unusual... However I'm
//experimenting for the sake of trying
//something new!
//_DEL_

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
 *
 *I was debating how heavy this may end up. This was
 *one of my interests whether or not to have it dedicated
 *to a filesystem object to strip file info to data encoded
 *in the filesystem tree. I'm trying to envision a better
 *way within a very large network to handle this aside from
 *the obvious answers. This is a source of some of the mentioned
 *conflicts.
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

		/*
			_25_..._25_..._25_			
			_26_..._26_			
			For lazy vi reg update later, and 25 of course 

			_25_assignFile_25_()_25_
			_25_generateHash_25_()_25_
			_25_checkHash_25_()_25_
			_25_stripChanges_25_()_25_
			_25_applyChanges_25_()_25_
		 */
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

		/*
			_25_..._25_..._25_			
			_26_..._26_			
			For lazy vi reg update later, and 25 of course 

			_25_updateEncoding_25_()_25_
			_25_getEncoding_25_()_25_
			_25_isLocal_25_()_25_
			_25_getChanges_25_()_25_
			_25_localizeChanges_25_()_25_
			_25_setPathHiveStatus_25_()_25_
			_25_getPathHiveStatus_25_()_25_
		 */
};

//REVISIONING FILE///////////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *
 *Allows for initilization with a file if demanded
 */
revisioning_file::revisioning_file():
	debug_status(revisioning_file_local_errors, debug_status::getMinClassError() + REVISIONING_FILE_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
revisioning_file::~revisioning_file()
{
}

/*
 *Assign a file for operations to occur on
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_assignFile_26_
{

}
*/

/*
 *Create a "unique" hash for file
 *identification. Also, thank goodness
 *I didn't implement SHA1...
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_generateHash_26_
{

}
*/

/*
 *Verify the input hash is the same as that
 *of the loaded file 
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_checkHash_26_
{

}
*/

/*
 *Return modifications to the file in an applicable
 *formats for updating of other instances
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_stripChanges_26_
{

}
*/

/*
 *Apply stripped changes to a current file
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_applyChanges_26_
{

}
*/

//REVISIONING FILESYSTEM/////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *
 *Allows for generation with a default hash
 */
revisioning_filesystem::revisioning_filesystem():
	debug_status(revisioning_filesystem_local_errors, debug_status::getMinClassError() + REVISIONING_FILESYSTEM_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
revisioning_filesystem::~revisioning_filesystem()
{
}

/*
 *Get and set the new encoding for an updated filesystem
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_updateEncoding_26_
{

}
*/

/*
 *Return the encoding last set for the filesystem
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_getEncoding_26_
{

}
*/

/*
 *See if a path requested is part of the last
 *encoding, then if so whether or not it is local
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_isLocal_26_
{

}
*/

/*
 *Return the changes made to the local filesystem
 *in relation to the last encoding
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_getChanges_26_
{

}
*/

/*
 *Apply changes specified to the host filesystem
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_localizeChanges_26_
{

}
*/

/*
 *Allow the toggling of what a node permits regarding
 *exclusion, commandeering, or joining the hive at
 *a specific node
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_setPathHiveStatus_26_
{

}
*/

/*
 *Get a node's hive status
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_getPathHiveStatus_26_
{

}
*/


//QA/////////////////////////////////////////
/////////////////////////////////////////////

void revisioning_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
