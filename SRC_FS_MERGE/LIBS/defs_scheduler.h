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
 *
 *	NOTE:
 *
 *	This is still in progress. It's very broken :)
 *	just an idea placeholder so that I can patch it up
 *	when I have time to come back to it
 *
 *
 *Also similarly to handler there are very few guards. The
 *classes are highly transparent and should be used
 *appropriately
 */

#ifndef DEFS_SCHEDULER
#define DEFS_SCHEDULER

#include <type_traits>
#include <typeinfo>
#include <memory>
#include <queue>
#include <list>
#include <unordered_map>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <signal.h>

#include "defs_debug.h"
#include "defs_time.h"
#include "defs_handler.h"
#include "defs_concurrency.h"

#define SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp 3
#define SCHEDULER_INTERRUPT_SIGNAL_COUNTp 8

#define SCHEDULER_RUNSLICE_LOCAL_ERROR_COUNTp 4
#define SCHEDULER_RUNSLICE_ROUND_ROBIN_PRIORITY_CYCLES 1
#define SCHEDULER_RUNSLICE_PRIORITY_ROUND_ROBIN_PRIORITY_CYCLES 1

using namespace std;

/*
 *Setup the needed information towards handlling
 *requests to run specific functions. No queue is
 *required as the handler receives then manages
 
		NOTE: Mutex logic needs cleanup, this is a
				hack and needs some fine tuning
				I forgot that I needed to account for
				the fact I didn't want to lock down the
				signal handling thread

				Consider this temporary "bolted on"
				logic


				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				ALSO it's going to blow up with the
				temporary references. I'm going
				to patch it all over to smart
				pointers!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

 *REQUIRES proper use. Obviously multiple objects
 *could be used. This however
 */
class scheduler_interrupt : public debug_status
{
	private:

		static scheduler_interrupt
			scheduler_instance;

		atomic_flag
			scheduler_is_initialized;

		bool
			scheduler_available_for_offload;

		mutex
			scheduler_callback_mutex;

		mutex
			scheduler_per_queue_mutex[SCHEDULER_INTERRUPT_SIGNAL_COUNTp];

		/*condition_variable
			scheduler_handler_call_condition;*/

		queue<pair<int, bool>>
			scheduler_signal_queues[SCHEDULER_INTERRUPT_SIGNAL_COUNTp];

		unordered_map<int, pair<int, handler_callback>>
			scheduler_callbacks;

		enum class scheduler_interrupt_signal_queues_offsets
		{
			sigabrt = 0,// = SIGABRT
			sigfpe,// = SIGFPE
			sigill,// = SIGILL
			sigint,// = SIGINT
			sigsegv,// = SIGSEGV
			sigterm,// = SIGTERM
			sigusr1,// = SIGUSR1
			sigusr2// = SIGUSR2
		};
	
		const int
			scheduler_signal_value_lookup[SCHEDULER_INTERRUPT_SIGNAL_COUNTp]
			{
				SIGABRT,
				SIGFPE,
				SIGILL,
				SIGINT,
				SIGSEGV,
				SIGTERM,
				SIGUSR1,
				SIGUSR2
			};

		unordered_map<int, int>
			scheduler_signal_value_reverse_lookup
			{
				{scheduler_signal_value_lookup[(int)scheduler_interrupt_signal_queues_offsets::sigabrt], (int)scheduler_interrupt_signal_queues_offsets::sigabrt},
				{scheduler_signal_value_lookup[(int)scheduler_interrupt_signal_queues_offsets::sigfpe], (int)scheduler_interrupt_signal_queues_offsets::sigfpe},
				{scheduler_signal_value_lookup[(int)scheduler_interrupt_signal_queues_offsets::sigill], (int)scheduler_interrupt_signal_queues_offsets::sigill},
				{scheduler_signal_value_lookup[(int)scheduler_interrupt_signal_queues_offsets::sigsegv], (int)scheduler_interrupt_signal_queues_offsets::sigsegv},
				{scheduler_signal_value_lookup[(int)scheduler_interrupt_signal_queues_offsets::sigterm], (int)scheduler_interrupt_signal_queues_offsets::sigterm},
				{scheduler_signal_value_lookup[(int)scheduler_interrupt_signal_queues_offsets::sigusr1], (int)scheduler_interrupt_signal_queues_offsets::sigusr1},
				{scheduler_signal_value_lookup[(int)scheduler_interrupt_signal_queues_offsets::sigusr2], (int)scheduler_interrupt_signal_queues_offsets::sigusr2}
			};

