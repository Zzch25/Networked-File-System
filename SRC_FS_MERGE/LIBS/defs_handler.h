/*
 *(c) Zachary Job
 *
 *defs_handler.h
 *
 *For the assignment, storage, and management
 *of functions for callback
 */

#ifndef DEFS_HANDLER
#define DEFS_HANDLER

//
//
//
//	DESIGN DRAWN - OPTIMIZING
// TIED WITH REVISIONING UPDATE
//
//
//

#include "defs_debug.h"

#define HANDLER_CALLBACK_LOCAL_ERROR_COUNTp 1

using namespace std;

/*
 *A structure for the storage and management
 *of functions
 */
class handler_callback : public debug_status
{
	private:
		
		enum class handler_callback_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			handler_callback_local_errors[HANDLER_CALLBACK_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};

	public:
		
		handler_callback();
		~handler_callback();
};

//HANDLER CALLBACK///////////////////////////
/////////////////////////////////////////////

handler_callback::handler_callback():
	debug_status(handler_callback_local_errors, debug_status::getMinClassError() + HANDLER_CALLBACK_LOCAL_ERROR_COUNTp - 1)
{
}

handler_callback::~handler_callback()
{
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void handler_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
