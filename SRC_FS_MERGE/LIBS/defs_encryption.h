/*
 *(c) Zachary Job
 *
 *defs_encryption.h
 *
 *Both a wrapper for any session libraries for secure connections
 *for modularity and ease of swapping, and a wrapper for offline
 *AES 256 based libraries for the same rationale 
 *
 *NOTE extern styling 
 *
 */

#ifndef DEFS_ENCRYPTION
#define DEFS_ENCRYPTION

#include <iostream>

#include "defs_debug.h"

#ifdef __cplusplus
extern "C"
{
	#include "../EXT_LIBS/libssh2/include/libssh2.h"
	#include "../EXT_LIBS/libssh2/include/libssh2_publickey.h"
	#include "../EXT_LIBS/libssh2/include/libssh2_sftp.h"
}
#endif

#define ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp 2

#define ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp 1

using namespace std;

/*
 *For a secure networking handshake, and to serve
 *as a wrapper so that different libraries can be attached
 */
class encryption_networked : public debug_status
{
	private:

		enum class encryption_local_errors_lookup
		{
			cannot_initialize_libssh2 = 0,
			cannot_initialize_libssh2_session
		};

		const string
			encryption_local_errors[ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp] =
			{
				"Cannot perform libssh2 initialization"
				"Cannot perform libssh2 session initialization"
			};

		mutable bool
			encryption_initialized,
			encryption_session_initialized;
		
		bool
			encryption_session_connected;

		LIBSSH2_SESSION
			*encryption_session;

	public:

		encryption_networked();
		~encryption_networked();
		
};

/*
 *TBD, primarily for use in securing local data with AES 256
 */
class encryption_offline : public debug_status
{
	private:

		enum class encryption_local_errors_lookup
		{
			some_generic_placeholder = 0
		};

		const string
			encryption_local_errors[ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp] =
			{
				"Some generic placeholder"
			};

	public:

		encryption_offline();
		~encryption_offline();
};

//ENCRYPTION NETWORKED///////////////////////
/////////////////////////////////////////////

encryption_networked::encryption_networked():
	encryption_initialized(false),
	encryption_session_initialized(false),
	encryption_session_connected(false),
	debug_status(encryption_local_errors, debug_status::getMinClassError() + ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp - 1)
{
	if(libssh2_init == 0)
	{
		encryption_initialized = true;
		encryption_session = libssh2_session_init();
		
		if(!encryption_session)
		{
			setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_libssh2_session);
			encryption_session_initialized = true;
		}
	}
	else
		setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_libssh2);
}

encryption_networked::~encryption_networked()
{
	if(encryption_initialized)
	{
		if(encryption_session_initialized)
		{
			libssh2_session_disconnect(encryption_session, "");
			libssh2_session_free(encryption_session);
		}
		libssh2_exit();
	}
}

/*
encryption_networked::()
{
	assert(encryption_libssh2_initialized && encryption_session_initialized);


}
*/

//ENCRYPTION OFFLINE/////////////////////////
/////////////////////////////////////////////

encryption_offline::encryption_offline():
	debug_status(encryption_local_errors, debug_status::getMinClassError() + ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp - 1)
{

}

encryption_offline::~encryption_offline()
{

}

/*
encryption_offline::()
{

}
 */

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void encryption_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
