/**
 * @file user.h
 * 
 * @author group 3
 * 
 * @brief creates a user or sets values for existing users. Has getters and setters to accomplish this
 */
#ifndef USER_H
#define USER_H

#include <iostream>
#include <unistd.h>
#include <string>
//json parser for c++
#include "json.hpp"
using json = nlohmann::json;

/**
 * @brief creates a user or sets values for existing users. 
 * Has getters and setters to accomplish this
 * 
 */
class User  
{
 
	public:

		/** constructor for new user so no report or report time
		 * @param username the name of the user
		 * @param deviceID the name of the device
		 * @param hash the hash value generated and associated to the user
		 * @param salt the salt value generated and associated to the userc
		 */
        User(std::string username, std::string deviceID, std::string hash, std::string salt, json report, int syncTime, int reportTime);
		
		/** 
		 * @brief full constructor
		 * @param username the name of the user
		 * @param deviceID the name of the device
		 * @param hash the hash value generated and associated to the user
		 * @param salt the salt value generated and associated to the user
		 * @param syncTime the time it takes to sync
		 * @param report the json report
		 */
		User(std::string username, std::string deviceID, std::string hash, std::string salt);
        
		/** 
		 * @brief barebones constructor
		 * @param username the name of the user
		 * @param deviceID the name of the device
		 */
        User(std::string username,std::string deviceID);

		/** 
		 * @brief destructor
		 */
		~User();

		/** 
		 * @brief returns the name of the user
		 * @return self_username
		 */
        std::string getUsername();

		/** 
		 * @brief returns the id of the device
		 * @return self_deviceID
		 */
		std::string getDeviceID();

		/** 
		 * @brief returns the hash
		 * @return self_hash
		 */
		std::string getHash();

		/** 
		 * @brief returns the salt
		 * @return self_salt
		 */
	    std::string getSalt();

		/** 
		 * @brief returns the report
		 * @return self_report
		 */
	    json getReport();

		/** 
		 * @brief returns the sync time
		 * @return self_syncTime
		 */
	    long getSyncTime();

		/** 
		 * @brief returns the report time
		 * @return self_reportTime
		 */
	    long getReportTime();

		/** 
		 * @brief sets the username
		 * @param username
		 */
        void setUsername(std::string username);

		/** 
		 * @brief sets the device id
		 * @param deviceID
		 */
	    void setDeviceID(std::string devID);

		/** 
		 * @brief sets the hash
		 * @param hash
		 */
		void setHash(std::string hash);

		/** 
		 * @brief sets the salt
		 * @param salt
		 */
	    void setSalt(std::string salt);

		/** 
		 * @brief sets the report
		 * @param report
		 */
	    void setReport(json report);

		/** 
		 * @brief sets the sync time
		 * @param syncTime
		 */
	    void setSyncTime(long syncTime);

		/** 
		 * @brief sets the report time
		 * @param reportTime
		 */
	    void setReportTime(long reportTime);

	private:

        std::string self_username;
		std::string self_deviceID;
		std::string self_hash;
		std::string self_salt;
		json self_report;
		long self_syncTime;
		long self_reportTime;
		
};

#endif
