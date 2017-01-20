/*
 *(c) Zachary Job
 *1/13/17
 *
 *defs_concurrency.h
 *
 *The intention is to provide a simple object to provide a generic means
 *to launch and control threads. This object essentially is a thread
 *manager, the intention is to prevent the need for bulky launchers.
 *
 *
 
	NOTE: This is still pre draft!!! Just a quick throw everything on paper
			version


 *
 *
 *Other concurrency methods can be developed here and likely will extend
 *to atomic type operations. This would be useful towards avoiding
 *the need for locks in simple situations. The hope is to truly leverage
 *C/C++11
 */
#ifndef DEFS_CONCURRENCY
#define DEFS_CONCURRENCY

#include <iostream>
#include <utility>
#include <functional>
#include <thread>
#include <string>
#include <atomic>
#include <list>
#include <stack>
#include <vector>
#include <unordered_map>

#include "defs_debug.h"

#define CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp 1
#define CONCURRENCY_MANAGER_SCHEDULED_THREAD_CONSUMPTIONp 0
#define CONCURRENCY_MANAGER_PERMANENT_THREAD_CONSUMPTIONp 1

using namespace std;

/*
 *This aims to provide a thread manager for extremely ease
 *of management including shared memory
 */
class concurrency_manager : public debug_status
{
	private:

		static atomic_flag
			concurrency_critical_section;

		static atomic<short>
			concurrency_permanent_thread_count,
			concurrency_scheduled_thread_count, //for future updates
			concurrency_running_scheduled_tasks;

		bool
			concurrency_primary_launch_ran,
			concurrency_scheduled_exit_set,
			concurrency_permanent_exit_set;

		short
			concurrency_local_thread_count,
			concurrency_cpu_count;

		unordered_map<string, pair<pair<void*, atomic_flag>, short>>
			concurrency_system_parameters;

		struct concurrency_thread_task
		{
			bool *is_exit;
			bool is_join;
			bool is_permanent;
			bool is_reoccuring;
			short task_id;

			vector<void*> task_parameters;
			vector<short*> task_access_count;
			function<void()> task_callback;
			function<void(vector<void*>, bool*)> save_function;
			vector<string> *save_parameters;

			concurrency_manager *master;

			concurrency_thread_task(concurrency_manager *set_master, bool *set_exit, bool set_join, bool set_permanent, bool set_reoccuring, auto function, vector<string> *parameters);	
			//ASSUME SCOPE IS MAINTAINED DUE TO FUNCTION CALL
			~concurrency_thread_task();
		};

		list<struct concurrency_thread_task>
			concurrency_task_queue;

		stack<short>
			concurrency_last_id_death;

		enum class concurrency_local_errors_lookup
		{
			some_generic_placeholder = 0
		};

		const string
			concurrency_local_errors[CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp] =
			{
				"Some generic placeholder"
			};

		inline void setResources();

	public:

		concurrency_manager();
		~concurrency_manager();

		bool enlistCommonVariable(auto *reference, string lookup, atomic_flag **get_lock = nullptr);
		bool removeCommonVariable(string lookup);
		void enqueueTask(bool set_join, bool set_permanent, bool set_reoccuring, auto function, vector<string> *parameters);
		void forwardTasks(vector<short> task_ids);
		void forceTasks(vector<short> task_ids);
		void startTasks();
		void endTasks();
		void startAllTasks();
		void endAllTasks();
		bool runTask(bool ignore_allocation = false);
};

atomic<short> concurrency_manager::concurrency_permanent_thread_count(CONCURRENCY_MANAGER_PERMANENT_THREAD_CONSUMPTIONp);
atomic<short> concurrency_manager::concurrency_scheduled_thread_count(CONCURRENCY_MANAGER_SCHEDULED_THREAD_CONSUMPTIONp);
atomic<short> concurrency_manager::concurrency_running_scheduled_tasks(0);
atomic_flag concurrency_manager::concurrency_critical_section = ATOMIC_FLAG_INIT;

//CONCURRENCY MANAGER////////////////////////
/////////////////////////////////////////////

/*
 *create task with list of common variables
 *
 *@PARAM: Allow generic creation for assignment elsewhere
 *@PARAM: The exit boolean for unilateral exit
 *@PARAM: If the task should be joined with the primary thread 
 *@PARAM: If the task ought to be run as a primary task
 *@PARAM: If the task should enqueue again upon finishing
 *@PARAM: The callback function
 *@PARAM: The variadic string list for assinging the parameters to the function call
 */