		enum class scheduler_interrupt_local_errors_lookup
		{
			identifier_not_available = 0,
			callback_dropped_prematurely,
			queue_is_empty,
		};

		const string
			scheduler_interrupt_local_errors[SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp] =
			{
				"Identifier not available",
				"Callback_dropped_prematurely",
				"Queue_is_empty",
			};

		static function<void(int)> handlerSignalFunction;
		void handlerUpdateAndRun(int signal);

		template <typename... T> inline bool registerShared(int identifier, void(*function_pointer)(T...), scheduler_interrupt_signal_queues_offsets signal_type, bool persist, T... args);
		inline bool registerSharedParameterFree(int identifier, void(*function_pointer)(), scheduler_interrupt_signal_queues_offsets signal_type, bool persist);
		inline bool registerSharedHelper(int identifier, handler_callback callback_payload, scheduler_interrupt_signal_queues_offsets signal_type, bool persist);
		template <typename... T> inline bool updateShared(int identifier, void(*function_pointer)(T...), T... args);
		inline bool updateSharedParameterFree(int identifier, void (*function_pointer)());
		inline bool updateSharedHelper(int identifier, unordered_map<int, pair<int, handler_callback>>::iterator *get_insertion);

	public:
	
		scheduler_interrupt();
		~scheduler_interrupt();

		bool registerFunction(int identifier, void(*function_pointer)(), scheduler_interrupt_signal_queues_offsets signal_type, bool persist);
		template <typename... T> bool registerFunctionParameters(int identifier, void(*function_pointer)(T...), T... args, scheduler_interrupt_signal_queues_offsets signal_type, bool persist);

		bool updateRegistration(int identifier, void (*function_pointer)());
		template <typename... T> bool updateRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args);
		template <typename... T> bool updateRegistrationParametersOnly(int identifier, T... args);

		bool removeRegistration(int identifier);

		void toggleHandlerSignaling();

		void clean();

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

		char
			scheduler_priority_runs_last;

		list<pair<int, pair<bool, handler_callback>>>
			scheduler_priority_queue,
			scheduler_normal_queue;

		unordered_map<int, bool>
			scheduler_is_priority_lookup;

		enum class scheduler_runslice_mode
		{
			round_robin = 0,
			priority_round_robin,
			priority_only_round_robin
		};

		scheduler_runslice_mode scheduler_mode;

		enum class scheduler_runslice_local_errors_lookup
		{
			identifier_not_available = 0,
			identifier_is_occupied,
			no_tasks_available,
			no_priority_tasks_available
		};

		const string
			scheduler_runslice_local_errors[SCHEDULER_RUNSLICE_LOCAL_ERROR_COUNTp] =
			{
				"Identifier not available",
				"Identifier is occupied",
				"No tasks available",
				"No priority tasks available"
			};

		template <typename... T> inline bool enqueueShared(int identifier, void (*function_pointer)(T...), T... args, bool is_priority, bool persist);
		inline bool enqueueSharedParameterFree(int identifier, void (*function_pointer)(), bool is_priority, bool persist);
		inline bool enqueueSharedHelper(int identifier);
		template <typename... T> inline bool updateShared(int identifier, void(*function_pointer)(T...), T... args);
		inline bool updateSharedParameterFree(int identifier, void (*function_pointer)());
		inline bool updateSharedHelper(int identifier, list<pair<int, pair<bool, handler_callback>>>::iterator *get_iterator_queue);
	
		inline bool removeRegistrationInline(int identifier);
		
		inline void runOperationInline(list<pair<int, pair<bool, handler_callback>>> run_queue);

	public:

		scheduler_runslice();
		~scheduler_runslice();

		void setAlgorithmMode(scheduler_runslice::scheduler_runslice_mode designated_allocator);

		bool enqueueRegistration(int identifier, void (*function_pointer)(), bool is_priority, bool persist);
		template <typename... T> bool enqueueRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args, bool is_priority, bool persist);

		bool updateRegistrationPriority(int identifier, bool is_priority);
		bool updateRegistration(int identifier, void (*function_pointer)());
		template <typename... T> bool updateRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args);
		template <typename... T> bool updateRegistrationParametersOnly(int identifier, T... args);

		bool removeRegistration(int identifier);

		void run();

		void clean();

};


