/**
 * @file hashfunc.h
 * @brief Using md5 to encrypt data
 * 
 */
#ifndef HASHFUNC_H
#define HASHFUNC_H

#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <stdlib.h> 
#include <linux/random.h>

/**
 * @brief Using md5 to encrypt data
 * 
 */
class hashfunc  {
 
	public:
		
        /**
         * @brief Hashing a string data
         * 
         * @param str String data to be hashed
         */
		void hashString(std::string str);

        /**
         * @brief Get the Salt object
         * 
         * @return Salt
         */
        std::string getSalt();

        /**
         * @brief Get the Hash object
         * 
         * @return Hash result
         */
        std::string getHash();
		
        /**
         * @brief Alternative getter for hash object
         * 
         * @param password 
         * @param salt 
         * @return Hash result
         */
        std::string getHash(std::string password, std::string salt);


	private:

        std::string salt;
        std::string hash;
		std::string password;

};

#endif