concurrency_manager::concurrency_thread_task::concurrency_thread_task(concurrency_manager *set_master, bool *set_exit, bool set_join, bool set_permanent, bool set_reoccuring, auto function, vector<string> *parameters):
	is_exit(set_exit),
	is_join(set_join),
	is_permanent(set_permanent),
	is_reoccuring(set_reoccuring),
	save_function(function),
	save_parameters(parameters),
	master(set_master)
{
	bool
		id_has_death;

	short
		get_is_type,
		top_id;
		
	assert(!master->concurrency_primary_launch_ran || !is_permanent);

	top_id = 0;
	
	get_is_type = (int)(is_permanent);
	concurrency_permanent_thread_count.fetch_add(get_is_type);
	get_is_type = (int)(!is_permanent);
	concurrency_scheduled_thread_count.fetch_add(get_is_type);
	
	master->concurrency_local_thread_count++;
	if(id_has_death = (!master->concurrency_last_id_death.empty()))
		top_id = master->concurrency_last_id_death.top();
	task_id = (short)(id_has_death) * top_id + (short)(!id_has_death) * master->concurrency_local_thread_count;

	for(auto current_element : *parameters)
	{
		auto get_element = master->concurrency_system_parameters.find(current_element);
		assert(get_element != master->concurrency_system_parameters.end());

		(get_element->second.second)++;
		task_access_count.push_back(&(get_element->second.second));
		task_parameters.push_back((void*)&get_element->second.first);
	}

	task_callback = bind(function, task_parameters, is_exit);
}

/*
 *ASSUME SCOPE IS MAINTAINED DUE TO FUNCTION CALL
 *Decrement values, assume linear garbage collection
 */
concurrency_manager::concurrency_thread_task::~concurrency_thread_task()
{
	//Debating on setReturnCode here. But permanent.. means.. until object death do they part
	assert(!is_permanent || *is_exit);
	
	concurrency_running_scheduled_tasks--;

	if(is_reoccuring && !(*is_exit))	
		master->enqueueTask(is_join, is_permanent, is_reoccuring, save_function, save_parameters);
	else
	{
		for(auto current_access_count : task_access_count)
			(*current_access_count)--;
	}
}

/*
 *Standard initilizer with debugging information setup
 */
concurrency_manager::concurrency_manager():
	debug_status(concurrency_local_errors, debug_status::getMinClassError() + CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp - 1),
	concurrency_primary_launch_ran(false),
	concurrency_local_thread_count(0),
	concurrency_scheduled_exit_set(false),
	concurrency_permanent_exit_set(false)
{
	setResources();
}

/*
 *Standard destructor in the event a library requires it
 */
concurrency_manager::~concurrency_manager()
{

}

/*
 *enlist common variable by lookup via reference
 *
 *@PARAM: The data pointer
 *@PARAM: The lookup key for insertion
 *@PARAM: A reference to the lock for the variable
 *@RETURN: If the lookup was able to insert
 */
bool concurrency_manager::enlistCommonVariable(auto *reference, string lookup, atomic_flag **get_lock)
{
	bool
		result;

	pair<void*, atomic_flag>
		inner_parameter;

	pair<pair<void*, atomic_flag>, short>
		outer_parameter;

	inner_parameter.first = (void*)reference;
	inner_parameter.second = atomic_flag(ATOMIC_FLAG_INIT);
	outer_parameter.first = inner_parameter;
	outer_parameter.second = 0;

	if(result = (concurrency_system_parameters.find(lookup) == concurrency_system_parameters.end()))
		concurrency_system_parameters.emplace(lookup, outer_parameter);

	if(result && get_lock != nullptr)
		*get_lock = &concurrency_system_parameters.at(lookup).first.second;

	return result;
}

/*
 *remove common variable by lookup if not in use
 *
 *@PARAM: The element to remove
 *@RETURN: If the remove found and eliminated the element
 */
bool concurrency_manager::removeCommonVariable(string lookup)
{
	bool
		result;

	result = false;

	auto get_element = concurrency_system_parameters.find(lookup);

	if(get_element != concurrency_system_parameters.end())
	{
		if(!(get_element->second.first.second).test_and_set(std::memory_order_acquire))
		{
			if(result = (get_element->second.second == 0))
				concurrency_system_parameters.erase(get_element);

			concurrency_critical_section.clear(std::memory_order_acquire);	
		}
	}

	return result;
}


/*
 *enqueue task with list of common variables
 *
 *@PARAM: If the task should be joined with the primary thread 
 *@PARAM: If the task ought to be run as a primary task
 *@PARAM: If the task should enqueue again upon finishing
 *@PARAM: The callback function
 *@PARAM: The variadic string list for assinging the parameters to the function call
 */
