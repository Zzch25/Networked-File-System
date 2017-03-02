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
#include <cstring>

#include "defs_debug.h"

extern "C"
{
	/*TEMP until headers configured*/
	#include <openssl/evp.h>
	#include <openssl/aes.h>
	#include <openssl/err.h>
	#include <openssl/rsa.h>
	#include <openssl/bio.h>
	#include <openssl/sha.h>
	#include <openssl/pem.h>
}

#define ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp 4
#define ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp 4096//1160

#define ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp 15
#define ENCRYPTION_OFFLINE_SECRET_MAXp 125
#define ENCRYPTION_OFFLINE_512BITp 64
#define ENCRYPTION_OFFLINE_256BITp 32
#define ENCRYPTION_OFFLINE_128BITp 16
#define ENCRYPTION_OFFLINE_TAG_DELIMETERp "\n\n"

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

		const int
			ENCRYPTION_PADDING_MODE;

		mutable bool
			encryption_remote_public_key_loaded;

		bool
			encryption_local_private_key_loaded;

		char
			encryption_local_private_key[ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp],
			encryption_remote_public_key[ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp];

		RSA
			*encryption_public_RSA,
			*encryption_private_RSA;

		BIO
			*encryption_public_keyBIO,
			*encryption_private_keyBIO;

		enum class encryption_local_errors_lookup
		{
			cannot_init_private_keyBIO = 0,
			cannot_init_private_RSA,
			cannot_init_public_keyBIO,
			cannot_init_public_RSA
		};

		const string
			encryption_local_errors[ENCRYPTION_NETWORKED_LOCAL_ERROR_COUNTp] =
			{
				"Cannot init private keyBIO",
				"Cannot init private RSA",
				"Cannot init public keyBIO",
				"Cannot init public RSA"
			};

		inline void initRSA();
		inline void cleanRSA(bool privateOnly);

	public:

		encryption_networked(short length_in);
		~encryption_networked();
		
		char *rsaEncryptPublic(char *in, int *size);
		char *rsaEncryptPrivate(char *in, int *size);
		char *rsaDecryptPublic(char *in, int *size);
		char *rsaDecryptPrivate(char *in, int *size);

		void loadPublicKeys(char *local_unkey, char *remote_unencrypted_key);

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
			*ENCRYPTION_STARTUP_IV = "1123789831053045",
			*ENCRYPTION_STARTUP_ADDRESS= "_default";

		char
			encryption_key_buffer[ENCRYPTION_OFFLINE_256BITp], //This bubba ought to cycle
			encryption_iv_buffer[ENCRYPTION_OFFLINE_128BITp], //This also ought to cycle
			encryption_secret[ENCRYPTION_OFFLINE_SECRET_MAXp],
			encryption_sha512[ENCRYPTION_OFFLINE_512BITp];

		unordered_map<string,string>
			encryption_secrets,
			encryption_keys,
			encryption_ivs;

		enum class encryption_local_errors_lookup
		{
			cannot_initialize_context = 0,
			cannot_initialize_encryption,
			cannot_initialize_decryption,
			cannot_set_iv_length,
			cannot_initialize_key_and_iv,
			cannot_append_additional_phrase,
			cannot_provide_plaintext,
			cannot_provide_ciphertext,
			cannot_finalize_encryption,
			cannot_finalize_decryption,
			cannot_obtain_tag,
			cannot_set_tag,
			cannot_verify_decryption,
			improper_ciphertext_format,
			cannot_get_SHA
		};

		const string
			encryption_local_errors[ENCRYPTION_OFFLINE_LOCAL_ERROR_COUNTp] =
			{
				"Cannot initialize context",
				"Cannot initialize encryption",
				"Cannot initialize decryption",
				"Cannot set iv length",
				"Cannot initialize key and iv",
				"Cannot append additional phrase",
				"Cannot provide plaintext",
				"Cannot provide ciphertext",
				"Cannot finalize encryption",
				"Cannot finalize decryption",
				"Cannot obtain tag",
				"Cannot set tag",
				"Cannot verify decryption",
				"Improper ciphertext format",
				"Cannot get SHA"
			};
		
		inline void insertTag(char *in, int *size, char *tag);
		inline char *getTagOffsetAndSplice(char *in, int size);

	public:

		encryption_offline();
		~encryption_offline();
	
		void setKeys(char *key, char *iv, string address);
		void wipeKeys(string address);
			
		char *AESEncrypt(char *plaintext, int plaintext_length, int *ciphertext_length, string address);
		char *AESDecrypt(char *ciphertext, int ciphertext_length, int *plaintext_length, string address);

		void addAddressSecret(string address, string secret);
		void wipeAddressSecret(string address);

		short getMaxSecret();

		string base64Encode(char *in, int *length);
		char *base64Decode(char *in, int *length);
		
		char *getSHA(char *payload, int payload_length);
		bool compareSHA(char *message, int message_size, char *SHA_text, int SHA_size);

		void setDefault(string secret);
		void wipeDefault();

		char *AESDecryptDefault(char *ciphertext, int ciphertext_length, int *plaintext_length);
		char *AESEncryptDefault(char *plaintext, int plaintext_length, int *ciphertext_length);
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
	ENCRYPTION_RSA_LENGTH(length_in),
	ENCRYPTION_PADDING_MODE(RSA_PKCS1_PADDING),
	encryption_local_private_key_loaded(false),
	encryption_remote_public_key_loaded(false),
	encryption_public_RSA(NULL), //C macro expected
	encryption_private_RSA(NULL),
	encryption_public_keyBIO(NULL),
	encryption_private_keyBIO(NULL)
{
	assert(length_in <= ENCRYPTION_NETWORKED_RSA_STOCK_LENGTHp);
}

