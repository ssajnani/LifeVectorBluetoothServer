/**
 * @file VisitLog.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief Class holding all the squashed and processed user 
 * visit information for a single location for all users
 * @version 0.1
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef VISIT_LOG_H
#define VISIT_LOG_H

#include <map>
#include <utility>
#include <set>
#include <vector>
#include <string>
#include <sstream>

#include "UserVisitInfo.h"
#include "StringParser.h"

class VisitLog
{
private:
  int visitCount, durationSpent;

  std::set<std::string> userLog;
  std::map<std::string, UserVisitInfo> userVisitRecord;

  /**
     * @brief update the visit count, and duration counters
     * 
     * @param log_entry 
     */
  void updateCounters(UserVisitInfo log_entry);

public:
  /**
   * @brief Construct a new Visit Log object
   */
  VisitLog();

  /**
   * @brief Destroy the Visit Log object 
   */
  ~VisitLog();

  /**
   * @brief adds a new user's visitation log information entry 
   * 
   * @param userID string - a composed id with username and deviceID
   * @return true if visit log for user is succefully created
   * @return false if visit log creation for user failed  
   */
  bool addNewUser(std::string userID);

  /**
   * @brief adds a fully populated visitation log for a single
   * user to the location visit log
   * 
   * @param userID string - a composed id with username and deviceID
   * @param log UserVisitInfo - full log
   * @return true if addition is successful
   * @return false if addition failed
   */
  bool addFullLog(std::string userID, UserVisitInfo log);

  /**
   * @brief adds a single visit instance to the visit log
   * 
   * @param userID string - a composed id with username and deviceID
   * @param time long - start time when reached location 
   * in unix time format 
   * @param duration int - time spent at the at instance in
   * seconds
   * @return true 
   * @return false 
   */
  bool addSingleEntry(std::string userID, long time, int duration);

  /**
   * @brief Get the User Visit Information mapped to each specific 
   * user in database
   * 
   * @param userID string - a composed id with username and deviceID
   * @return std::map<std::string, UserVisitInfo>::iterator map of
   * all visit log information mapped each user, using userID
   */
  std::map<std::string, UserVisitInfo>::iterator getUserVisitInformation(std::string usernID);

  /**
   * @brief finds the user in the user set
   * 
   * @param userID string - a composed id with username and deviceID
   * @return std::set<std::string>::iterator points at location user
   * resides in the user set. returns the end of iterator if user not
   * present
   */
  std::set<std::string>::iterator findUser(std::string userID);

  /**
   * @brief Get the User set
   * 
   * @return std::set<std::string> set holding all the users that has 
   * visited this location
   */
  std::set<std::string> getVisitUsers();

  /**
   * @brief Get the Total Times Location was visited, by all users
   * 
   * @return int 
   */
  int getTotalTimesVisited();

  /**
   * @brief Get the Total Time Spent at this location, by all users
   * 
   * @return int 
   */
  int getTotalTimeSpent();

  // bool checkers:

  /**
   * @brief check to see if user has visited this location
   * 
   * @param userID string - a composed id with username and deviceID
   * @return true if user has been here before
   * @return false if user has not been to this location
   */
  bool userFound(std::string userID);

  /**
     * @brief converts information in the object into a formatted
     * string  
     * 
     * @return std::string 
     */
  std::string beautify();

  /**
     * @brief prints formatted string to console
     * 
     */
  void printLog();
};

#endif