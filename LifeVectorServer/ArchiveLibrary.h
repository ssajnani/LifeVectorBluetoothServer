/**
 * @file ArchiveLibrary.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief The Library to handle interfacing between the C++ objects
 * and database. Generates and updates Archived Locations and the 
 * Visit Logs related to each location and for all users.
 * @version 0.1
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#ifndef ARCHIVE_LIBRARY_H
#define ARCHIVE_LIBRARY_H

#include <map>
#include <set>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "Database.h"
#include "ArchivedLocation.h"
#include "VisitLog.h"
#include "UserVisitInfo.h"

#include "StringParser.h"

/**
 * @brief The Library to handle interfacing between the C++ objects
 * and database. Generates and updates Archived Locations and the 
 * Visit Logs related to each location and for all users.
 */
class ArchiveLibrary
{
private:
  Database *connected_db; // Pointer to a database link to mySQL

  /**
   * @brief Composes the userID for internal querying within the class
   *        Combines the username and the deviceID to userID with a '/'
   * 
   * @param username string - user's username identifier
   * @param deviceID string - device's identifier
   * @return std::string a combo userID string
   */
  std::string composeID(std::string username, std::string deviceID);

  /* Helpers: */
  /**
   * @brief Helper method to take retrieved data related to user logs
   * and insert them into C++ containers and object. 
   * 
   * @param userLog map<int, UserVisitInfo> * - the pointer pointing 
   * to the map that contains all the user visitation information, 
   * identified by the ID of location it is associated with  
   * @param result string - the input to function. It represents 
   * all the entries returned from the SQL query.
   */
  void userLogDBtoObj(std::map<int, UserVisitInfo> *userLog, std::string result);

  /**
   * @brief Helper method to take retrieved data related to location 
   * based logs and insert them into C++ containers and object
   * 
   * @param record VisitLog - the container for visit information at
   * a single speicified location for all users that have visited 
   * @param result string - the input to the function. It represents 
   * all the entries returned from the SQL query.
   */
  void locationLogDBtoOBJ(VisitLog *record, std::string result);

public:
  /**
   * @brief Construct a new Archive Library object
   * 
   * @param db_pointer Database* - points to a database client 
   * controller, handles all queries to the SQL database
   */
  ArchiveLibrary(Database *db_pointer);

  /**
   * @brief Destroy the Archive Library object
   * 
   */
  ~ArchiveLibrary();

  /* Location Functions: */

  /**
   * @brief Constructs a fully identified and defined Archive Location.
   * 
   * @param id int - location identifier
   * @param name string - name of the location
   * @param address string - physical address to the location
   * @param description - qualitative descriptors related to the location
   * @param latitude double - reference GPS latitude point of location
   * @param longitude double - reference GPS longitude point of location
   * @param northBound double - the northern boundary of the location
   * @param southBound double - the southern boundary of the location 
   * @param eastBound double - the eastern boundary of the location
   * @param westBound double - the western boundary of the location
   * @return ArchivedLocation object holding all the information related
   * to a location
   */
  ArchivedLocation constructLocation(int id, std::string name, std::string address, std::string description,
                                     double latitude, double longitude,
                                     double northBound, double southBound, double eastBound, double westBound);

  /**
   * @brief Get the Location From Database
   * 
   * @param id int - the ID of the location of interest
   * @return ArchivedLocation* point to the location of the given ID
   * found in the database. Returns a NULL pointer if location is not
   * known (not found in database)
   */
  ArchivedLocation *getLocationFromDatabase(int id);

  /**
   * @brief Saves a location object to the database
   * 
   * @param location ArchivedLocation - the object of the location to be
   * saved into the database.
   * @return true - location saved to database successfully
   * @return false - location failed to save to database
   */
  bool saveLocationToDatabase(ArchivedLocation location);