/*
 *Deinit in the event a library requires something here
 */
encryption_networked::~encryption_networked()
{
	bool
		mode;

	if(!(mode = !encryption_remote_public_key_loaded) || encryption_local_private_key_loaded)
		cleanRSA(mode);
}

/*
 *Encrypt a string via a public key
 *
 *@PARAM: The plaintext input string
 *@PARAM: Optional text size for retrieval and input size if number is greater than 0
 *@RETURN: The ciphertext output string
 */
char *encryption_networked::rsaEncryptPublic(char *in, int *size = nullptr)
{
	int
		text_length;

	char
		*result = nullptr;

	assert(encryption_remote_public_key_loaded && encryption_public_RSA != NULL);

	if(size != nullptr && *size > 0)
		text_length = *size;
	else
		text_length = strlen(in);

	result = new char[RSA_size(encryption_public_RSA)];
	text_length = RSA_public_encrypt(text_length,(unsigned char *)in,(unsigned char *)result,encryption_public_RSA,ENCRYPTION_PADDING_MODE);

	if(size != nullptr)
		*size = text_length;
	
	return result;
}

/*
 *Encrypt a string via a private key
 *
 *@PARAM: The plaintext input string
 *@PARAM: Optional text size for retrieval and input size if number is greater than 0
 *@RETURN: The ciphertext output string
 */
char *encryption_networked::rsaEncryptPrivate(char *in, int *size = nullptr)
{
	int
		text_length;

	char
		*result = nullptr;
	
	assert(encryption_local_private_key_loaded && encryption_private_RSA != NULL);

	if(size != nullptr && *size > 0)
		text_length = *size;
	else
		text_length = strlen(in);

	result = new char[RSA_size(encryption_public_RSA)];
	text_length = RSA_private_encrypt(text_length,(unsigned char *)in,(unsigned char *)result,encryption_public_RSA,ENCRYPTION_PADDING_MODE);

	if(size != nullptr)
		*size = text_length;
	
	return result;
}

/*
 *Decrypt a string via a public key
 *
 *@PARAM: The ciphertext input string
 *@PARAM: Optional text size for retrieval and input size if number is greater than 0
 *@RETURN: The plaintext output string
 */
char *encryption_networked::rsaDecryptPublic(char *in, int *size = nullptr)
{
	int
		text_length;

	char
		*result = nullptr;
	
	assert(encryption_remote_public_key_loaded && encryption_public_RSA != NULL);

	if(size != nullptr && *size > 0)
		text_length = *size;
	else
		text_length = strlen(in);

	result = new char[RSA_size(encryption_public_RSA)];
	text_length = RSA_public_decrypt(text_length,(unsigned char *)in,(unsigned char *)result,encryption_public_RSA,ENCRYPTION_PADDING_MODE);

	if(size != nullptr)
		*size = text_length;
	
	return result;
}

/*
 *Decrypt a string via a private key
 *
 *@PARAM: The ciphertext input string
 *@PARAM: Optional text size for retrieval and input size if number is greater than 0
 *@RETURN: The plaintext output string
 */
