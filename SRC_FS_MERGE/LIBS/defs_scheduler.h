/*
 *(c) Zachary Job
 *
 *defs_scheduler.h
 *
 *A means to schedule tasks so that they can be run
 *as requested on a schedule
 */

#ifndef DEFS_SCHEDULER
#define DEFS_SCHEDULER

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
#include "defs_handler.h"
#include "defs_concurrency.h"

#define SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp 1
#define SCHEDULER_TIMESLICE_LOCAL_ERROR_COUNTp 1

using namespace std;

/*
 *Offer a means to interrupt execution to attend to
 *scheduled tasks
 */
class scheduler_interrupt : public debug_status
{
	private:
		
		enum class scheduler_interrupt_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			scheduler_interrupt_local_errors[SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};

	public:
	
		scheduler_interrupt();
		~scheduler_interrupt();

		/*
			_25_..._25_..._25_			
			_26_..._26_			
			For lazy vi reg update later, and 25 of course 

			_25_registerFunction_25_()_25_
			_25_updateRegistration_25_()_25_
			_25_updateRegistrationParameters_25_()_25_
			_25_removeRegistration_25_()_25_
			_25_clean_25_()_25_
		 */
};

/*
 *Utilize a time scheduler for execution of tasks
 *based off a designated time slice algortihm
 *
 *Allow for a threaded and blocking mode
 */
class scheduler_timeslice : public debug_status
{
	private:
		
		enum class scheduler_timeslice_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			scheduler_timeslice_local_errors[SCHEDULER_TIMESLICE_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};
		
	public:

		scheduler_timeslice();
		~scheduler_timeslice();

		/*
			_25_..._25_..._25_			
			_26_..._26_			
			For lazy vi reg update later, and 25 of course 

			_25_setAlgorithmMode_25_()_25_
			_25_enqueueRegistration_25_()_25_
			_25_removeRegistration_25_()_25_
			_25_updateRegistration_25_()_25_
			_25_updateRegistrationParameters_25_()_25_
			_25_updateTerminator_25_()_25_
			_25_suspendScheduling_25_()_25_
			_25_startScheduling_25_()_25_
			_25_clean_25_()_25_
		 */
};


//SCHEDULER INTERRUPT////////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *
 *Allows for configuration without including tasks
 */
scheduler_interrupt::scheduler_interrupt():
	debug_status(scheduler_interrupt_local_errors, debug_status::getMinClassError() + SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
scheduler_interrupt::~scheduler_interrupt()
{
}

/*
 *Setup a handler with a function and/or
 *parameters for trigger by event
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_registerFunction_26_
{

}
*/

/*
 *Update a handler regarding its function
 *and/or parameters 
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_updateRegistration_26_
{

}
*/

/*
 *Solely update a registers parameters
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_updateRegistrationParameters_26_
{

}
*/

/*
 *Remove a registration from its handler
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_removeRegistration_26_
{

}
*/

/*
 *Wipe the handlers and the structure
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_clean_26_
{

}
*/

//SCHEDULER_TIMESLICE////////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *
 *Allows for configuration without tasks
 */
scheduler_timeslice::scheduler_timeslice():
	debug_status(scheduler_timeslice_local_errors, debug_status::getMinClassError() + SCHEDULER_TIMESLICE_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
scheduler_timeslice::~scheduler_timeslice()
{
}

/*
 *Modify the allowed timeslice allocator
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_setAlgorithmMode_26_
{

}
*/

/*
 *Enlist a new handler registration with
 *parameters
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_enqueueRegistration_26_
{

}
*/

/*
 *Remove a registration from the queue
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_removeRegistration_26_
{

}
*/

/*
 *Update a registration by function and/or
 *parameters
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_updateRegistration_26_
{

}
*/

/*
 *Update a registrations parameters only
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_updateRegistrationParameters_26_
{

}
*/

/*
 *Halt the scheduler
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_suspendScheduling_26_
{

}
*/

/*
 *Resume the scheduler
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_startScheduling_26_
{

}
*/

/*
 *Modify the terminating function
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_updateTerminator_26_
{

}
*/

/*
 *Wipe the scheduler structure
 *
 *@PARAM:
 *@RETURN:
 */
/*
_26_clean_26_
{

}
*/

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void scheduler_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