//SCHEDULER INTERRUPT////////////////////////
/////////////////////////////////////////////

/*
 *For use in locking the queue bucket required when signaled 
 *
 *@PARAM: The signal to be passed as required by c signal
 */
void scheduler_interrupt::handlerUpdateAndRun(int signal)
{
	int
		offset,
		get_identifier;

	offset = scheduler_signal_value_reverse_lookup[signal];

	//The mutex logic is high...

	if(scheduler_available_for_offload)
	{
		scheduler_per_queue_mutex[offset].lock();

		if(!scheduler_signal_queues[offset].empty())
		{
			get_identifier = (scheduler_signal_queues[offset].back()).first;
			scheduler_callback_mutex.lock();
			auto get_iterator = scheduler_callbacks.find(get_identifier);
		
			if(get_iterator != scheduler_callbacks.end())
				(get_iterator->second).second.run();
			else
				setReturnCode((int)scheduler_interrupt_local_errors_lookup::callback_dropped_prematurely);
			scheduler_callback_mutex.unlock();

			if((scheduler_signal_queues[offset].back()).second == true)
				scheduler_signal_queues[offset].push(pair<int, bool>(get_identifier, true));
			else
			{
				scheduler_callback_mutex.lock();
				scheduler_callbacks.erase(get_identifier);	
				scheduler_callback_mutex.unlock();
			}

			scheduler_signal_queues[offset].pop();
		}
		else
			setReturnCode((int)scheduler_interrupt_local_errors_lookup::queue_is_empty);

		scheduler_per_queue_mutex[offset].unlock();
	}
}
	
/*
 *For compaction of the registration functions and ease
 *of updating. Allows registration of functions by parameter
 *and/or function
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The signal type associated with the function
 *@PARAM: If the signal ought to persist and re-queue
 *@PARAM: The parameters to accompany the function
 *@RETURN: If the function could be registered
 */
template <typename... T>
bool scheduler_interrupt::registerShared(int identifier, void(*function_pointer)(T...), scheduler_interrupt_signal_queues_offsets signal_type, bool persist, T... args)
{
	handler_callback
		callback_payload(nullptr);

	callback_payload.setFunction(function_pointer, args...);

	return registerSharedHelper(identifier, callback_payload, signal_type, persist);
}

/*
 *For compaction of the registration functions and ease
 *of updating. Allows registration of functions by parameter
 *and/or function
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The signal type associated with the function
 *@PARAM: If the signal ought to persist and re-queue
 *@RETURN: If the function could be registered
 */
bool scheduler_interrupt::registerSharedParameterFree(int identifier, void(*function_pointer)(), scheduler_interrupt_signal_queues_offsets signal_type, bool persist)
{
	handler_callback
		callback_payload(nullptr);

	callback_payload.setBoundOrFreeFunction(function_pointer);

	return registerSharedHelper(identifier, callback_payload, signal_type, persist);
}

/*
 *Share the common actions fo the registerShared functions 
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The signal type associated with the function
 *@PARAM: If the signal ought to persist and re-queue
 *@RETURN: If the function could be registered
 */
bool scheduler_interrupt::registerSharedHelper(int identifier, handler_callback callback_payload, scheduler_interrupt_signal_queues_offsets signal_type, bool persist)
{
	bool
		get_insertion_result,
		result;

	result = false;
	
	scheduler_callback_mutex.lock();
	get_insertion_result = scheduler_callbacks.emplace(identifier, pair<int, handler_callback>((int)signal_type, callback_payload)).second;
	scheduler_callback_mutex.unlock();
	
	if(get_insertion_result)
	{
		//FORCE a lock here, this should not have a blocking instance to halt
		//other execution
		scheduler_per_queue_mutex[(int)signal_type].lock();
		scheduler_signal_queues[(int)signal_type].emplace(pair<int, bool>(identifier, persist));
		scheduler_per_queue_mutex[(int)signal_type].unlock();
		
		result = true;
	}
		
	return result;
}