char *encryption_networked::rsaDecryptPrivate(char *in, int *size = nullptr)
{
	int
		text_length;

	char
		*result = nullptr;
	
	assert(encryption_local_private_key_loaded && encryption_private_RSA != NULL);

	if(size != nullptr && *size > 0)
		text_length = *size;
	else
		text_length = strlen(in);

	result = new char[RSA_size(encryption_public_RSA)];
	text_length = RSA_private_decrypt(text_length,(unsigned char *)in,(unsigned char *)result,encryption_public_RSA,ENCRYPTION_PADDING_MODE);

	if(size != nullptr)
		*size = text_length;
	
	return result;
}

/*
 *Load the public keys in bulk
 *
 *@PARAM: The local public key
 *@PARAM: The remote public key
 */
void encryption_networked::loadPublicKeys(char *local_unencrypted_private_key, char *remote_unencrypted_public_key)
{
	if(local_unencrypted_private_key != nullptr)
		memcpy(encryption_local_private_key, local_unencrypted_private_key, ENCRYPTION_RSA_LENGTH);
	else
		encryption_local_private_key_loaded = false;

	if(remote_unencrypted_public_key != nullptr)
		memcpy(encryption_remote_public_key, remote_unencrypted_public_key, ENCRYPTION_RSA_LENGTH);
	else
		encryption_remote_public_key_loaded = false;
}

/*
 *Wipe the decrypted private key, try and avoid having it
 *too obvious in memory
 */
void encryption_networked::flushDecryptedPrivateKey()
{
	char
		zero;

	zero = 0;

	memcpy(encryption_local_private_key, &zero, ENCRYPTION_RSA_LENGTH);
	cleanRSA(true);
	encryption_local_private_key_loaded = false;
}

/*
 *Rearm the decrypted private key into memory
 *
 *@PARAM: The decrypted private key
 */
void encryption_networked::rearmPrivateKey(char *local_decrypted_private_key)
{
	if(local_decrypted_private_key != nullptr)
		memcpy(encryption_local_private_key, local_decrypted_private_key, ENCRYPTION_RSA_LENGTH);
	else
		encryption_local_private_key_loaded = false;
}

/*
 *Initialize RSA structure intended for encryption
 */
void encryption_networked::initRSA()
{
	assert(encryption_local_private_key_loaded || encryption_remote_public_key_loaded);

	if(encryption_local_private_key_loaded && encryption_private_RSA == NULL)
	{
		encryption_private_keyBIO = BIO_new_mem_buf(encryption_local_private_key, -1);
		if(encryption_private_keyBIO != NULL)
		{
			//Check if same pointer returned to lvalue as with the double pointer
			if((encryption_private_RSA = PEM_read_bio_RSAPrivateKey(encryption_private_keyBIO, &encryption_private_RSA, NULL, NULL)) == NULL)
			{
				setReturnCode((int)encryption_local_errors_lookup::cannot_init_private_RSA);
				BIO_free(encryption_private_keyBIO);
				encryption_private_keyBIO = NULL;
			}
		}
		else
			setReturnCode((int)encryption_local_errors_lookup::cannot_init_private_keyBIO);
	}

	if(encryption_remote_public_key_loaded && encryption_public_RSA == NULL)
	{
		encryption_public_keyBIO = BIO_new_mem_buf(encryption_remote_public_key, -1);
		if(encryption_public_keyBIO != NULL)
		{
			//Check if same pointer returned to lvalue as with the double pointer
			if((encryption_public_RSA = PEM_read_bio_RSA_PUBKEY(encryption_public_keyBIO, &encryption_public_RSA, NULL, NULL)) == NULL)
			{
				setReturnCode((int)encryption_local_errors_lookup::cannot_init_public_RSA);
				BIO_free(encryption_public_keyBIO);
				encryption_public_keyBIO = NULL;
			}
		}
		else
			setReturnCode((int)encryption_local_errors_lookup::cannot_init_public_keyBIO);
	}	
}

/*
 *De-initialize RSA structure intended for encryption
 *
 *@PARAM: If the public clean should be ignored
 */
