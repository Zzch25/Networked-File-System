/*
 *(c) Zachary Job
 *
 *defs_network.h
 *
 *Networking objects intended to easily establish
 */

#ifndef DEFS_NETWORK
#define DEFS_NETWORK

#include <iostream>
#include <unistd.h>
#include <memory>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "./defs_debug.h"

#define NETWORK_INSTANCE_SIZE_BUFFERp 256
#define NETWORK_INSTANCE_LOCAL_ERROR_COUNTp 6
#define NETWORK_INSTANCE_CONNECTION_BACKLOGp 5

using namespace std;

/*
 *Not intended to be a parent
 *
 *A instance for communicating to a remote device
 */
class network_instance : public debug_status
{
	private:

		enum class network_local_errors_lookup
		{
			cannot_obtain_server_address = 0,
			cannot_obtain_client_address,
			cannot_open_socket,
			cannot_bind,
			cannot_accept_connection,
			cannot_connect
		};

		const string
			network_local_errors[NETWORK_INSTANCE_LOCAL_ERROR_COUNTp] =
			{
				"Cannot obtain server address information",
				"Cannot obtain client address",
				"Cannot open socket",
				"Cannot bind",
				"Cannot accept connection",
				"Cannot connect"
			};
		
		bool
			network_connection_ready,
			network_connection_is_client,
			network_connection_established;

		char
			network_size_buffer[NETWORK_INSTANCE_LOCAL_ERROR_COUNTp];

		int
			network_socket_descriptor,
			network_established_socket_descriptor,
			network_port_number,
			network_address_size,
			network_bytes_actioned;

		struct hostent
			*network_server_configuration,
			*network_client_configuration;

		struct sockaddr_in
			network_server_address,
			network_client_address;

		inline bool initConnection();

	public:

		network_instance();
		virtual ~network_instance();

		void setParameters(string client_address, int client_port, string server_address, int server_port);

		void acceptConnection();
		void startConnection();
		void terminateConnection();

		void sendToConnection(char *in, int size);
		shared_ptr<char> receiveFromConnection(int *size);
};

//NETWORK INSTANCE///////////////////////////
/////////////////////////////////////////////

/*
 *Initializer, do not allow init of connection yet just because!
 */
network_instance::network_instance():
	debug_status(network_local_errors, debug_status::getMinClassError() + NETWORK_INSTANCE_LOCAL_ERROR_COUNTp - 1),
	network_connection_ready(false),	
	network_connection_is_client(false),	
	network_connection_established(false)
{
}

/*
 *In the event something needs a deinit
 */
network_instance::~network_instance()
{
}

/*
 *Setup the connection on the local machine
 *
 *Do not forget INADDR_ANY
 *
 *@PARAM: The address of the local client
 *@PARAM: The port of the local client
 *@PARAM: The address of the server
 *@PARAM: The port of the server
 */
void network_instance::setParameters(string client_address, int client_port, string server_address, int server_port)
{	
	network_client_configuration = gethostbyname(client_address.c_str());
	network_server_configuration = gethostbyname(server_address.c_str());

	if(network_client_configuration != NULL && network_server_configuration != NULL)
	{
		memset(&network_client_address, 0, sizeof(network_client_address));
		memset(&network_server_address, 0, sizeof(network_server_address));

		network_client_address.sin_family = AF_INET;
		network_client_address.sin_port = htons(client_port);
		network_client_address.sin_addr = *(struct in_addr *)network_client_configuration->h_addr;

		network_server_address.sin_family = AF_INET;
		network_server_address.sin_port = htons(server_port);
		network_server_address.sin_addr = *(struct in_addr *)network_server_configuration->h_addr;
	
		network_connection_ready = initConnection();
	}
	else
		network_connection_ready = false;

	//Possibly the source of an update in debug, logging multiple errors	
	if(network_client_configuration == NULL)
		setReturnCode((int)network_local_errors_lookup::cannot_obtain_client_address);
	else
		setReturnCode((int)network_local_errors_lookup::cannot_obtain_server_address);

}

/*
 *Await connection from a client
 */
void network_instance::acceptConnection()
{
	int
		size;

	assert(network_connection_ready);

	size = sizeof(network_client_address);
	network_connection_established = false;
	network_connection_is_client = false;

	if(bind(network_socket_descriptor, (struct sockaddr *)&network_server_address, sizeof(network_server_address)) >= 0)
	{
		listen(network_socket_descriptor, NETWORK_INSTANCE_CONNECTION_BACKLOGp);
		
		if((network_established_socket_descriptor = accept(network_socket_descriptor, (struct sockaddr *)&network_client_address, &size)) >= 0)
			network_connection_established = true;
		else
			setReturnCode((int)network_local_errors_lookup::cannot_accept_connection);
	}
	else
		setReturnCode((int)network_local_errors_lookup::cannot_bind);
}

/*
 *Attempt to connect to a waiting await 
 */
void network_instance::startConnection()
{
	assert(network_connection_ready && !network_connection_established);

	network_connection_is_client = true;

	if((connect(network_socket_descriptor, (struct sockaddr *)&network_server_address, sizeof(network_server_address))) < 0)
		setReturnCode((int)network_local_errors_lookup::cannot_connect);
	else
		network_connection_established = true;
}

/*
 *End connection
 */
void network_instance::terminateConnection()
{
	assert(network_connection_established);

	shutdown(network_socket_descriptor, SHUT_RDWR);
	close(network_socket_descriptor);
	
	if(!network_connection_is_client)
	{
		shutdown(network_established_socket_descriptor, SHUT_RDWR);
		close(network_established_socket_descriptor);
	}

	network_connection_established = false;
}

/*
 *Send a payload to a connected member
 *
 *@PARAM: The payload to send
 */
void network_instance::sendToConnection(char *in, int size)
{
	int
		network_descriptor;
	
	assert(network_connection_established);

	if(network_connection_is_client)	
		network_descriptor =	network_socket_descriptor;
	else
		network_descriptor = network_established_socket_descriptor;

	sprintf(network_size_buffer, "%d", size);
	write(network_descriptor, network_size_buffer, NETWORK_INSTANCE_SIZE_BUFFERp);
	write(network_descriptor, in, size);
}

/*
 *Receive a payload from a connected member
 *
 *@RETURN: The payload received
 */
shared_ptr<char> network_instance::receiveFromConnection(int *size)
{
	char
		*in;

	int
		network_descriptor;
	
	assert(network_connection_established);

	if(network_connection_is_client)	
		network_descriptor =	network_socket_descriptor;
	else
		network_descriptor = network_established_socket_descriptor;

	read(network_descriptor, network_size_buffer, NETWORK_INSTANCE_SIZE_BUFFERp);
	*size = atoi(network_size_buffer);
	in = new char[*size];
	read(network_descriptor, in, *size);

	shared_ptr<char> result (in);

	return result;
}

/*
 *Attempt to establish parameters on the local machine
 */
bool network_instance::initConnection()
{
	network_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if(network_socket_descriptor < 0)
	{
		setReturnCode((int)network_local_errors_lookup::cannot_open_socket);
		return false;
	}

	return true;
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void network_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