/*
 *For code compaction and maintainability. Updates
 *an entry accordingly.
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@RETURN: If the function could be updated
 */
template <typename... T> 
bool scheduler_interrupt::updateShared(int identifier, void(*function_pointer)(T...), T... args)
{
	bool
		result;

	unordered_map<int, pair<int, handler_callback>>::iterator
		get_insertion;
	
	result = updateSharedHelper(identifier, &get_insertion);

	if(result)
	{
		//FORCE a lock here, this should not have a blocking instance to halt
		//other execution
		scheduler_per_queue_mutex[(int)get_insertion->second.first].lock();

		if(function_pointer == nullptr)
			(get_insertion->second.second).setParameters(args...);
		else
			(get_insertion->second.second).setFunction(function_pointer, args...);

		scheduler_per_queue_mutex[(int)get_insertion->second.first].unlock();
	}

	return result;
}

/*
 *For code compaction and maintainability. Updates
 *an entry so accordingly.
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@RETURN: If the function could be updated
 */
bool scheduler_interrupt::updateSharedParameterFree(int identifier, void (*function_pointer)())
{
	bool
		result;

	unordered_map<int, pair<int, handler_callback>>::iterator
		get_insertion;

	result = updateSharedHelper(identifier, &get_insertion);

	if(result)
	{
		//FORCE a lock here, this should not have a blocking instance to halt
		//other execution
		scheduler_per_queue_mutex[(int)get_insertion->second.first].lock();

		(get_insertion->second.second).setBoundOrFreeFunction(function_pointer);

		scheduler_per_queue_mutex[(int)get_insertion->second.first].unlock();
	}

	return result;
}

/*
 *For compaction of the registration functions and ease
 *of updating. 
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@RETURN: If the function could be updated
 */
bool scheduler_interrupt::updateSharedHelper(int identifier, unordered_map<int, pair<int, handler_callback>>::iterator *get_insertion)
{
	scheduler_callback_mutex.lock();
	*get_insertion = scheduler_callbacks.find(identifier);
	scheduler_callback_mutex.unlock();
	
	return *get_insertion != scheduler_callbacks.end();
}

/*
 *Generic initializer
 *
 * Another point that needs fixing
 * obviously out of scope compiler only
 * fix
 *
 *Allows for configuration without including tasks
 */
scheduler_interrupt::scheduler_interrupt():
	debug_status(scheduler_interrupt_local_errors, debug_status::getMinClassError() + SCHEDULER_INTERRUPT_LOCAL_ERROR_COUNTp - 1),
	scheduler_available_for_offload(false),
	scheduler_is_initialized(ATOMIC_FLAG_INIT)
{
	assert(scheduler_is_initialized.test_and_set());

	for(auto iterator : scheduler_signal_value_lookup)
	{
		handlerSignalFunction = function<void(int)>([this](int signal){this->handlerUpdateAndRun(signal);});
		signal(iterator, *handlerSignalFunction.target<void(*)(int)>());
	}
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
 *Identifier misses can be considered not terribly
 *lethal. They will be forgiven for now
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameter free or bound function
 *@PARAM: The signal type associated with the function
 *@PARAM: If the signal ought to persist and re-queue
 *@RETURN: If the function could be registered
 */
bool scheduler_interrupt::registerFunction(int identifier, void(*function_pointer)(), scheduler_interrupt::scheduler_interrupt_signal_queues_offsets signal_type, bool persist)
{
	return registerSharedParameterFree(identifier, function_pointer, signal_type, persist);
}

/*
 *Setup a handler with a function and
 *parameters for trigger by event
 *
 *Identifier misses can be considered not terribly
 *lethal. They will be forgiven for now
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@PARAM: The signal type associated with the function
 *@PARAM: If the signal ought to persist and re-queue
 *@RETURN: If the function could be registered
 */
template <typename... T>
bool scheduler_interrupt::registerFunctionParameters(int identifier, void(*function_pointer)(T...), T... args, scheduler_interrupt::scheduler_interrupt_signal_queues_offsets signal_type, bool persist)
{
	return registerShared<T...>(identifier, function_pointer, signal_type, persist, args...);
}

/*
 *Update a registration to a bound or parameter free function
 *
 *Consider utilizing a singular function for all of the
 *update functions
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameter free or bound function
 *@RETURN: If the function could be updated
 */
bool scheduler_interrupt::updateRegistration(int identifier, void (*function_pointer)())
{
	return updateSharedParameterFree(identifier, function_pointer);
}

/*
 *Setup a handler with a function and
 *parameters for trigger by event
 *
 *Consider utilizing a singular function for all of the
 *update functions
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@RETURN: If the function could be updated
 */
template <typename... T>
bool scheduler_interrupt::updateRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args)
{
	return updateShared<T...>(identifier, function_pointer, args...);
}

