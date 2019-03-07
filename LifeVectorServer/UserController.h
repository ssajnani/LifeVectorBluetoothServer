/**
 * @file UserController.h
 * 
 * @author group 3
 * 
 * @brief Controller for the user information stored in database
 * 
 */
#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <stdlib.h>

#include "UserLibrary.h"
#include "user.h"
#include "hashfunc.h"
#include "RawDataRepository.h"

#include "json.hpp"
using json = nlohmann::json;

/**
 * @brief Controller for the user information stored in database
 * 
 */
class UserController
{
public:
  /**
   * @brief Construct a new User Controller object
   * 
   * @param dbUsername MySQL username
   * @param dbPassword MySQL password
   */
  UserController(std::string dbUsername, std::string dbPassword);

  /**
   * @brief Destroy the User Controller object
   * 
   */
  ~UserController();

  /**
   * @brief Create user and save into database
   * 
   * @param username User's username
   * @param devID User's device ID
   * @param password User's password
   * @return true User created
   * @return false User creation failed
   */
  bool createUser(std::string username, std::string devID, std::string password);

  /**
   * @brief Get the user object from database
   * 
   * @param target User pointer to store the data
   * @param username User's username
   * @param devID User's device ID
   * @return true User found 
   * @return false User not found
   */
  bool retrieveUser(User *target, std::string username, std::string devID);

  /**
   * @brief Delete user from database
   * 
   * @param username User's username
   * @param devID User's device ID
   * @return true User deleted
   * @return false User not deleted (possibly not found)
   */
  bool deleteUser(std::string username, std::string devID);

  /**
   * @brief Replace user report data
   * 
   * @param username User's username
   * @param deviceID User's device ID
   * @param newReport New report data 
   * @return true Report updated
   * @return false Failed (possibly user not found)
   */
  bool updateReport(string username, string deviceID, json newReport);

  /**
   * @brief Get the report from database
   * 
   * @param dbReport JSON pointer to store the report
   * @param username User's username
   * @param deviceID User's device ID
   * @return true Report found
   * @return false Report not found
   */
  bool fetchReport(json* dbReport, string username, string deviceID);

  /**
   * @brief Save raw GPS data into database
   * 
   * @param username User's username
   * @param deviceID User's device ID
   * @param rawDataSet Raw data to be input
   * @return true Data saved
   * @return false Saving failed (possibly user not found)
   */
  bool saveRawGPStoDB(std::string username, std::string deviceID, RawDataRepository rawDataSet);


  /**
   * @brief Authenticate user's identity by comparing the hash
   * 
   * @param username User's username
   * @param devID User's device ID
   * @param password User's password
   * @return true Hash matched
   * @return false Hash not matched
   */
  bool compareUserHash(std::string username, std::string devID, std::string password);

  /**
   * @brief Get the database object
   * 
   * @return Database* Pointer to database used
   */
  Database *getDBConnection();

private:
  UserLibrary userLibrary;
};

#endif
