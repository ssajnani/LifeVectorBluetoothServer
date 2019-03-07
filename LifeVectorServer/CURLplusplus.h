/** 
 * @file CURLplusplus.h
 * @brief used to query the google API
 */
#ifndef CURLPLUSPLUS_H
#define CURLPLUSPLUS_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include "json.hpp"
#include <fstream>
/** 
 * @brief
 */
class CURLplusplus{

	private:
	    CURL* curl;
	    std::stringstream ss;
	    long http_code;

	public:
	    CURLplusplus() : curl(curl_easy_init()) , http_code(0){

	    }
	    ~CURLplusplus(){

	        if (curl) curl_easy_cleanup(curl);
	    }
	    /** 
		 * @brief get the string representation of the json we're getting from the google api
		 * @param the url of the site we're querying in our case it's google's specific site for getting location info
		 * @return a string containing json information
		 */
	    std::string Get(const std::string& url);
	    /** 
		 * @brief fetches the http code stored in the class
		 * @return the http code
		 */
	    long GetHttpCode();

	private:
		/** 
		 * @brief writes data to the buffer
		 * @param buffer a buffer to store information
		 * @param size the size of the buffer
		 * @param nmemb a number representing the member
		 * @param userp a pointer to the user
		 * @return the data we need
		 */
	    static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
	   /** 
		* @brief writes data to the buffer
		* @param buffer a buffer to store information
		* @param size the size of the buffer
		* @param nmemb a number representing the member
		* @return the data we need
		*/
	    size_t Write(void *buffer, size_t size, size_t nmemb);
};

#endif