  /**
   * @brief Get the Adjacent Locations - Finds all the matching adjacent 
   * locations to the GPS point provided.
   * 
   * @param matchedLocations map<int, CoordinateInformation>& - container
   * holding all the adjacent matching locations. Container content is
   * only the relevant coordinate information, identified by the 
   * location's id
   * @param latitude double - the input x-coordinate
   * @param longitude double - the input y-coordinate
   * @return true if query to databse is success and the map is 
   * successfully created and stored.
   * @return false if not matching locations are found in the 
   * database
   */
  bool getAdjacentLocations(std::map<int, CoordinateInformation> &matchedLocations, double latitude, double longitude);

  /**
   * @brief Gets the ID of a matched location with the closest 
   * reference point to the inputed GPS Coordinate
   * 
   * @param matchedLocations map<int, CoordinateInformation>& - container
   * holding all the adjacent matching locations. Container content is
   * only the relevant coordinate information, identified by the 
   * location's id
   * @param latitude double - the input x-coordinate
   * @param longitude double - the input y-coordinate
   * @return int the location ID of the closest known matched location
   */
  int matchNearestLocation(std::map<int, CoordinateInformation> &matchedLocations, double latitude, double longitude);

  /* Location Visit Records Functions: */

  /**
   * @brief Archives a squashed GPS log of coordinate data in the database.
   * This log is identified by the associated location (with ID) and the 
   * user who generated the information
   * 
   * @param locationID int - location identifier
   * @param user - string username of the user who generated the log
   * @param device - sting deviceID of the user
   * @param userVisitLog - the User specific tracking log of squashed
   * information
   * @return true if log data has been successfully archived into
   * the database 
   * @return false if log data failed to save into the database
   */
  bool archiveUserLog(int locationID, std::string user, std::string device, UserVisitInfo userVisitLog);

  /**
   * @brief Get the User Log Map From Database - 
   * This function retrieves all the VisitLog information for all the
   * locations the specified user has visited.  
   * 
   * @param user string - the username of user of interest
   * @param device string - the device ID of the user of interest
   * @return std::map<int, UserVisitInfo>* a map of all the user visit
   * information identified by the associated location ID
   */
  std::map<int, UserVisitInfo> *getUserLogFromDatabase(std::string user, std::string device);

  /**
   * @brief Get the User Log Map From Database - 
   * This function retrieves all the VisitLog information for all the
   * locations the specified user has visited between a specified
   * time interval.
   *  
   * @param user string - the username of user of interest
   * @param device string - the device ID of the user of interest
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return std::map<int, UserVisitInfo>* a map of all the user visit
   * information identified by the associated location ID
   */
  std::map<int, UserVisitInfo> *getUserLogBetween(std::string user, std::string device, long start, long fin);

  /**
   * @brief Get the Location Record From Database object -
   * Retrieves the visit record for all the users that have visited
   * this location
   * 
   * @param locationID int - the location ID of the place of interest
   * @return VisitLog* a VisitLog object holding all the associated 
   * user visit information for all users
   */
  VisitLog *getLocationRecordFromDatabase(int locationID);

  /**
   * @brief Get the Location Record From Database object -
   * Retrieves the visit record for all the users that have visited
   * this location between a specified time interval.
   * 
   * @param locationID int - the location ID of the place of interest
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return VisitLog* a VisitLog object holding all the associated 
   * user visit information for all users
   */
  VisitLog *getLocationRecordFBetween(int locationID, long start, long fin);

  /**
   * @brief Get the Visit Count at the specified location for all the 
   * users that has visited
   * 
   * @param locationID int - the location ID of the place of interest
   * @return int - the count (frequency) of visits at the location, 
   * returns 0 if query failed or no visits at this location
   */
  int getVisitCount(int locationID);

  /**
   * @brief Get the Visit Count at the specified location for the 
   * specified user
   * 
   * @param locationID int - the location ID of the place of interest
   * @param user string - the username of user of interest
   * @param device string - the device ID of the user of interest
   * @return int - the count (frequency) of visits at the location, 
   * returns 0 if query failed or no visits at this location
   */
  int getVisitCount(int locationID, std::string user, std::string device);

