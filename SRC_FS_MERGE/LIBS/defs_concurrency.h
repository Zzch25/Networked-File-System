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
 *Other concurrency methods can be developed here and likely will extend
 *to atomic type operations. This would be useful towards avoiding
 *the need for locks in simple situations. The hope is to truly leverage
 *C/C++11
 */
#ifndef DEFS_CONCURRENCY
#define DEFS_CONCURRENCY

#include <iostream>
#include <functional>
#include <thread>
#include <string>
#include <atomic>
#include <queue>
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

		static atomic<short> 
			concurrency_scheduled_thread_count;

		short
			concurrency_permanent_thread_count;

		short
			concurrency_cpu_count;

		unordered_map<string, pair<void*, atomic_short>>
			concurrency_system_parameters;

		struct concurrency_thread_task
		{
			bool is_join;
			bool is_permanent;
			bool is_exit;
			bool is_reoccuring;
			short task_id;

			vector<void*> task_parameters;
			function<void()> task_callback;

			concurrency_thread_task(bool set_join, bool set_permanent, bool set_reoccuring, auto &function, string parameters...):
				is_join(set_join),
				is_permanent(set_permanent),
				is_exit(false),
				is_reoccuring(set_reoccuring)
			{
				task_id = (int)(!set_permanent) * concurrency_scheduled_thread_count.load() + concurrency_permanent_thread_count;
			
				//setup parameters
				//
				//TODO

				task_callback = bind(function, task_parameters);//, &atomic_flag);
			}

			~concurrency_thread_task()
			{

			}
		};

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
		inline bool requestOccupancy();

	public:

		concurrency_manager();
		~concurrency_manager();

		bool enlistCommonVariable(void *reference, string lookup);
		bool removeCommonVariable(string lookup);
		short enqueueTask(bool set_join, bool set_permanent, bool set_reoccuring, auto &function, string parameters...);
		short runTask();
		short forwardTasks(short task_ids...);
		short endTasks(short task_ids);
		void startPrimaryTasks();
		void endPrimaryTasks();
};

atomic<short> concurrency_manager::concurrency_scheduled_thread_count(CONCURRENCY_MANAGER_SCHEDULED_THREAD_CONSUMPTIONp);

//CONCURRENCY MANAGER////////////////////////
/////////////////////////////////////////////

/*
 *Standard initilizer with debugging information setup
 */
concurrency_manager::concurrency_manager():
	debug_status(concurrency_local_errors, debug_status::getMinClassError() + CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp - 1)
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
 *Set private variables regarding available resources
 *
 *This likely will expand
 */
void concurrency_manager::setResources()
{
	concurrency_cpu_count = thread::hardware_concurrency() - concurrency_permanent_thread_count;

	//Allow some leverage even when the thread count is restricted.
	concurrency_cpu_count += (int)(concurrency_cpu_count <= 0) + (int)(concurrency_cpu_count < 0);
}

/*
 *The intention is to expand this however right now
 *it provides the ability to check if maximum occupancy
 *of the cpu has been reached
 *
 *Should be combined with schedule to yield retry
 *
 *NOT race free but offers additional accuracy in the next step
 *
 *@RETURN: If the CPU can be considered to be hosting the 
 *software at maximum occupancy
 */
bool concurrency_manager::requestOccupancy()
{
	short
		tester;

	tester = concurrency_cpu_count;

	return !concurrency_scheduled_thread_count.compare_exchange_strong(tester, tester);
}

/*
 *enlist common variable by lookup via reference
 *
 *@PARAM:
 *@RETURN
 */
bool concurrency_manager::enlistCommonVariable(void *reference, string lookup)
{

	return 0;
}

/*
 *remove common variable by lookup if not in use
 *
 *@PARAM:
 *@RETURN
 */
bool concurrency_manager::removeCommonVariable(string lookup)
{

	return 0;
}

/*
 *enqueue task with list of common variables
 *
 *@PARAM:
 *@RETURN
 */
short concurrency_manager::enqueueTask(bool set_join, bool set_permanent, bool set_reoccuring, auto &function, string parameters...)
{

	return 0;
}

/*
 *callback for attempt run event
 *
 *@PARAM:
 *@RETURN
 */
short concurrency_manager::runTask()
{

	return 0;
}

/*
 *callback for running specified scheduled tasks IF permanent(s) is running
 *
 *@PARAM:
 *@RETURN
 */
short concurrency_manager::forwardTasks(short task_ids...)
{

	return 0;
}

/*
 *callback for ending specified scheduled tasks IF permanent(s) is running
 *
 *@PARAM:
 *@RETURN
 */
short concurrency_manager::endTasks(short task_ids)
{

	return 0;
}

/*
 *start permanent tasks
 *
 *@PARAM:
 *@RETURN
 */
void concurrency_manager::startPrimaryTasks()
{

}

/*
 *end permanent tasks
 *
 *@PARAM:
 *@RETURN
 */
void concurrency_manager::endPrimaryTasks()
{

}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void concurrency_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
