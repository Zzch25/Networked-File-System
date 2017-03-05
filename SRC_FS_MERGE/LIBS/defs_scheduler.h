/*
 *(c) Zachary Job
 *
 *defs_scheduler.h
 *
 *A means to schedule tasks so that they can be run
 *as requested on a schedule
 *
 *Mutexes will be employed as tasks can be considered queued.
 *This voids the atomic logic. Conditionals can be employed to
 *wake the handler thread.
 *
 *Also similarly to handler there are very few guards. The
 *classes are highly transparent and should be used
 *appropriately
 */

#ifndef DEFS_SCHEDULER
#define DEFS_SCHEDULER

#include <queue>
#include <list>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <condition_variable>

#include "defs_debug.h"
#include "defs_time.h"
#include "defs_handler.h"
#include "defs_concurrency.h"

#define SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp 1
#define SCHEDULER_INTERRUPT_HANDLER_LOCAL_ERROR_COUNTp 1
#define SCHEDULER_RUNSLICE_LOCAL_ERROR_COUNTp 1

using namespace std;

/*
 *Setup the needed information towards handlling
 *requests to run specific functions. No queue is
 *required as the handler receives then manages
 */
class scheduler_interrupt : public debug_status
{
	private:

		static int
			scheduler_available_for_offload;

		static mutex
			scheduler_critical_section_mutex;	

		static condition_variable
			scheduler_handler_call_condition;

		static queue<int>
			scheduler_queued_tasks;

		static unordered_map<int, handler_callback>
			scheduler_interrupt_callbacks;

		enum class scheduler_interrupt_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			scheduler_interrupt_local_errors[SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};

		friend class scheduler_interrupt_handler;

	public:
	
		scheduler_interrupt();
		~scheduler_interrupt();

		bool registerFunction(function<void()> function_pointer, int identifier);
		template <typename... T> bool registerFunctionParameters(void(*function_pointer)(T...), int identifier, T... args);

		bool updateRegistration(int identifier, function<void()> function_pointer);
		template <typename... T> bool updateRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args);
		template <typename... T> bool updateRegistrationParametersOnly(int identifier, T... args);

		bool removeRegistration(int identifier);

		void toggleHandlerSignaling();
		void haltRegistrationHandlers();

		void clean();
};

/*
 *Check for requests to spawn a thread to execute a task
 *Multiple handlers can be had, however it's use is intended
 *for one to one.
 */
class scheduler_interrupt_handler : public debug_status
{
	private:

		bool
			scheduler_handler_set;

		queue<handler_callback>
			scheduler_stripped_tasks;

		enum class scheduler_interrupt_handler_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			scheduler_interrupt_handler_local_errors[SCHEDULER_INTERRUPT_HANDLER_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};

	public:
	
		scheduler_interrupt_handler();
		~scheduler_interrupt_handler();

		void run();

		/*Quick syntax refresher..We aren't in pthreads anymore
		static unique_lock<mutex>
			condition_wait_lock(scheduler_critical_section_mutex);

		scheduler_handler_call_condition.wait(condition_wait_lock, <<<function here>>>);

		scheduler_handler_call_condition.notify_one(); //We don't accept broadcasts here!
		*/
};

/*
 *Utilize a scheduler for execution of tasks
 *based off a designated slice algortihm
 *
 * Note: I have stepped back from time based
 * because I realize I just nearly got way too
 * ambitious. Contexts would require porting parts
 * of my late OS scheduler and that really
 * is a poor idea. The level of handling required
 * especially for networked portions decreases
 * the friendliness of the code. Responsible use
 * of the assigned tasks is a much better move.
 *
 *Run in a blocking mode as it expects user handled
 *threading
 */
class scheduler_runslice : public debug_status
{
	private:
	
		list<handler_callback>
			scheduler_priority_queue;

		enum class scheduler_runslice_mode
		{
			round_robin = 0,
			priorty_round_robin,
			priority_only_round_robin
		};

		enum class scheduler_runslice_local_errors_lookup
		{
			generic_placeholder = 0,
		};

		const string
			scheduler_runslice_local_errors[SCHEDULER_RUNSLICE_LOCAL_ERROR_COUNTp] =
			{
				"Generic placeholder"
			};
		
	public:

		bool
			scheduler_terminator;

		scheduler_runslice();
		~scheduler_runslice();

		void setAlgorithmMode(scheduler_runslice::scheduler_runslice_mode designated_allocator);

		bool enqueueRegistration(function<void()> function_pointer, int identifier, bool is_priority);
		template <typename... T> bool enqueueRegistrationParameters(void(*function_pointer)(T...), int identifier, bool is_priority, T... args);

		bool updateRegistrationPriority(int identifier, bool is_priority);
		bool updateRegistration(int identifier, function<void()> function_pointer);
		template <typename... T> bool updateRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args);
		template <typename... T> bool updateRegistrationParametersOnly(int identifier, T... args);

		bool removeRegistration(int identifier);

		void run();

		void clean();

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
 *Setup a handler with a function without
 *parameters or bound parameters for trigger by event
 *
 *@PARAM: The parameter free or bound function
 *@PARAM: The identifier for use in the lookup
 *@RETURN: If the function could be registered
 */
