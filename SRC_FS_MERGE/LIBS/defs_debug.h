/*
 *(c) Zachary Job
 *12/27/16
 *
 *defs_debug.h
 *
 *Debugging and logging interfaces and objects
 *
 *Note:
 *I could have done a switch like I usually do.
 *The goal of this project is not only to make
 *a functional piece of software but also to make
 *myself uncomfortable. I wish to use methods which
 *may not even always be the best way to ensure
 *I am keeping myself to date on a wider field of
 *C++ concepts
 */

#ifndef DEFS__DEBUG
#define DEFS__DEBUG

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

#include "defs_file.h"
#include "defs_time.h"

#define DEBUG_LOG_HEADER "\nSTART LOGGER::"

using namespace std;

/*
 *For use by all classes in need of error checking
 *
 *Provides interfaces to allow for error formatting.
 *The primary intention is for ease of logging and
 *development. Users may also benefit.
 *
 *Maps avoided for additional speed and space savings
 */
class debug_status
{
	private:	

		const string debug_standard_code_to_status[2] = 
		{
			"OK",
			"Allocation failed"
		};

		static const int debug_class_min_error_init;

		const int debug_standard_min_error = 0;
		const int debug_standard_max_error = 1;
		const int debug_class_min_error = 2500;
		const int debug_class_max_error = -1;
		const string debug_class_identifier;
		const string *debug_class_code_to_status;
		const string debug_class_custom_message_ignore;	

		mutable int debug_class_return_code;
		mutable string debug_class_status;
		mutable string debug_class_custom_message;

	protected:
		
		enum class debug_standard_errors
		{
			ok = 0,
			allocation
		};

	public:
		
		debug_status(const string in[], int max_error); 
		virtual ~debug_status() = 0; //So that base pointer deletions reach the child
		virtual ostream &operator<<(ostream &output) final;
		virtual int getReturnCode() const final;
		virtual string getReturnStatus() const final;
		virtual void setReturnCode(const int in) final;
		virtual void setCustomMessage(const string in) final;
		static int getMinClassError(); 
};

const int debug_status::debug_class_min_error_init = 2500;

/*
 *Logging object passable for independent logging
 *
 *Not designed for being a parent
 *
 */
class debug_logger
{

	private:

		const string debug_default_log_file = "__logfile";

		string debug_last_time_stamp;

		file_descriptor output_file;
		time_setter_getter time_output_file;

		inline void getTimeStamp();
		inline void prependTime(bool isStart);

	public:	

		//gratuitous overrides, remove in post
		debug_logger(const string file_name, const file_descriptor::file_mode_e file_mode);
		void log(const string custom_string);
		virtual istream &operator>>(istream &input);
		void setName(const string name);
		void setTruncate(const string name = "");
		void setAppend(const string name = "");	
		string getLastLog();
};

//DEBUG STATUS///////////////////////////////
/////////////////////////////////////////////

/*
 *Optional initializer
 *
 *This allows the child to create its own error messages if needed
 *
 *@PARAM: The const list of possible errors
 */
debug_status::debug_status(const string in[], int max_error) : 
	debug_class_code_to_status(in),
	debug_class_max_error(max_error)
{
}

/*
 *Stream output formatter
 *
 *Enables objects to output their status easily and uniformily.
 *It may seem restrictive, but I see it as great for quick debugging.
 *
 *@PARAM: Stream to output
 *@RETURN: Default
 */
ostream &debug_status::operator<<(ostream &output)
{
		output << debug_class_identifier << '[' << debug_class_return_code << "]::\"" << debug_class_status << "\"";
		if(debug_class_custom_message != debug_class_custom_message_ignore)
			output << "::" << debug_class_custom_message;

		return output;
}

/*
 *Return the objects status code
 *
 *@RETURN: The status code
 */
int debug_status::getReturnCode() const
{
	return debug_class_return_code;
}

