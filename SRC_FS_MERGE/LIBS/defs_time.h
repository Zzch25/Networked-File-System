/*
 *(c) Zachary Job
 *12/28/16
 *
 *defs_time.h
 *
 *For time operations, eventually a stop watch
 *functionality will be added for when it is
 *useful
 */

#ifndef DEFS_TIME
#define DEFS_TIME

#include <chrono>
#include <ctime>

using namespace std;

/*
 *Not intended as a parent
 *
 *A time object intended on set then get operations

N O T E: To be expanded

 */
class time_setter_getter
{
	private:

		string time_last_recorded;

	public:

		time_setter_getter();
		void setTime();
		string getTime();
};

/*
 *Not intended as a parent
 *
 *A time object intended on getting current information
 */
class time_now
{
	private:

	public:

		string getTime();
};

//TIME SETTER GETTER/////////////////////////
/////////////////////////////////////////////

time_setter_getter::time_setter_getter()
{
	time_last_recorded = getTime();
}

void time_setter_getter::setTime()
{
	time_t time;
	time = chrono::system_clock::to_time_t(chrono::system_clock::now());

	time_last_recorded = string(ctime(&time));
}

string time_setter_getter::getTime()
{
	return time_last_recorded;
}

//TIME NOW///////////////////////////////////
/////////////////////////////////////////////

string time_now::getTime()
{
	time_t time;
	time = chrono::system_clock::to_time_t(chrono::system_clock::now());

	return string(ctime(&time));
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void time_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