void encryption_networked::cleanRSA(bool privateOnly = false)
{
	if(!privateOnly)
	{
		assert(encryption_remote_public_key_loaded);

		if(encryption_public_RSA != NULL) //C macro expected
		{
			RSA_free(encryption_public_RSA);
			encryption_public_RSA = NULL;
		}
		if(encryption_public_keyBIO != NULL)
		{
			BIO_free(encryption_public_keyBIO);
			encryption_public_keyBIO = NULL;
		}
	}
	else
		assert(encryption_remote_public_key_loaded);

	if(encryption_private_RSA != NULL)
	{
		RSA_free(encryption_private_RSA);
		encryption_private_RSA = NULL;
	}
	if(encryption_private_keyBIO != NULL)
	{
		BIO_free(encryption_private_keyBIO);
		encryption_private_keyBIO = NULL;
	}
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
 *@PARAM: The corresponding address
 */
void encryption_offline::setKeys(char *key, char *iv, string address)
{
	assert(address != "" && key != "" && iv != "");

	encryption_keys.emplace(address, string(key));
	encryption_ivs.emplace(address, string(iv));
}

/*
 *Set the keys, note these keys should cycle often
 *
 *@PARAM: The corresponding address
 */
void encryption_offline::wipeKeys(string address)
{
	encryption_keys.erase(address);
	encryption_ivs.erase(address);
}


/*
 *Encrypt plaintext to ciphertext under AES 256
 *
 *@PARAM: The data to encrypt
 *@PARAM: The length of the data to encrypt
 *@PARAM: The length of the encrypted data
 *@PARAM: The lookup to get the corresponding secret
 *@RETURN: The resulting ciphertext
 */
char *encryption_offline::AESEncrypt(char *plaintext, int plaintext_length, int *ciphertext_length, string address)
{
	int
		length = 0;

	char
		*ciphertext,
		*tag;

	EVP_CIPHER_CTX
		*ctx = NULL;

	assert(encryption_secrets.find(address) != encryption_secrets.end() && encryption_keys.find(address) != encryption_keys.end() && encryption_ivs.find(address) != encryption_ivs.end());

	ciphertext = NULL;
	*ciphertext_length = 0;
	
	if(!(ctx = EVP_CIPHER_CTX_new()))
		setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_context);

	if(EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1)
		setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_encryption);

	if(EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, ENCRYPTION_OFFLINE_128BITp, NULL) != 1)
		setReturnCode((int)encryption_local_errors_lookup::cannot_set_iv_length);

	if(EVP_EncryptInit_ex(ctx, NULL, NULL, (unsigned char *)(encryption_keys[address]).c_str(), (unsigned char *)(encryption_ivs[address]).c_str()) != 1)
		setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_key_and_iv);

	if(address != "")
	{
		if(EVP_EncryptUpdate(ctx, NULL, &length, (unsigned char *)(encryption_secrets[address]).c_str(), (encryption_secrets[address]).size()) != 1)
			setReturnCode((int)encryption_local_errors_lookup::cannot_append_additional_phrase);
	}

	if(plaintext != NULL)
	{
		if(EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &length, (unsigned char *)plaintext, plaintext_length) != 1)
			setReturnCode((int)encryption_local_errors_lookup::cannot_provide_plaintext);

		*ciphertext_length = length;
	}

	if(EVP_EncryptFinal_ex(ctx, (unsigned char *)(ciphertext + length), &length) != 1)
		setReturnCode((int)encryption_local_errors_lookup::cannot_finalize_encryption);
	*ciphertext_length += length;

	if(EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, ENCRYPTION_OFFLINE_128BITp, tag) != 1)
		setReturnCode((int)encryption_local_errors_lookup::cannot_obtain_tag);

	EVP_CIPHER_CTX_free(ctx);

	insertTag(ciphertext, ciphertext_length, tag);

	return ciphertext;
}

/*
 *Decrypt ciphertext to plaintext under AES 256
 *
 *@PARAM: The data to decrypt
 *@PARAM: The length of the data to decrypt
 *@PARAM: The lookup to get the corresponding secret
 *@PARAM: The secret lookup
 *@RETURN: The resulting plaintext
 */