/*
 *Update a handler's parameters only
 *
 *Consider utilizing a singular function for all of the
 *update functions
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameters to apply
 *@RETURN: If the parameters were applied
 */
template <typename... T>
bool scheduler_interrupt::updateRegistrationParametersOnly(int identifier, T... args)
{
	return updateShared<T...>(identifier, nullptr, args...);
}

/*
 *Remove a registration from its handler
 *
 *This truly is intended for diagnostic means.
 *It should not be employed in the final product.
 *Instead thoughtful design is essential
 *
 *@PARAM: The registration lookup to remove
 *@RETURN: If the registration was removed
 */
bool scheduler_interrupt::removeRegistration(int identifier)
{
	bool
		result;

	result = false;

	//FORCE a lock here, this should not have a blocking instance to halt
	//other execution
	scheduler_callback_mutex.lock();
	if(scheduler_callbacks.erase(identifier) > 0)
		result = true;
	scheduler_callback_mutex.unlock();
	
	return result;
}

/*
 *Dedicate or halt the caller until set to quit towards
 *listening for requests to run handlers.
 */
void scheduler_interrupt::toggleHandlerSignaling()
{
	//No need to be THAT accurate, locking will be ignored
	scheduler_available_for_offload = !scheduler_available_for_offload;
}

/*
 *Clean the structure for being repurposed
 *however finish the queue
 */
void scheduler_interrupt::clean()
{
	int
		iterator;

	scheduler_callback_mutex.lock();
	scheduler_callbacks.clear();
	scheduler_callback_mutex.unlock();
	
	//Wipe the queues to eliminate
	for(iterator = 0; iterator < SCHEDULER_INTERRUPT_SIGNAL_COUNTp; ++iterator)
	{
		scheduler_per_queue_mutex[iterator].lock();
		while(!scheduler_signal_queues[iterator].empty())
			scheduler_signal_queues[iterator].pop();
		scheduler_per_queue_mutex[iterator].unlock();
	}
}

//SCHEDULER RUNSLICE////////////////////////
/////////////////////////////////////////////

/*
 *General enqueue function for code compaction
 *and maintainability. Enqueue a handler into the
 *specified queue for use when ready. The handler
 *shall have parameters for its function.
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@PARAM: If the handler is of priority
 *@PARAM: If the item should re-enter the queue
 *@RETURN: If the function could be registered
 */
template <typename... T> 
bool scheduler_runslice::enqueueShared(int identifier, void(*function_pointer)(T...), T... args, bool is_priority, bool persist)
{
	bool
		result;

	handler_callback
		callback_payload(nullptr);

	result = enqueueSharedHelper(identifier);

	if(result)
	{
		callback_payload.setFunction(function_pointer, args...);

		if(is_priority)
			scheduler_priority_queue.emplace_front(pair<int, pair<bool, handler_callback>>(pair<int, pair<bool, handler_callback>>(identifier, pair<bool, handler_callback>(persist, callback_payload))));
		else
			scheduler_normal_queue.emplace_front(pair<int, pair<bool, handler_callback>>(pair<int, pair<bool, handler_callback>>(identifier, pair<bool, handler_callback>(persist, callback_payload))));

		scheduler_is_priority_lookup.emplace(identifier, is_priority);
	}

	return result;
}

