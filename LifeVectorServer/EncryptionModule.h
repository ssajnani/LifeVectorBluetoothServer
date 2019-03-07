/*if needed install ssl:sudo apt-get install libssl-dev*/
/**
 * @file EncryptionModule.h
 * @author Samar Sajnani (ssajnani@uwo.ca)
 * @brief Encryption Module for SSL encryption
 * @date -------
 * @version 
 * @copyright 	
 *
 */

#ifndef ENCRYPTIONMODULE_H
#define ENCRYPTIONMODULE_H
#define LOG(x) \
	cout << x << endl;
#include <string>
#include <memory>
#include <limits>
#include <stdexcept>

#include <iostream>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <cstring>

#define KEY_LENGTH 2048		  //Length of RSA Key
#define PUBLIC_EXPONENT 65537 //Public exponent should be a prime number.
#define PUBLIC_KEY_PEM 1
#define PRIVATE_KEY_PEM 0

static const unsigned int KEY_SIZE = 32; //Constants for AES
static const unsigned int BLOCK_SIZE = 16;

/**
 * @brief Define a new allocator for string to create a secure_string
 * The allocator will allocate, deallocate, destroy, construct, 
 * or rebind the secure string
 * 
 * @tparam T 
 */

template <typename T>
struct zallocator
{
  public:
	typedef T value_type;
	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	pointer address(reference v) const { return &v; }
	const_pointer address(const_reference v) const { return &v; }

	pointer allocate(size_type n, const void *hint = 0)
	{
		if (n > std::numeric_limits<size_type>::max() / sizeof(T))
			throw std::bad_alloc();
		return static_cast<pointer>(::operator new(n * sizeof(value_type)));
	}

	void deallocate(pointer p, size_type n)
	{
		OPENSSL_cleanse(p, n * sizeof(T));
		::operator delete(p);
	}

	size_type max_size() const
	{
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}

	template <typename U>
	struct rebind
	{
		typedef zallocator<U> other;
	};

	void construct(pointer ptr, const T &val)
	{
		new (static_cast<T *>(ptr)) T(val);
	}

	void destroy(pointer ptr)
	{
		static_cast<T *>(ptr)->~T();
	}

#if __cpluplus >= 201103L
	template <typename U, typename... Args>
	void construct(U *ptr, Args &&... args)
	{
		::new (static_cast<void *>(ptr)) U(std::forward<Args>(args)...);
	}

	template <typename U>
	void destroy(U *ptr)
	{
		ptr->~U();
	}
#endif
};

/**
 * @brief New types are defined here including unsigned chars 
 * as bytes, secure_strings and an EVP CIPHER for AES/RSA
 * 
 */
typedef unsigned char byte;
typedef std::basic_string<char, std::char_traits<char>, zallocator<char>> secure_string;
using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

/**
 * @brief ENCRYPTION MODULE HEADER FILE
 * Description: Contains methods for base64 encoding/decoding, 
 * RSA encryption/decryption, AES_256_CTR Cipher encryption/decryption 
 * methods
 * Details: A secure_string struct is defined in order to use the AES 
 * encryption and decryption methods The AES Key and Block size constants 
 * are also defined at the top 
 * Public and Private key constants are also defined for RSA encryption
 */