bool scheduler_interrupt::registerFunction(function<void()> function_pointer, int identifier)
{
	return 0;
}

/*
 *Setup a handler with a function and
 *parameters for trigger by event
 *
 *@PARAM: The funtion to be used
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The function arguments
 *@RETURN: If the function could be registered
 */
template <typename... T>
bool scheduler_interrupt::registerFunctionParameters(void(*function_pointer)(T...), int identifier, T... args)
{
	return 0;
}

/*
 *Update a registration to a bound or parameter free function
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameter free or bound function
 *@RETURN: If the function could be updated
 */
bool scheduler_interrupt::updateRegistration(int identifier, function<void()> function_pointer)
{
	return 0;
}

/*
 *Setup a handler with a function and
 *parameters for trigger by event
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@RETURN: If the function could be updated
 */
template <typename... T>
bool scheduler_interrupt::updateRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args)
{
	return 0;
}

/*
 *Update a handler's parameters only
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameters to apply
 *@RETURN: If the parameters were applied
 */
template <typename... T>
bool scheduler_interrupt::updateRegistrationParametersOnly(int identifier, T... args)
{
	return 0;
}

/*
 *Remove a registration from its handler
 *
 *@PARAM: The registration lookup to remove
 *@RETURN: If the registration was removed
 */
bool scheduler_interrupt::removeRegistration(int identifier)
{
	return 0;
}

/*
 *Dedicate or halt the caller until set to quit towards
 *listening for requests to run handlers.
 */
void scheduler_interrupt::toggleHandlerSignaling()
{

}

/*
 *Kill the registration handler
 */
void scheduler_interrupt::haltRegistrationHandlers()
{

}

/*
 *Clean the structure for being repurposed
 *however finish the queue
 */
void scheduler_interrupt::clean()
{

}

//SCHEDULER INTERRUPT HANDLER////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 */
scheduler_interrupt_handler::scheduler_interrupt_handler():
	debug_status(scheduler_interrupt_handler_local_errors, debug_status::getMinClassError() + SCHEDULER_INTERRUPT_HANDLER_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
scheduler_interrupt_handler::~scheduler_interrupt_handler()
{
}

/*
 *Run the handler cycle
 */
void scheduler_interrupt_handler::run()
{

}

//SCHEDULER RUNSLICE////////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *
 *Allows for configuration without tasks
 */
scheduler_runslice::scheduler_runslice():
	debug_status(scheduler_runslice_local_errors, debug_status::getMinClassError() + SCHEDULER_RUNSLICE_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *Generic cleanup placeholder for transparent cleaning
 *of otherwise ambiguous or user defined data
 */
scheduler_runslice::~scheduler_runslice()
{
}

/*
 *Modify the allowed runslice allocator
 */
void scheduler_runslice::setAlgorithmMode(scheduler_runslice::scheduler_runslice_mode designated_allocator)
{

}

/*
 *Setup a handler with a function without
 *parameters or bound parameters for trigger by event
 *and enqueue it
 *
 *@PARAM: The parameter free or bound function
 *@PARAM: The identifier for use in the lookup
 *@PARAM: If the handler is of function
 *@RETURN: If the function could be registered
 */
bool scheduler_runslice::enqueueRegistration(function<void()> function_pointer, int identifier, bool is_priority)
{
	return 0;
}

/*
 *Setup a handler with a function and
 *parameters for trigger by event and enqueue it
 *
 *@PARAM: The funtion to be used
 *@PARAM: The identifier for use in the lookup
 *@PARAM: If the handler is of priority
 *@PARAM: The function arguments
 *@RETURN: If the function could be registered
 */
template <typename... T>
bool scheduler_runslice::enqueueRegistrationParameters(void(*function_pointer)(T...), int identifier, bool is_priority, T... args)
{
	return 0;
}

/*
 *Update a registration's priority
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: If the handler is of priority
 *@RETURN: If the function could be updated
 */
bool scheduler_runslice::updateRegistrationPriority(int identifier, bool is_priority)
{
	return 0;
}

/*
 *Update a registration to a bound or parameter free function
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameter free or bound function
 *@RETURN: If the function could be updated
 */
bool scheduler_runslice::updateRegistration(int identifier, function<void()> function_pointer)
{
	return 0;
}

/*
 *Setup a handler with a function and
 *parameters for trigger by event
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@RETURN: If the function could be updated
 */
template <typename... T>
bool scheduler_runslice::updateRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args)
{
	return 0;
}

/*
 *Update a handler's parameters only
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameters to apply
 *@RETURN: If the parameters were applied
 */
template <typename... T>
bool scheduler_runslice::updateRegistrationParametersOnly(int identifier, T... args)
{
	return 0;
}

/*
 *Remove a registration from the queue
 *
 *@PARAM: The registration lookup to remove
 *@RETURN: If the registration was removed
 */
bool scheduler_runslice::removeRegistration(int identifier)
{
	return 0;
}

/*
 *Run the scheduler
 *
 *@PARAM:
 *@RETURN:
 */
void scheduler_runslice::run()
{

}

/*
 *Wipe the scheduler structure
 *
 *@PARAM:
 *@RETURN:
 */
void scheduler_runslice::clean()
{

}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void scheduler_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