/*
 *General enqueue function for code compaction
 *and maintainability. Enqueue a handler into the
 *specified queue for use when ready.
 *The handler shall be without parameters to its
 *function.
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: If the handler is of priority
 *@PARAM: If the item should re-enter the queue
 *@RETURN: If the function could be registered
 */
bool scheduler_runslice::enqueueSharedParameterFree(int identifier, void(*function_pointer)(),  bool is_priority, bool persist)
{
	bool
		result;

	handler_callback
		callback_payload(nullptr);

	result = enqueueSharedHelper(identifier);

	if(result)
	{
		callback_payload.setBoundOrFreeFunction(function_pointer);

		if(is_priority)
			scheduler_priority_queue.emplace_front(pair<int, pair<bool, handler_callback>>(pair<int, pair<bool, handler_callback>>(identifier, pair<bool, handler_callback>(persist, callback_payload))));
		else
			scheduler_normal_queue.emplace_front(pair<int, pair<bool, handler_callback>>(pair<int, pair<bool, handler_callback>>(identifier, pair<bool, handler_callback>(persist, callback_payload))));

		scheduler_is_priority_lookup.emplace(identifier, is_priority);
	}

	return result;
}

/*
 *General enqueue function for code compaction
 *and maintainability. Enqueue a handler into the
 *specified queue for use when ready.
 *
 *@PARAM: Verify the idenitifier is not taken
 *@RETURN: If the identifier
 */
bool scheduler_runslice::enqueueSharedHelper(int identifier)
{
	bool
		result;

	result = false;

	auto get_iterator = scheduler_is_priority_lookup.find(identifier);

	if(get_iterator == scheduler_is_priority_lookup.end())
		result = true;
	else
		setReturnCode((int)scheduler_runslice_local_errors_lookup::identifier_is_occupied);

	return result;
}

/*
 *For code compaction and maintainability. Updates
 *an entry accordingly.
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@RETURN: If the function could be updated
 */
template <typename... T> 
bool scheduler_runslice::updateShared(int identifier, void(*function_pointer)(T...), T... args)
{
	bool
		result;

	list<pair<int, pair<bool, handler_callback>>>::iterator
		get_iterator_queue;
	
	result = updateSharedHelper(identifier, &get_iterator_queue);

	if(result)
	{
		if(function_pointer == nullptr)
			get_iterator_queue->second.second.setParameters(args...);
		else
			get_iterator_queue->second.second.setFunction(function_pointer, args...);
	}

	return result;
}

/*
 *For code compaction and maintainability. Updates
 *an entry so accordingly.
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@RETURN: If the function could be updated
 */
bool scheduler_runslice::updateSharedParameterFree(int identifier, void (*function_pointer)())
{
	bool
		result;

	list<pair<int, pair<bool, handler_callback>>>::iterator
		get_iterator_queue;

	result = updateSharedHelper(identifier, &get_iterator_queue);
	
	if(result)
		get_iterator_queue->second.second.setBoundOrFreeFunction(function_pointer);

	return result;
}

/*
 *For completion of the updateShared functions
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@RETURN: If the function could be updated
 */
bool scheduler_runslice::updateSharedHelper(int identifier, list<pair<int, pair<bool, handler_callback>>>::iterator *get_iterator_queue)
{
	bool
		result;

	result = false;

	auto get_iterator_lookup = scheduler_is_priority_lookup.find(identifier);

	if(get_iterator_lookup != scheduler_is_priority_lookup.end())
	{
		//No checking here, this should be internally managed
		if(get_iterator_lookup->second)
			*get_iterator_queue = find_if(scheduler_priority_queue.begin(), scheduler_priority_queue.end(),
					[identifier](pair<int, pair<bool, handler_callback>> iterator){return iterator.first == identifier;});
		else
			*get_iterator_queue = find_if(scheduler_normal_queue.begin(), scheduler_normal_queue.end(),
					[identifier](pair<int, pair<int, handler_callback>> iterator){return iterator.first == identifier;});

		result = true;
	}
	else
		setReturnCode((int)scheduler_runslice_local_errors_lookup::identifier_not_available);

	return result;
}

