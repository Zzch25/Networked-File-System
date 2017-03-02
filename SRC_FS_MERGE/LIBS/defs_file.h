/*
 *(c) Zachary Job
 *12/28/16
 *
 *defs_file.h
 *
 *For wrapping file operations into a simple object
 */

#ifndef DEFS_FILE
#define DEFS_FILE

#include <iostream>
#include <fstream>
#include <memory>
#include <cstring>
#include <cassert>

using namespace std;

/*
 *Not intended to be a parent
 *
 *An object for ease of file operations
 */
class file_descriptor
{
	public:

		enum class file_mode_e
		{
			append,
			truncate,
			in,
			binary
		};

	private:

		mutable bool is_write;
		mutable bool is_string;
	
		mutable string file_name;	
		mutable file_mode_e file_mode;
		mutable fstream file_instance;
		
		inline void getFileDescriptor();

	public:

		
		file_descriptor(string name, file_mode_e mode);
		~file_descriptor();

		void write_file(const char* custom, long int size);
		char* read_file_binary(long int *size);
		string read_file_string(long int *size);
		void write_newline();
		istream &operator>>(istream &input);
		ostream &operator<<(ostream &output);

		void modifyMode(const file_mode_e mode);
		void repurpose(const string name);
		void repurpose(const string name, const file_mode_e mode);
};

//FILE DESCRIPTOR////////////////////////////
/////////////////////////////////////////////

/*
 *Initializer, set the file name and mode then open a descriptor
 *
 *@PARAM: The file name
 *@PARAM: The file mode
 */
file_descriptor::file_descriptor(string name, file_mode_e mode):
	file_name(name),
	file_mode(mode)
{
	assert(file_name != "");

	is_write = file_mode < file_mode_e::in;
	is_string = file_mode == file_mode_e::in;

	getFileDescriptor();
}

/*
 *Destructor, ensure the descriptor is closed
 */
file_descriptor::~file_descriptor()
{
	file_instance.close();
}

/*
 *Allow insertion of a custom binary/string to the file
 *
 *@PARAM: The custom binary/string to insert
 *@PARAM: The size of the binary
 */
void file_descriptor::write_file(const char* custom, long int size)
{
	file_instance.write(custom, size);
}

/*
 *Get the contents of a file as requested
 *
 *@RETURN: The file in string format
 */
char* file_descriptor::read_file_binary(long int *size)
{
	assert(!is_write);

	file_instance.seekg(0, ios::end);
	*size = file_instance.tellg();
	file_instance.seekg(0, ios::beg);

	shared_ptr<char> file_data(new char[*size]);
	file_instance.read(&(file_data.get()[0]), (int)*size);

	return file_data.get();
}

/*
 *Get the contents of a file as requested
 *
 *@RETURN: The file in binary format
 */
string file_descriptor::read_file_string(long int *size)
{
	assert(is_write);

	file_instance.seekg(0, ios::end);
	*size = file_instance.tellg();
	file_instance.seekg(0, ios::beg);

	shared_ptr<char> file_data(new char[*size]);
	file_instance.read(&(file_data.get()[0]), (int)*size);

	return string(file_data.get());
}

/*
 *Insert a newline when in write string mode
 *
 *@PARAM: The custom string to insert
 */
void file_descriptor::write_newline()
{
	assert(is_string && is_write);

	file_instance.write("\n", 1);
}

/*
 *Overload for input, allow data streams to insert into the output file
 *
 *@PARAM: The input string
 *@RETURN: Default
 */
istream &file_descriptor::operator>>(istream &input)
{
	char *char_arr_in;
	input >> char_arr_in;

	file_instance.write(char_arr_in, strlen(char_arr_in));

	return input;
}

/*
 *Stream output from file
 *
 *@PARAM: Stream to output
 *@RETURN: Default
 */
ostream &file_descriptor::operator<<(ostream &output)
{
	int size;
	
	file_instance.seekg(0, ios::end);
	size = file_instance.tellg();
	file_instance.seekg(0, ios::beg);

	shared_ptr<char> file_data(new char[size]);
	file_instance.read(&(file_data.get()[0]), size);

	if(is_write)	
		output << string(file_data.get());
	else
		output << file_data.get();

	return output;
}

/*
 *Change the file output mode and reopen the destructor
 *
 *@PARAM: The mode for file output
 */
void file_descriptor::modifyMode(const file_mode_e mode)
{
	file_mode = mode;

	getFileDescriptor();
}

/*
 *Change the file and maintain the output mode
 *
 *@PARAM: The new file name
 */
void file_descriptor::repurpose(const string name)
{
	assert(file_name != "");
	
	file_name.assign(name);

	getFileDescriptor();
}

/*
 *Repurpose the object to a wholly differing output file
 *
 *@PARAM: The name of the output file
 *@PARAM: The mode of the output file
 */
void file_descriptor::repurpose(const string name, file_mode_e mode)
{
	assert(file_name != "");
	
	file_name.assign(name);
	file_mode = mode;

	getFileDescriptor();
}

/*
 *Open a file descriptor according to internal parameters
 */
inline void file_descriptor::getFileDescriptor()
{
	switch(file_mode)
	{
		case file_mode_e::append:
			file_instance.open(file_name, ios::out | ios::app);
			break;
		case file_mode_e::truncate:
			file_instance.open(file_name, ios::out | ios::trunc);
			break;
		case file_mode_e::in:
			file_instance.open(file_name, ios::in);
			break;
		case file_mode_e::binary:
			file_instance.open(file_name, ios::in | ios::binary);
			break;
		default:
			file_instance.open(file_name, ios::out | ios::app);
			break;
	}
	
	is_write = file_mode < file_mode_e::in;
	is_string = file_mode == file_mode_e::in;
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void file_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
