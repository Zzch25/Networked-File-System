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

//
//
//
//	DESIGN DRAWN - OPTIMIZING
// TIED WITH REVISIONING UPDATE
//
//
//

#include "defs_debug.h"
#include "defs_handler.h"

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
};

/*
 *Utilize a time scheduler for execution of tasks
 *based off a designated time slice algortihm
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
};


//SCHEDULER INTERRUPT////////////////////////
/////////////////////////////////////////////

scheduler_interrupt::scheduler_interrupt():
	debug_status(scheduler_interrupt_local_errors, debug_status::getMinClassError() + SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp - 1)
{
}

scheduler_interrupt::~scheduler_interrupt()
{
}

//SCHEDULER_TIMESLICE////////////////////////
/////////////////////////////////////////////

scheduler_timeslice::scheduler_timeslice():
	debug_status(scheduler_timeslice_local_errors, debug_status::getMinClassError() + SCHEDULER_TIMESLICE_LOCAL_ERROR_COUNTp - 1)
{
}

scheduler_timeslice::~scheduler_timeslice()
{
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void scheduler_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
