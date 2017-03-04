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

		/*
			_25_..._25_..._25_			
			_26_..._26_			
			For lazy vi reg update later, and 25 of course 

			_25_setFunction_25_()_25_
			_25_setParameters_25_()_25_
			_25_run_25_()_25_
		 */
};

//HANDLER CALLBACK///////////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *
 *Placeholder for generic initialization of the
 *handler data for callback
 */
handler_callback::handler_callback():
	debug_status(handler_callback_local_errors, debug_status::getMinClassError() + HANDLER_CALLBACK_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
handler_callback::~handler_callback()
{
}

/*
 *Load the function to the callback object
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_setFunction_26_
{

}
*/

/*
 *Load or update the function parameters for the
 *callback object
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_setParameters_26_
{

}
*/

/*
 *Run the callback
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_run_26_
{

}
*/

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void handler_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