void concurrency_manager::enqueueTask(bool set_join, bool set_permanent, bool set_reoccuring, auto function, vector<string> *parameters)
{
	bool
		*exit_pointer;
	
	assert(!concurrency_primary_launch_ran || !set_permanent);

	if(set_permanent)
		exit_pointer = &concurrency_permanent_exit_set;
	else
		exit_pointer = &concurrency_scheduled_exit_set;

	struct concurrency_thread_task
		to_add(
					this,
					exit_pointer,
					set_join,
					set_permanent,
					set_reoccuring,
					function,
					parameters
				);

	if(!set_permanent)
		concurrency_task_queue.emplace_front(to_add);
	else
		concurrency_task_queue.emplace_back(to_add);
}

/*
 *callback for running specified scheduled tasks IF permanent(s) is running
 *
 *@PARAM: The id's to forward from their location in the queue to running status
 */
void concurrency_manager::forwardTasks(vector<short> task_ids)
{
	assert(concurrency_primary_launch_ran);

	unordered_map<short, bool> //lazy, could use hash... should do that later
		verify_selected;

	for(auto get_element : task_ids)
		verify_selected.emplace(get_element, true);

	concurrency_task_queue.sort(
			[verify_selected](const struct concurrency_thread_task &first, const struct concurrency_thread_task &second)
			{
				return verify_selected.find(first.task_id) == verify_selected.end();
			}
		);
}

/*
 *callback for running specified scheduled tasks IF permanent(s) is running
 *
 *@PARAM: The id's to forward from their location in the queue to running status
 */
void concurrency_manager::forceTasks(vector<short> task_ids)
{
	int
		size;

	assert(concurrency_primary_launch_ran);

	unordered_map<short, bool> //lazy, could use hash... should do that later
		verify_selected;

	for(auto get_element : task_ids)
	{
		verify_selected.emplace(get_element, true);
		++size;
	}

	forwardTasks(task_ids);

	while(size-- > 0)
		runTask(true);
}

/*
 *callback for allowing scheduled tasks IF permanent(s) is running
 *NOTE: This is non blocking, this only sets flags
 */
void concurrency_manager::startTasks()
{
	assert(concurrency_primary_launch_ran);

	concurrency_scheduled_exit_set = false;
}

/*
 *callback for ending scheduled tasks IF permanent(s) is running
 *NOTE: This is non blocking, this only sets flags
 */
void concurrency_manager::endTasks()
{
	assert(concurrency_primary_launch_ran);

	concurrency_scheduled_exit_set = true;
}

/*
 *start primary tasks
 */
void concurrency_manager::startAllTasks()
{
	short
		list_size;

	for(list_size = concurrency_task_queue.size(); list_size > 0; --list_size)
		while(!runTask());

	concurrency_scheduled_exit_set = false;	
	concurrency_permanent_exit_set = false;	
	concurrency_primary_launch_ran = true;
}

/*
 *end primary tasks and all scheduled tasks
 */
void concurrency_manager::endAllTasks()
{

	assert(concurrency_primary_launch_ran);

	concurrency_scheduled_exit_set = true;	
	concurrency_permanent_exit_set = true;	
	concurrency_primary_launch_ran = false;
}

/*
 *callback to run the next task available
 *
 *ABSOLUTE accurracy is not needed. A thread may die
 *releasing an allocation to an otherwise full running
 *pool mid execution
 *
 *@PARAM: Ignore the recommended thread count
 *@RETURN: If a task was ready and ran
 */
bool concurrency_manager::runTask(bool ignore_allocation)
{
	bool
		result;

	short
		available_task_allocation;

	struct concurrency_thread_task
		*poped_task;

	result = false;

	if(!concurrency_critical_section.test_and_set(std::memory_order_acquire) && !concurrency_scheduled_exit_set)
	{
		if(!concurrency_task_queue.empty())
		{
			setResources();
			available_task_allocation = concurrency_cpu_count - concurrency_running_scheduled_tasks.load();

			if(available_task_allocation > 0 || ignore_allocation)
			{
				poped_task = &concurrency_task_queue.back();
				
				thread child(poped_task->task_callback); //verify !& asm difference for reference
				concurrency_running_scheduled_tasks++;
				
				concurrency_local_thread_count--;
				concurrency_last_id_death.push(poped_task->task_id);

				if(!poped_task->is_permanent)
					concurrency_scheduled_thread_count--;

				if(poped_task->is_join)
					child.join();
				else
					child.detach();
				
				concurrency_task_queue.pop_back();
				result = true;
			}
		}
		concurrency_critical_section.clear(std::memory_order_acquire);
	}

	return result;
}

/*
 *Set private variables regarding available resources
 *
 *This likely will expand
 */
void concurrency_manager::setResources()
{
	concurrency_cpu_count = thread::hardware_concurrency() - concurrency_permanent_thread_count.load();
	concurrency_cpu_count += (int)(concurrency_cpu_count <= 0) + (int)(concurrency_cpu_count < 0);
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void concurrency_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