/*
 *Inline for calls so that remove can be called
 *internally with lower penalties.
 *Remove a registration from the queue
 *
 *@PARAM: The registration lookup to remove
 *@RETURN: If the registration was removed
 */
bool scheduler_runslice::removeRegistrationInline(int identifier)
{
	bool
		result;

	result = false;

	auto get_iterator_lookup = scheduler_is_priority_lookup.find(identifier);

	if(get_iterator_lookup != scheduler_is_priority_lookup.end())
	{
		//No checks, this should be handled internally
		if(get_iterator_lookup->second)
		{
			auto get_iterator_queue = find_if(scheduler_priority_queue.begin(), scheduler_priority_queue.end(),
					[identifier](pair<int, pair<bool, handler_callback>> iterator){return iterator.first == identifier;});
			scheduler_priority_queue.erase(get_iterator_queue);
		}
		else
		{
			auto get_iterator_queue = find_if(scheduler_normal_queue.begin(), scheduler_normal_queue.end(),
					[identifier](pair<int, pair<bool, handler_callback>> iterator){return iterator.first == identifier;});
			scheduler_normal_queue.erase(get_iterator_queue);
		}

		scheduler_is_priority_lookup.erase(get_iterator_lookup);
		result = true;
	}
	else
		setReturnCode((int)scheduler_runslice_local_errors_lookup::identifier_not_available);

	return result;
}

/*
 *For compaction of code.
 *Perform the run operation
 *
 *@PARAM: The desired list, this pass is done in hopes
 *				it will be optimized out since the alternative
 *				is large and would not save much space
 */
void scheduler_runslice::runOperationInline(list<pair<int, pair<bool, handler_callback>>> run_queue)
{
	(run_queue.back()).second.second.run();

	if((run_queue.back()).second.first)
		run_queue.emplace_front(run_queue.back());
	else
		scheduler_is_priority_lookup.erase((run_queue.back()).first);

	run_queue.pop_back();
}

/*
 *Generic initializer
 *
 *Allows for configuration without tasks
 */
scheduler_runslice::scheduler_runslice():
	debug_status(scheduler_runslice_local_errors, debug_status::getMinClassError() + SCHEDULER_RUNSLICE_LOCAL_ERROR_COUNTp - 1),
	scheduler_mode(scheduler_runslice_mode::round_robin),
	scheduler_priority_runs_last(0)
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
 *
 * No guards will be had here... If someone decidces
 * to overide the given values of runslice_mode,
 * the fault is on them
 *
 *@PARAM: The allocator mode
 */
void scheduler_runslice::setAlgorithmMode(scheduler_runslice::scheduler_runslice_mode designated_allocator)
{
	scheduler_mode = designated_allocator;
}

/*
 *Setup a handler with a function without
 *parameters or bound parameters for trigger by event
 *and enqueue it
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameter free or bound function
 *@PARAM: If the handler is of function
 *@PARAM: If the item should re-enter the queue
 *@RETURN: If the function could be registered
 */
bool scheduler_runslice::enqueueRegistration(int identifier, void (*function_pointer)(), bool is_priority, bool persist)
{
	return enqueueSharedParameterFree(identifier, function_pointer, is_priority, persist);
}

/*
 *Setup a handler with a function and
 *parameters for trigger by event and enqueue it
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The funtion to be used
 *@PARAM: The function arguments
 *@PARAM: If the handler is of priority
 *@PARAM: If the item should re-enter the queue
 *@RETURN: If the function could be registered
 */