char *encryption_offline::AESDecrypt(char *ciphertext, int ciphertext_length, int *plaintext_length, string address)
{
	int 
		length = 0,
		status;

	char
		*tag,
		*plaintext;

	EVP_CIPHER_CTX
		*ctx = NULL;

	assert(encryption_secrets.find(address) != encryption_secrets.end() && encryption_keys.find(address) != encryption_keys.end() && encryption_ivs.find(address) != encryption_ivs.end());
	
	plaintext = NULL;
	*plaintext_length = 0;

	if(!(ctx = EVP_CIPHER_CTX_new()))
		setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_context);
	
	if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
	  setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_decryption);

	if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, ENCRYPTION_OFFLINE_128BITp, NULL))
	  setReturnCode((int)encryption_local_errors_lookup::cannot_set_iv_length);

	if(!EVP_DecryptInit_ex(ctx, NULL, NULL, (unsigned char *)(encryption_keys[address]).c_str(), (unsigned char *)(encryption_ivs[address]).c_str()));
		setReturnCode((int)encryption_local_errors_lookup::cannot_initialize_key_and_iv);

	if(address != "")
	{
	  if(!EVP_DecryptUpdate(ctx, NULL, &length, (unsigned char *)(encryption_secrets[address]).c_str(), (encryption_secrets[address]).size()))
			setReturnCode((int)encryption_local_errors_lookup::cannot_append_additional_phrase);
	}

	if(ciphertext)
	{
	  if(!EVP_DecryptUpdate(ctx, (unsigned char *)plaintext, &length, (unsigned char *)ciphertext, ciphertext_length))
			setReturnCode((int)encryption_local_errors_lookup::cannot_provide_ciphertext);

	  *plaintext_length = length;
	}

	tag = getTagOffsetAndSplice(ciphertext, ciphertext_length);

	if(!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, ENCRYPTION_OFFLINE_128BITp, tag))
	  setReturnCode((int)encryption_local_errors_lookup::cannot_set_tag);

	status = EVP_DecryptFinal_ex(ctx, (unsigned char *)(plaintext + length), &length);

	EVP_CIPHER_CTX_free(ctx);

	if(status > 0)
	  *plaintext_length += length;
	else
		setReturnCode((int)encryption_local_errors_lookup::cannot_verify_decryption);

	return plaintext;
}
 
/*
 *The connection partner shall provide an exhange allowing
 *for password encryption for the connection. It will then
 *be added to the hash map

	TOTALLY SECURE ;)

 *
 *@PARAM: The partner address
 *@PARAM: The provided secret
 */
void encryption_offline::addAddressSecret(string address, string secret)
{
	assert(address != "" && secret != "");

	encryption_secrets.emplace(address, secret);	
}

/*
 *Remove a secret by address

	ALSO VERY VERY SECURE ;)

 *
 *@PARAM: The address containing the value to remove
 */
