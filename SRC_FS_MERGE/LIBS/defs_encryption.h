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
 *ALSO the pass key must be stored in a means which helps prevent
 *dumps from easily discovering the affected machines password.
 *
 */

#ifndef DEFS_ENCRYPTION
#define DEFS_ENCRYPTION

#include <iostream>
#include <unordered_map>
#include <string>

#include "defs_debug.h"

#ifdef __cplusplus
extern "C"
{
	/*TEMP until headers configured*/
	#include <openssl/evp.h>
	#include <openssl/aes.h>
	#include <openssl/err.h>
	#include <openssl/rsa.h>
}
#endif

#define ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp 2
#define ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp 1160

#define ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp 1
#define ENCRYPTION_OFFLINE_SECRET_MAXp 125
#define ENCRYPTION_OFFLINE_512BITp 64
#define ENCRYPTION_OFFLINE_256BITp 32
#define ENCRYPTION_OFFLINE_128BITp 16

using namespace std;

/*
 *Initializer for any libraries in need
 */
class encryption_libraries
{
	private:
		static bool
			encryption_is_init;

	public:
		
		encryption_libraries();
		~encryption_libraries();
};

bool encryption_libraries::encryption_is_init = false;

/*
 *For a secure networking handshake, and to serve
 *as a wrapper so that different libraries can be attached
 */
class encryption_networked : public debug_status
{
	private:

		const short
			ENCRYPTION_RSA_LENGTH;

		bool
			encryption_keys_loaded;

		char
			encryption_local_private_key[ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp],
			encryption_local_public_key[ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp],
			encryption_remote_public_key[ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp];

		enum class encryption_local_errors_lookup
		{
			some_generic_placeholder = 0
		};

		const string
			encryption_local_errors[ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp] =
			{
				"Some generic placeholder"
			};

	public:

		encryption_networked(short length_in);
		~encryption_networked();
		
		char *rsaEncryptPublic(char *in);
		char *rsaEncryptPrivate(char *in);
		char *rsaDecryptPublic(char *in);
		char *rsaDecryptPrivate(char *in);

		void loadPublicKeys(char *local_unencrypted_key, char *remote_unencrypted_key);

		void flushDecryptedPrivateKey();

		void rearmPrivateKey(char *local_decrypted_private_key);
};

/*
 *TBD, primarily for use in securing local data with AES 256
 */
class encryption_offline : public debug_status
{
	private:
		
		//This obviously is temporary. The goal is to probably implement a SALT
		//For now this will be called, lazy not salt stuff... enough to force
		//brute force attacks only
		const char
			*ENCRYPTION_STARTUP_KEY = "93853810395438910475691239120591",
			*ENCRYPTION_STARTUP_IV = "1123789831053045";

		char
			encryption_key[ENCRYPTION_OFFLINE_256BITp], //This bubba ought to cycle
			encryption_iv[ENCRYPTION_OFFLINE_128BITp], //This also ought to cycle
			encryption_secret[ENCRYPTION_OFFLINE_SECRET_MAXp],
			encryption_sha512_buffer[ENCRYPTION_OFFLINE_512BITp],
			encryption_sha512[ENCRYPTION_OFFLINE_512BITp];

		unordered_map<string,string>
			encryption_secrets;

		enum class encryption_local_errors_lookup
		{
			some_generic_placeholder = 0
		};

		const string
			encryption_local_errors[ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp] =
			{
				"Some generic placeholder"
			};

		inline char *getSHA(char *payload, int payload_length);

	public:

		encryption_offline();
		~encryption_offline();
	
		void setKeys(char *key, char *iv);
			
		char *AESEncrypt(char *plaintext, int plaintext_length);
		char *AESDecrypt(char *ciphertext, int ciphertext_length);

		char *AESGenerateHello();
		bool AESVerifyHello(char *);

		void addAddressSecret(string address, string secret);
		string getAndDecryptAddressSecret(string address);

		void shatterSecret(string secret, string shatter);
		char *getShatteredSecret();

		short getMaxSecret();
};

//ENCRYPTION LIBRARIES///////////////////////
/////////////////////////////////////////////

encryption_libraries::encryption_libraries()
{
	assert(!encryption_is_init);

	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
}

encryption_libraries::~encryption_libraries()
{
	assert(encryption_is_init);
	encryption_is_init = false;

	EVP_cleanup();
	ERR_free_strings();
}

//ENCRYPTION NETWORKED///////////////////////
/////////////////////////////////////////////

/*
 *Generic initializer
 *Init error class and proceed with init
 *
 *@PARAM: The expected length for the RSA file buffers
 */
encryption_networked::encryption_networked(short length_in = ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp):
	debug_status(encryption_local_errors, debug_status::getMinClassError() + ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp - 1),
	ENCRYPTION_RSA_LENGTH(length_in)
{
}

/*
 *Deinit in the event a library requires something here
 */
encryption_networked::~encryption_networked()
{
}

/*
 *Encrypt a string via a public key
 *
 *@PARAM: The plaintext input string
 *@RETURN: The ciphertext output string
 */
