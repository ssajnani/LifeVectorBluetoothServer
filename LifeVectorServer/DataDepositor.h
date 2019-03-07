/**
 * @file DataDepositor.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief This Class is a method used to save the raw, unprocessed GPS
 * coordinate data received from the mobile device to the database. 
 * The raw data is the parsed json stored in C++ objects already. The
 * database pointer received is already initialized, with connection SQL 
 * server already established. This is saved according to the user,
 * so deviceID is used to identify the user this data being stored
 * is assoiated with.
 * @version 0.1
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef DATA_DEPOSITOR_H
#define DATA_DEPOSITOR_H

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RawDataRepository.h"
#include "Database.h"


/**
 * @brief This Class is a method used to save the raw, unprocessed GPS
 * coordinate data received from the mobile device to the database. 
 * The raw data is the parsed json stored in C++ objects already. The
 * database pointer received is already initialized, with connection SQL 
 * server already established. This is saved according to the user,
 * so deviceID is used to identify the user this data being stored
 * is assoiated with.
 * 
 */
class DataDepositor
{
private:
  Database *db_link; // pointer to database connector

  /**
     * @brief Helper Function to concatenate raw gps log data
     * to its associated user in a string
     * 
     * @param timestamp 
     * @param latitude 
     * @param longitude 
     * @param user 
     * @param device 
     * @return std::string the concatentated string
     */
  std::string concatRawQuery(long timestamp, double latitude, double longitude, std::string user, std::string device);

public:
  /**
   * @brief Construct a new Data Depositor object
   * 
   * @param db_pointer Database* - pointer to database object
   * already linked to a database
   */
  DataDepositor(Database *db_pointer);

  /**
   * @brief Destroy the Data Depositor object
   * 
   */
  ~DataDepositor();

  
  /**
   * @brief Saves a whole set of raw GPS data into the SQL
   * database, with the associated user.
   * 
   * @param username string 
   * @param deviceID string
   * @param dataset RawDataRepository - the raw, unprocessed 
   * set of logged GPS data.  
   * @return true if successfully saved into the SQL database
   * @return false if save to database failed.
   */
  bool depositRaw(std::string username, std::string deviceID, RawDataRepository dataset);

};

#endif