void encryption_offline::wipeAddressSecret(string address)
{
	encryption_secrets.erase(address);
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
 *Encode data to base64
 *
 *@PARAM: The input data
 *@PARAM: The input length, and output storage
 *@RETURN: The resulting base64 string
 */
string encryption_offline::base64Encode(char *in, int *length)
{
	string
		result;

	char
		*c_string;

	BIO
		*buff,
		*b64f;

	BUF_MEM
		*ptr;

	b64f = BIO_new(BIO_f_base64());
	buff = BIO_new(BIO_s_mem());
	buff = BIO_push(b64f, buff);

	BIO_set_flags(buff, BIO_FLAGS_BASE64_NO_NL);
	BIO_set_close(buff, BIO_CLOSE);
	BIO_write(buff, in, *length);
	BIO_flush(buff);

	BIO_get_mem_ptr(buff, &ptr);
	*length = ptr->length;
	c_string = new char[*length + 1];
	memcpy(c_string, ptr->data, *length);
	c_string[*length] = 0;

	BIO_free_all(buff);
	result.assign(c_string);
	delete c_string;

	return result;
}

/*
 *Decode data from base64
 *
 *@PARAM: The encoded string
 *@PARAM: Optional string length
 *@RETURN: The decoded c string
 */
char *encryption_offline::base64Decode(char *in, int *length)
{
	int
		old_length,
		local_length;

	char
		*result;

	BIO
		*buff,
		*b64f;

	old_length = local_length = strlen(in);

	b64f = BIO_new(BIO_f_base64());
	buff = BIO_new_mem_buf((void *)in, local_length);
	buff = BIO_push(b64f, buff);
	result = new char[local_length];

	BIO_set_flags(buff, BIO_FLAGS_BASE64_NO_NL);
	BIO_set_close(buff, BIO_CLOSE);
	local_length = BIO_read(buff, result, local_length);
	result = new (result) char[local_length - old_length + 1];
	result[local_length] = 0;

	if(length != NULL)
		*length = local_length;

	BIO_free_all(buff);
	
	return result;
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
	char
		*result;
		
	result = new char[ENCRYPTION_OFFLINE_512BITp];

	//memcheckingggggg everywhere neeed

	if(SHA512((unsigned char *)payload, payload_length, (unsigned char *)encryption_sha512) == 0)
		setReturnCode((int)encryption_local_errors_lookup::cannot_get_SHA);
	else
		memcpy(result, encryption_sha512, ENCRYPTION_OFFLINE_512BITp);
		
	return result;
}

/*
 *Verifies a hello cipher RSA and SHA as described above
 *
 *@PARAM: The message
 *@PARAM: The message length
 *@PARAM: The SHA comparator
 *@PARAM: The SHA comparator length
 *@RETURN: If the ciphertext SHA hash matched
 */
bool encryption_offline::compareSHA(char *message, int message_size, char *SHA_text, int SHA_size)
{
	char
		*message_SHA;

	message_SHA = getSHA(message, message_size);
	return strncmp(SHA_text, message_SHA, ENCRYPTION_OFFLINE_512BITp) == 0;
}

/*
 *Enable a default lookup location for default encryption key and iv
 *
 *@PARAM: The secret to set to the default encryption
 */
void encryption_offline::setDefault(string secret)
{
	addAddressSecret(string(ENCRYPTION_STARTUP_ADDRESS), secret);
	setKeys((char *)ENCRYPTION_STARTUP_KEY, (char *)ENCRYPTION_STARTUP_IV, ENCRYPTION_STARTUP_ADDRESS);
}

/*
 *Destroy the default lookup parameters 
 */
void encryption_offline::wipeDefault()
{
	wipeKeys(ENCRYPTION_STARTUP_ADDRESS);
	wipeAddressSecret(ENCRYPTION_STARTUP_ADDRESS);
}

/*
 *Wrap encryption of ciphertext to plaintext under AES 256 using startup parameters
 *
 *@PARAM: The data to decrypt
 *@PARAM: The length of the data to decrypt
 *@PARAM: The lookup to get the corresponding secret
 *@RETURN: The resulting plaintext
 */
char *encryption_offline::AESDecryptDefault(char *ciphertext, int ciphertext_length, int *plaintext_length)
{
	return AESEncrypt(ciphertext, ciphertext_length, plaintext_length, ENCRYPTION_STARTUP_ADDRESS);	
}

/*
 *Wrap for encryption of plaintext to ciphertext under AES 256 using startup parameters
 *
 *@PARAM: The data to encrypt
 *@PARAM: The length of the data to encrypt
 *@PARAM: The length of the encrypted data
 *@RETURN: The resulting ciphertext
 */
char *encryption_offline::AESEncryptDefault(char *plaintext, int plaintext_length, int *ciphertext_length)
{
	return AESDecrypt(plaintext, plaintext_length, ciphertext_length, ENCRYPTION_STARTUP_ADDRESS);	
}

/*
 *Append the tag to the inserted 
 *
 *@PARAM: The string to append to
 *@PARAM: The end position of the string to append to
 *@PARAM: The tag to append
 */
void encryption_offline::insertTag(char *in, int *size, char *tag)
{
	int
		tag_length,
		delimeter_length;

	tag_length = tag_length;
	delimeter_length = strlen(ENCRYPTION_OFFLINE_TAG_DELIMETERp);

	in = new (in) char[tag_length + delimeter_length];

	memcpy(&(in[*size]), ENCRYPTION_OFFLINE_TAG_DELIMETERp, delimeter_length);
	*size += delimeter_length;
	memcpy(&(in[*size]), tag, tag_length);
	*size += tag_length;

	in[*size] = 0;
}

/*
 *Splice the memory with a null terminator and return the tag offset pointer
 *
 *@PARAM: The ciphertext with tag to split
 *@PARAM: The end position of the input
 *@RETURN: The tag offset pointer
 */
char *encryption_offline::getTagOffsetAndSplice(char *in, int size)
{
	bool
		char_match;

	int
		delimeter_length,
		i,
		original_size;

	char
		*result;

	original_size = size;
	delimeter_length = strlen(ENCRYPTION_OFFLINE_TAG_DELIMETERp);

	i = delimeter_length - 1;
	result = NULL;

	while(i != -1 && --size >= -1 + delimeter_length)
	{
		char_match = ENCRYPTION_OFFLINE_TAG_DELIMETERp[i] == in[size];
		i = i * (int)char_match + delimeter_length * (int)!char_match;
		--i;
	}

	if(original_size != size && i == -1)
		result = &(in[size + delimeter_length]);
	else
		setReturnCode((int)encryption_local_errors_lookup::improper_ciphertext_format);

	return result;
}

//QA/////////////////////////////////////////
/////////////////////////////////////////////

void encryption_test_cases()
{
	cout << "Nothing to do here yet";
}

#endif
