/**
 * @file UserLibrary.h
 * 
 * @author group 3
 * 
 * @brief Directly controll the user data in database
 */

#ifndef USERLIBRARY_H
#define USERLIBRARY_H

#include <string>
#include <mysql/mysql.h> //https://dev.mysql.com/downloads/connector/c/
#include "json.hpp"

#include "Database.h"
#include "user.h"
#include "DataDepositor.h"

/**
 * @brief Directly controll the user data in database
 * 
 */
class UserLibrary
{
  public:
	/**
	 * @brief Construct a new UserLibrary object
	 * 
	 * @param userName MySQL username
	 * @param password MySQL password
	 */
	UserLibrary(std::string userName, std::string password);

	/**
	 * @brief Create a user in database
	 * 
	 * @param user User object to be stored
	 * @return true User created
	 * @return false User creation failed
	 */
	bool createUserInDB(User user);

	/**
	 * @brief Delete a user from database
	 * 
	 * @param username User's username
	 * @param deviceID User's device ID
	 * @return true User deleted 
	 * @return false User deteting failed 
	 */
	bool deleteUserFromDB(string username, string deviceID);

	/**
	 * @brief Get the user from database
	 * 
	 * @param user User pointer to store the user object
	 * @param username User's username
	 * @param deviceID User's device ID
	 * @return true User found
	 * @return false User not found
	 */
	bool retrieveUserFromDB(User *user, std::string username, std::string deviceID);
	Database *getDBConnection();

	/**
	 * @brief Update User's last synchronization time
	 * 
	 * @param user User to be updated
	 * @param syncTime Synchronization time to be updated
	 * @return true Updated 
	 * @return false Update failed
	 */
	bool updateUserSyncTime(User user, int syncTime);

	/**
	 * @brief Update User's report data
	 * 
	 * @param user User to be updated
	 * @param report Report data to be updated
	 * @return true Report updated
	 * @return false Update failed
	 */
	bool updateReport(User user, nlohmann::json report);

	/**
	 * @brief Save raw gps data into database
	 * 
	 * @param username User's username
	 * @param deviceID User's device ID
	 * @param rawDataSet New raw GPS data
	 * @return true Updated
	 * @return false Failed
	 */
	bool uploadRawGPSData(std::string username, std::string deviceID, RawDataRepository rawDataSet);

	/**
	 * @brief Helper method to determine if a user is in the database
	 * 
	 * @param username User's username
	 * @param deviceID User's device ID
	 * @return true User is found in database
	 * @return false User is not found in database
	 */
	bool isRegistered(std::string username, std::string deviceID);

	/**
	 * @brief Print all user information 
	 * 
	 * @return true Printed
	 * @return false Print failed
	 */
	bool printAllUsers();

  private:
	Database db;

	// Helpers
	bool isPresent(std::string deviceID);
};

#endif