template <typename... T>
bool scheduler_runslice::enqueueRegistrationParameters(int identifier, void(*function_pointer)(T...), T... args, bool is_priority, bool persist)
{
	return enqueueShared<T...>(identifier, function_pointer, args..., is_priority, persist);
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
	bool
		result;

	result = false;

	auto get_iterator_lookup = scheduler_is_priority_lookup.find(identifier);

	if(get_iterator_lookup != scheduler_is_priority_lookup.end())
	{
		if(get_iterator_lookup->second != is_priority)
		{
			//No checks, this should be handled internally
			if(get_iterator_lookup->second)
			{
				auto get_iterator_queue = find_if(scheduler_priority_queue.begin(), scheduler_priority_queue.end(),
					[identifier](pair<int, pair<bool, handler_callback>> iterator){return iterator.first == identifier;});
				scheduler_normal_queue.emplace_front(pair<int, pair<bool, handler_callback>>(*get_iterator_queue));
				scheduler_priority_queue.erase(get_iterator_queue);
			}
			else
			{
				auto get_iterator_queue = find_if(scheduler_normal_queue.begin(), scheduler_normal_queue.end(),
					[identifier](pair<int, pair<bool, handler_callback>> iterator){return iterator.first == identifier;});
				scheduler_priority_queue.emplace_front(pair<int, pair<bool, handler_callback>>(*get_iterator_queue));
				scheduler_normal_queue.erase(get_iterator_queue);
			}
		}

		get_iterator_lookup->second = is_priority;
		result = true;
	}
	else
		setReturnCode((int)scheduler_runslice_local_errors_lookup::identifier_not_available);

	return result;
}

/*
 *Update a registration to a bound or parameter free function
 *
 *@PARAM: The identifier for use in the lookup
 *@PARAM: The parameter free or bound function
 *@RETURN: If the function could be updated
 */
bool scheduler_runslice::updateRegistration(int identifier, void (*function_pointer)())
{
	return updateSharedParameterFree(identifier, function_pointer);
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
	return updateShared<T...>(identifier, function_pointer, args...);
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
	return updateShared<T...>(identifier, nullptr, args...);
}

/*
 *Remove a registration from the queue
 *
 * Consider compacting?
 *
 *@PARAM: The registration lookup to remove
 *@RETURN: If the registration was removed
 */
bool scheduler_runslice::removeRegistration(int identifier)
{
	return removeRegistrationInline(identifier);
}

/*
 *Run the scheduler
 */
void scheduler_runslice::run()
{
	//#define 1
	bool
		priority_empty,
		normal_empty;
	
	if(!scheduler_is_priority_lookup.empty())
	{
		if(scheduler_mode != scheduler_runslice_mode::priority_only_round_robin)
		{
			if((scheduler_priority_runs_last && !scheduler_priority_queue.empty()) || scheduler_normal_queue.empty())
				runOperationInline(scheduler_priority_queue);
			else
				runOperationInline(scheduler_normal_queue);

			scheduler_priority_runs_last += scheduler_runslice_mode::round_robin == scheduler_mode;
			scheduler_priority_runs_last *= scheduler_priority_runs_last > SCHEDULER_RUNSLICE_ROUND_ROBIN_PRIORITY_CYCLES; 
			scheduler_priority_runs_last += scheduler_runslice_mode::priority_round_robin == scheduler_mode;
			scheduler_priority_runs_last *= scheduler_priority_runs_last > SCHEDULER_RUNSLICE_PRIORITY_ROUND_ROBIN_PRIORITY_CYCLES; 
		}
		else if(scheduler_mode == scheduler_runslice_mode::priority_only_round_robin)
		{
			if(!scheduler_priority_queue.empty())
				runOperationInline(scheduler_priority_queue);
			else
				setReturnCode((int)scheduler_runslice_local_errors_lookup::no_priority_tasks_available);

			scheduler_priority_runs_last = 0;
		}
	}
	else
		setReturnCode((int)scheduler_runslice_local_errors_lookup::no_tasks_available);
}

/*
 *Wipe the scheduler structure
 */
void scheduler_runslice::clean()
{
	scheduler_priority_runs_last = 0;
	scheduler_mode = scheduler_runslice_mode::round_robin;

	while(!scheduler_priority_queue.empty())
		scheduler_priority_queue.pop_front();
	while(!scheduler_normal_queue.empty())
		scheduler_normal_queue.pop_front();

	scheduler_is_priority_lookup.clear();
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void scheduler_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