class EncryptionModule
{
  private:
  public:
	/** @brief gen_params: function to generate an AES key and block size (iv)
	 * @param key - pointer for future location of aes key
	 * @param iv - pointer for future byte array of aes iv
	 *  Takes as input pointers to byte arrays for both the KEY and BLOCK
	 */
	void gen_params(byte key[KEY_SIZE], byte iv[BLOCK_SIZE]);
	/** @brief aes_encrypt: Takes a key, a iv value, a secure_string as the input text (ptext) by reference and a cipher         * text (output text) by reference
	 * @param key - AES key pointer
	 * @param iv - aes block cipher bytes 
	 * @param ptext - pointer to plaintext secure_string
	 * @param ctext - pointer to cipher text secure_string
	 */
	void aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], const secure_string &ptext, secure_string &ctext);
	/** @brief aes_decrypt: Takes a key, iv, secure_string by reference for cipher text (ctext) and secure_string for de         * crypted text (rtext) as a reference
	 *  Takes as input pointers to byte arrays for both the KEY and BLOCK
	 * @param key - AES key pointer
	 * @param iv - aes block cipher bytes 
	 * @param len in int - C++ specific AES decryption requires initialization of decryption array 
	 * @param ctext - pointer to cipher text secure_string
	 * @param rtext - pointer to return secure_string
	 */
	void aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], int, const secure_string &ctext, secure_string &rtext);
	/** @brief envelope_seal: Takes a public key, plaintext with it's length, generates an encrypted key and it's length         * which are passed as pointers. The encrypted key is used to generate a public key encrypted ciphertext
	 * @param pub_key pointer to EVP public key
	 * @param plaintext - pointer to plaintext unsigned char
	 * @param plaintext_len - integer size of plaintext
	 * @param encrypted_key - pointer to a unsigned char*
	 * @param *encrypted_key_len - int *
	 * @param iv - Block size for AES cipher
	 * @param ciphertext - returned cipher text from AES/Public key EVP cipher 
	 * @return int 
	 */
	int envelope_seal(EVP_PKEY **pub_key, unsigned char *plaintext, int plaintext_len, unsigned char **encrypted_key, int *encrypted_key_len, unsigned char *iv, unsigned char *ciphertext);
	/** @brief envelope_seal: Takes a public key, plaintext with it's length, generates an encrypted key and it's length         * which are passed as pointers. The encrypted key is used to generate a public key encrypted ciphertext
	 * @param priv_key pointer to EVP private key
	 * @param plaintext - returned pointer to plaintext unsigned char
	 * @param plaintext_len - integer size of plaintext
	 * @param encrypted_key - pointer to a unsigned char*
	 * @param *encrypted_key_len - int *
	 * @param iv - Block size for AES cipher
	 * @param ciphertext - passed cipher text from AES/Public key EVP cipher 
	 * @return int 
	 */
	int envelope_open(EVP_PKEY *priv_key, unsigned char *ciphertext, int ciphertext_len, unsigned char *encrypted_key, int encrypted_key_len, unsigned char *iv, unsigned char *plaintext);

	/** @brief handleErrors used to properly catch and print error logs
	 */
	void handleErrors(void);

	/** @brief Encryption for base64: takes a pointer to a byte array with a designated length, decodes the bytes from 
	 * decimal to base64. Converts this array to a char* that is returned
	 * @param input - unsigned char pointer 
	 * @param length - length of input
	 * @return char* - return char array
	 */
	char *base64(const unsigned char *input, int length);

	/** @brief base64_decode: 
	 *  Takes as input a reference to base64 string and returns a decoded decimal string as output
	 *  @param encoded_string - passed as reference
	 *  @return decimal decoded string
	 */
	std::string base64_decode(std::string const &encoded_string);

	/** @brief getKeyFromString: 
	 * creates an openSSL RSA key from a pem formatted key string
	 * @param key - public key pem formatted
	 * @param pem_type - type of public key
	 * @param cipher - RSA key pointer for openSSL RSA type
	 */
	void getKeyFromString(std::string key, int pem_type, RSA *cipher);

	/** @brief public_encrypt:       
	 * RSA encryption using an openSSL loaded public RSA key, an input char array from with length flen, and
	 * an output cipher string to all passed as pointers
	 * @param flen - length of from string
	 * @param from - unsigned char* 
	 * @param to - unsigned char*
	 * @param RSA - key*
	 * @param padding - int
	 * @return integer 
	 */
	int public_encrypt(int flen, unsigned char *from, unsigned char *to, RSA *key, int padding);

	/** @brief private_decrypt:       
	 * * RSA decryption using an openSSL loaded RSA private key, an input cipher char array from with length flen
	 * an output char array to
	 * @param flen - length of from string
	 * @param from - unsigned char* 
	 * @param to - unsigned char*
	 * @param RSA - key*
	 * @param padding - int
	 * @return integer 
        */
	int private_decrypt(int flen, unsigned char *from, unsigned char *to, RSA *key, int padding);
	
	/** @brief create_encrypted_file: Write ciphertext char* to a file 
	 *  @param encrypted - char array pointer for the encrypted text
	 *  @param key_pair - RSA* encryption key_pair
	 */
	void create_encrypted_file(char *encrypted, RSA *key_pair);
};
#endif