char *encryption_networked::rsaEncryptPublic(char *in)
{
	

	
	
	return nullptr;
}

/*
 *Encrypt a string via a private key
 *
 *@PARAM: The plaintext input string
 *@RETURN: The ciphertext output string
 */
char *encryption_networked::rsaEncryptPrivate(char *in)
{
	

	
	
	return nullptr;
}

/*
 *Decrypt a string via a public key
 *
 *@PARAM: The ciphertext input string
 *@RETURN: The plaintext output string
 */
char *encryption_networked::rsaDecryptPublic(char *in)
{
	

	
	
	return nullptr;
}

/*
 *Decrypt a string via a private key
 *
 *@PARAM: The ciphertext input string
 *@RETURN: The plaintext output string
 */
char *encryption_networked::rsaDecryptPrivate(char *in)
{
	

	
	
	return nullptr;
}

/*
 *Load the public keys in bulk
 *
 *@PARAM: The local public key
 *@PARAM: The remote public key
 */
void encryption_networked::loadPublicKeys(char *local_unencrypted_key, char *remote_unencrypted_key)
{

}

/*
 *Wipe the decrypted private key, try and avoid having it
 *too obvious in memory
 */
void encryption_networked::flushDecryptedPrivateKey()
{

}

/*
 *Rearm the decrypted private key into memory
 *
 *@PARAM: The decrypted private key
 */
void encryption_networked::rearmPrivateKey(char *local_decrypted_private_key)
{

}

//ENCRYPTION OFFLINE/////////////////////////
/////////////////////////////////////////////

/*
 *Initialize the offline encryption including inherited
 *debug data
 */
encryption_offline::encryption_offline():
	debug_status(encryption_local_errors, debug_status::getMinClassError() + ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp - 1)
{
}

/*
 *In the event a library requires a deinit
 */
encryption_offline::~encryption_offline()
{
}

/*
 *Set the keys, note these keys should cycle often
 *
 *@PARAM: The key for encryption
 *@PARAM: The iv for increased safety
 */
void encryption_offline::setKeys(char *key, char *iv)
{

}

/*
 *Encrypt plaintext to ciphertext under AES 256
 *
 *@PARAM: The data to encrypt
 *@PARAM: The length of the data to encrypt
 *@RETURN: The resulting ciphertext
 */
char *encryption_offline::AESEncrypt(char *plaintext, int plaintext_length)
{
	

	
	
	return nullptr;
}

/*
 *Decrypt ciphertext to plaintext under AES 256
 *
 *@PARAM: The data to decrypt
 *@PARAM: The length of the data to decrypt
 *@RETURN: The resulting plaintext
 */
char *encryption_offline::AESDecrypt(char *ciphertext, int ciphertext_length)
{
	

	
	
	return nullptr;
}

/*
 *Generates a hello message where the decryption combined with a
 *SHA hash should verify proper decryption via proper comparison
 *of the hash with the message
 *
 *@RETURN: The resulting verification ciphertext
 */
char *encryption_offline::AESGenerateHello()
{
	

	
	
	return nullptr;
}

/*
 *Verifies a hello cipher RSA and SHA as described above
 *
 *@PARAM: The verification ciphertext
 *@RETURN: If the ciphertext SHA hash matched
 */
bool encryption_offline::AESVerifyHello(char *ciphertext)
{
	bool result;

	result = false;
	
	return result;
}

/*
 *The connection partner shall provide an exhange allowing
 *for password encryption for the connection. It will then
 *be added to the hash map
 *
 *@PARAM: The partner address
 *@PARAM: The provided secret
 */
void encryption_offline::addAddressSecret(string address, string secret)
{

}

/*
 *Obtain the ciphertext for the address from the hash map and
 *using the partner provide decryption
 *
 *@PARAM: The address to lookup
 *@RETURN: The secret for the address
 */
string encryption_offline::getAndDecryptAddressSecret(string address)
{
	string result;

	
	
	return result;
}

/*
 *Attempt to moderately obfusicate the input secret in memory
 *
 *@PARAM: The secret
 *@PARAM: The encryption additional key
 */
void encryption_offline::shatterSecret(string secret, string shatter)
{

}

/*
 *Return the obfusicated secret
 *
 *@RETURN: The secret ciphertext
 */
char *encryption_offline::getShatteredSecret()
{
	

	
	
	return nullptr;
}

/*
 *Return the secret's maximum allowed length
 *
 * inline?
 *
 *@RETURN: The secrets maximum allowed length
 */
short encryption_offline::getMaxSecret()
{
	return ENCRYPTION_OFFLINE_SECRET_MAXp;
}

/*
 *Get a hash for a payload for verification on the other end
 *
 *@PARAM: The payload to hash
 *@PARAM: The payload length
 *@RETURN: The resulting hash
 */
char *encryption_offline::getSHA(char *payload, int payload_length)
{
	

	
	
	return nullptr;
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void encryption_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
