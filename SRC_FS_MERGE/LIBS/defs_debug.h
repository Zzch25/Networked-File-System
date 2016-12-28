/*
 *(c) Zachary Job
 *12/27/16
 *
 *defs.h
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
#include <chrono>
#include <ctime>
#include <cstring>
#include <cassert>

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
 
 N O T E: MUST MIGRATE FILE OPERATIONS TO FILE DEFS
 
 
 */
class debug_logger
{
	public:

		enum class debug_file_mode_e
		{
			append,
			truncate
		};

	private:

		const string debug_default_log_file = "__logfile";

		mutable ofstream debug_output_file;
		mutable string debug_file_name;
		mutable debug_file_mode_e debug_file_mode;

		string debug_last_time_stamp;

		inline void getTimeStamp();
		inline void getFileDescriptor();
		inline void prependTime(bool isStart);

	public:	

		//gratuitous overrides, remove in post
		debug_logger(const string file_name, const debug_file_mode_e file_mode);
		~debug_logger(); //no virtual, avoid vtable stress... however this may be exapnded on
		void insert_custom(const string custom_string);
		virtual istream &operator>>(istream &input);
		void modifyMode(const debug_file_mode_e file_mode);
		void repurpose(const string file_name);	
		void repurpose(const string file_name, const debug_file_mode_e file_mode);
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
 *@PARAM: Stream to output to
 *@PARAM: Format
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
debug_logger::debug_logger(string file_name, debug_file_mode_e file_mode):
	debug_file_name(file_name),
	debug_file_mode(file_mode)
{
	if(debug_file_name == "")
		debug_file_name.assign(debug_default_log_file);

	getFileDescriptor();
	getTimeStamp();

	prependTime(true);
}

/*
 *Destructor, ensure the descriptor is closed
 */
debug_logger::~debug_logger()
{
	debug_output_file.close();
}

/*
 *Allow insertion of a custom string to the log file
 *
 *@PARAM: The custom string to insert
 */
void debug_logger::insert_custom(const string custom_string)
{
	prependTime(false);
	debug_output_file.write(custom_string.c_str(), custom_string.size());
	debug_output_file.write("\n", 1);
}

/*
 *Overload for input, allow data streams to insert into the output file
 *
 *@PARAM: The input string
 *@PARAM: Format
 *@RETURN: Default
 */
istream &debug_logger::operator>>(istream &input)
{
	char *char_arr_in;
	input >> char_arr_in;

	prependTime(false);
	debug_output_file.write(char_arr_in, strlen(char_arr_in));
	debug_output_file.write("\n", 1);

	return input;
}

/*
 *Change the file output mode and reopen the destructor
 *
 *@PARAM: The mode for file output
 */
void debug_logger::modifyMode(const debug_file_mode_e file_mode)
{
	debug_file_mode = file_mode;

	getFileDescriptor();
}

/*
 *Change the file and maintain the output mode
 *
 *@PARAM: The new file name
 */
void debug_logger::repurpose(const string file_name)
{
	if(file_name == "")
		debug_file_name.assign(debug_default_log_file);
	else
		debug_file_name.assign(file_name);

	getFileDescriptor();
}

/*
 *Repurpose the object to a wholly differing output file
 *
 *@PARAM: The name of the output file
 *@PARAM: The mode of the output file
 */
void debug_logger::repurpose(const string file_name, debug_file_mode_e file_mode)
{
	if(file_name == "")
		debug_file_name.assign(debug_default_log_file);
	else
		debug_file_name.assign(file_name);

	debug_file_mode = file_mode;

	getFileDescriptor();
}

/*
 *Return the last time the log file was modified
 *
 *@RETURN: The last time the log file was modified
 */
string debug_logger::getLastLog()
{
	return debug_last_time_stamp;
}

/*
 *Get the system time and date at time of log
 */
inline void debug_logger::getTimeStamp()
{
	time_t time;
	time = chrono::system_clock::to_time_t(chrono::system_clock::now());

	debug_last_time_stamp = string(ctime(&time));
}

/*
 *Open a file descriptor according to internal parameters
 */
inline void debug_logger::getFileDescriptor()
{
	switch(debug_file_mode)
	{
		case debug_file_mode_e::append:
			debug_output_file.open(debug_file_name, ios::app);
			break;
		case debug_file_mode_e::truncate:
			debug_output_file.open(debug_file_name, ios::trunc);
			break;
		default:
			debug_output_file.open(debug_file_name, ios::app);
			break;
	}
}

inline void debug_logger::prependTime(bool isStart)
{
	if(isStart)
		debug_output_file.write("\nSTART LOGGER::", 1);
	debug_output_file.write(debug_last_time_stamp.c_str(), debug_last_time_stamp.size());
	if(isStart)
		debug_output_file.write("\n\n", 2);
	else	
		debug_output_file.write("::", 2);
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void debug_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
