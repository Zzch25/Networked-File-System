/*
 *(c) Zachary Job
 *
 *defs_handler.h
 *
 * NOTE, no debugging overhead will be had in the
 * callback portion. This may be latter expanded so
 * the include will stay. Callback shall not waste
 * overhead when it is too simple not to mess up.
 * also handling errors spawned before a call is
 * even made within the scheduler should NOT be 
 * encouraged...
 *
 *For the assignment, storage, and management
 *of functions for callback
 */

#ifndef DEFS_HANDLER
#define DEFS_HANDLER

#include <functional>

using namespace std;

/*
 *A structure for the storage and management
 *of functions
 */
class handler_callback
{
	private:
		
		bool
			handler_function_parameter_free;

		void
			*handler_unbound_function_pointer;

		function<void()>
			handler_formatted_function;

	public:
			
		handler_callback(auto bound_or_free_function = nullptr);
		~handler_callback();

		template <typename... T> void setFunction(void(*function_pointer)(T...), T... args);
		void setBoundOrFreeFunction(function<void()> function_pointer);
		template <typename... T> void setParameters(T... args);
		void run();
};

//HANDLER CALLBACK///////////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *
 *Placeholder for generic initialization of the
 *handler allowing for a bound or parameter
 *free function
 *
 *@PARAM: A bound or free function
 */
handler_callback::handler_callback(auto bound_or_free_function):
	handler_formatted_function(bound_or_free_function)
{
	if(handler_formatted_function == nullptr)
		handler_function_parameter_free = true;
	else
		handler_function_parameter_free = false;
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
 *using template specified parameters.
 *Responsibility lays with the user to 
 *use these functions correctly. The use
 *cases are obvious. Errors in calling will
 *blow up.
 *
 *@PARAM: The function pointer to store and assign
 *@PARAM: The templated variadic function arguments
 */
template <typename... T>
void handler_callback::setFunction(void(*function_pointer)(T...), T... args)
{
	//I'm unsure if C++ will allow this in the event of no parameters.
	//From what I read, it won't. Which would be kind to users and
	//a nice loving slap in the face if they don't read my comments!
	//I'll check this in testing. I'm shaky on expansions. It's been a while
	handler_formatted_function = bind(function_pointer, args...);
	handler_unbound_function_pointer = (void*)function_pointer;
	
	handler_function_parameter_free = false;
};

/*
 *Load the function to the callback object
 *with pre-bound or no parameters. This is to
 *eliminate uneeded overhead. Responsibility
 *lays with the user to use these functions
 *correctly. The use cases are obvious. Errors
 *in calling will blow up.
 *
 *@PARAM: The function to store
 */
void handler_callback::setBoundOrFreeFunction(function<void()> function_pointer)
{
	handler_formatted_function = function_pointer;

	handler_function_parameter_free = true;
}

/*
 *Load or update the function parameters for the
 *callback object
 *
 *@PARAM: The templated variadic function arguments
 */
template <typename... T>
void handler_callback::setParameters(T... args)
{
	assert(!handler_function_parameter_free);

	//I'm unsure if C++ will allow this in the event of no parameters.
	//From what I read, it won't. Which would be kind to users and
	//a nice loving slap in the face if they don't read my comments!
	//I'll check this in testing. I'm shaky on expansions. It's been a while
	handler_formatted_function = bind((void(*)(T...))handler_unbound_function_pointer, args...);
};

/*
 *Run the stored callback
 */
void handler_callback::run()
{
	//I don't think this is necesary, but this is more a
	//forgetfull guard
	assert(handler_formatted_function != nullptr);
	
	handler_formatted_function();
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void handler_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