  /**
   * @brief Get the Visit Count at the specified location for all the 
   * users that has visited between a specified time interval. 
   * 
   * @param locationID int - the location ID of the place of interest
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return int - the count (frequency) of visits at the location, 
   * returns 0 if query failed or no visits at this location
   */
  int getCountBetween(int locationID, long start, long fin);

  /**
   * @brief Get the Visit Count at the specified location for the 
   * specified user between a specified time interval.
   * 
   * @param locationID int - the location ID of the place of interest
   * @param user string - the username of user of interest
   * @param device string - the device ID of the user of interest
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return int - the count (frequency) of visits at the location, 
   * returns 0 if query failed or no visits at this location
   */
  int getCountBetween(int locationID, std::string user, std::string device, long start, long fin);

  /**
   * @brief Get the Total Visit Count for all locations and all users
   * 
   * @return int - the total count (frequency) of visits made, 
   * returns 0 if query failed or no visits found
   */
  int getTotalVisitCount();

  /**
   * @brief Get the Total Visit Count for all locations and all users
   * between a specified time interval.
   * 
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return int - the total count (frequency) of visits made, 
   * returns 0 if query failed or no visits found
   */
  int getTotalCountBetween(long start, long fin);

  /**
   * @brief Get the Duration spent at the specified location for all the 
   * users that has visited
   * 
   * @param locationID int - the location ID of the place of interest
   * @return int the duration spent at location, in seconds,
   * returns 0 if query failed or no time spent there
   */
  int getDurationAtLocation(int locationID);

  /**
   * @brief Get the Duration spent at the specified location for the 
   * specified user
   * 
   * @param locationID int - the location ID of the place of interest
   * @param user string - the username of user of interest
   * @param device string - the device ID of the user of interest
   * @return int the duration spent at location, in seconds,
   * returns 0 if query failed or no time spent there
   */
  int getDurationAtLocation(int locationID, std::string user, std::string device);

  /**
   * @brief Get the Duration spent at the specified location for all the 
   * users that has visited between a specified time interval.
   * 
   * @param locationID int - the location ID of the place of interest
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return int the duration spent at location, in seconds,
   * returns 0 if query failed or no time spent there
   */
  int getDurationBetween(int locationID, long start, long fin);

  /**
   * @brief Get the Duration spent at the specified location for the 
   * specified user between a specified time interval.
   * 
   * @param locationID int - the location ID of the place of interest
   * @param user string - the username of user of interest
   * @param device string - the device ID of the user of interest
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return int the duration spent at location, in seconds,
   * returns 0 if query failed or no time spent there
   */
  int getDurationBetween(int locationID, std::string user, std::string device, long start, long fin);

  /**
   * @brief Get the Total Duration spent all the location by all the users
   * 
   * @return int the total duration spent, in seconds, 
   * returns 0 if query failed or no time spent there
   */
  int getTotalDuration();

  /**
   * @brief Get the Total Duration spent all the location by all the users
   * between a specified time interval
   * 
   * @param start long - the starting point of the time interval in 
   * UNIX time format
   * @param fin long - the end point of the time interval in UNIX
   * time format
   * @return int the total duration spent, in seconds, 
   * returns 0 if query failed or no time spent there
   */
  int getTotalDurationBetween(long start, long fin);

  /* Checks */

  /**
   * @brief Helper function that checks if the provide ID
   * is of a known location
   * 
   * @param locationID int - the location ID of the place of interest
   * @return true if location is in database, known
   * @return false if location is not in database, not known
   */
  bool isKnown(int locationID);

  /**
   * @brief Helper function that provides a quick check on whether
   * a user of the provided username and deviceID is a known,
   * registered user in the database
   * 
   * @param user string - the username of user of interest
   * @param device string - the device ID of the user of interest
   * @return true if user is found in database
   * @return false if user is not found in database
   */
  bool isRegistered(std::string user, std::string device);

  /**
   * @brief Get the Last Location ID
   * 
   * @return int location ID
   */
  int getLastLocationID();
};

#endif
