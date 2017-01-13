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
#include <pthread.h>

#include "defs_debug.h"

#define CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp 1

using namespace std;

/*
 *This class
 *
 */
class concurrency_manager : public debug_status
{
	private:

		enum class concurrency_local_errors_lookup
		{
			some_generic_placeholder = 0
		};

		const string
			concurrency_local_errors[CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp] =
			{
				"Some generic placeholder"
			};

	public:

		concurrency_manager();
		~concurrency_manager();
};

concurrency_manager::concurrency_manager():
	debug_status(concurrency_local_errors, debug_status::getMinClassError() + CONCURRENCY_MANAGER_LOCAL_ERROR_COUNTp - 1)
{

}

concurrency_manager::~concurrency_manager()
{

}

/*
 *
 *
 *@PARAM:
 *@RETURN:
 */
/*concurrency_manager::()
{

}
*/

#endif