/*
 *Return the objects status plain text
 *
 *@RETURN: The status plain text
 */
string debug_status::getReturnStatus() const
{
	return debug_class_status;
}

/*
 *Set the object's status code after asserting the call was valid 
 *
 *@PARAM: The error code to verify
 */
void debug_status::setReturnCode(const int in)
{
	assert(
				(in >= debug_standard_min_error &&
				in <= debug_standard_max_error)
				||
				(in >= debug_class_min_error &&
				in <= debug_class_max_error)
			);

	debug_class_return_code = in;

	if(in > debug_standard_max_error)
		debug_class_status.assign(debug_class_code_to_status[debug_class_return_code - debug_class_min_error]);
	else
		debug_class_status.assign(debug_standard_code_to_status[debug_class_return_code]);
}

/*
 *Set a custom message to accompany the standard error format
 *
 *@PARAM: The custom message
 */
void debug_status::setCustomMessage(const string in)
{
	debug_class_custom_message = in;
}

/*
 *Return the minimum error allowed for the class custom space
 *
 *@PARAM: The minmum class error
 */
int debug_status::getMinClassError()
{
	return debug_class_min_error_init;
}

//DEBUG LOGGER///////////////////////////////
/////////////////////////////////////////////

/*
 *Initializer, set the file name and mode then open a descriptor
 *
 *@PARAM: The file name
 *@PARAM: The file mode
 */
debug_logger::debug_logger(string name, file_descriptor::file_mode_e mode):
	output_file(((name == "")?debug_default_log_file:name), mode)	
{
	prependTime(true);
}

/*
 *Allow insertion of a custom string to the log file
 *
 *@PARAM: The custom string to insert
 */
void debug_logger::log(const string custom_string)
{
	prependTime(false);
	output_file.write_file(custom_string.c_str(), custom_string.size());
	output_file.write_newline();
}

/*
 *Overload for input, allow data streams to insert into the output file
 *
 *@PARAM: The input string
 *@RETURN: Default
 */
istream &debug_logger::operator>>(istream &input)
{
	char *char_arr_in;
	input >> char_arr_in;

	prependTime(false);
	output_file.write_file(char_arr_in, strlen(char_arr_in));
	output_file.write_newline();

	return input;
}

/*
 *Repurpose the object
 *
 *@PARAM: The name of the output file, or empty for the same
 */
void debug_logger::setName(const string name)
{
	assert(name != "");
	
	output_file.repurpose(name);
}

/*
 *Repurpose the object
 *
 *@PARAM: The name of the output file, or empty for the same
 */
void debug_logger::setTruncate(const string name)
{
	if(name == "")
		output_file.modifyMode(file_descriptor::file_mode_e::truncate);
	else
		output_file.repurpose(name, file_descriptor::file_mode_e::truncate);
}

/*
 *Repurpose the object
 *
 *@PARAM: The name of the output file, or empty for the same
 */
void debug_logger::setAppend(const string name)
{
	if(name == "")
		output_file.modifyMode(file_descriptor::file_mode_e::append);
	else
		output_file.repurpose(name, file_descriptor::file_mode_e::append);
}

/*
 *Return the last time the log file was modified
 *
 *@RETURN: The last time the log file was modified
 */
string debug_logger::getLastLog()
{
	return time_output_file.getTime();
}

/*
 *Add a timestamp to the log with the option
 *of inserting a timestamp header
 *
 *@PARAM: If it is a header
 */
inline void debug_logger::prependTime(bool isStart)
{
	time_output_file.setTime();
	string curr_time = time_output_file.getTime();

	if(isStart)
		output_file.write_file(DEBUG_LOG_HEADER, strlen(DEBUG_LOG_HEADER)); //add to defines
	output_file.write_file(curr_time.c_str(), curr_time.size());
	if(isStart)
		output_file.write_file("\n\n", 2);
	else
		output_file.write_file("::", 2);
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void debug_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
