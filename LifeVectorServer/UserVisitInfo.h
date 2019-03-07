/**
 * @file UserVisitInfo.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief Class holding all the squashed and processed user 
 * visit information
 * @version 0.1
 * 
 */
#ifndef USER_VISIT_INFO_H
#define USER_VISIT_INFO_H

#include <map>
#include <string>
#include <iostream>
#include <sstream>

#include "SystemTimeManager.h"

/**
 * @brief Class holding all the squashed and processed user 
 * visit information
 */
class UserVisitInfo
{
  private:
    int visitFrequency;
    long totalVisitDuration;
    std::map<long, int> timeLog;

  public:
    /**
     * @brief Construct a new User Visit Info object
     */
    UserVisitInfo();

    /**
     * @brief Destroy the User Visit Info object
     * 
     */
    ~UserVisitInfo();

    /**
     * @brief adds a single instance of squashed data into the 
     * object map
     * 
     * @param time_stamp long - start time when reached location 
     * in unix time format 
     * @param time_spent int - time spent at the at instance in
     * seconds
     */
    void addSingleLog(long time_stamp, int time_spent);

    /**
     * @brief Updates entries in object with a fully fledged
     * UserVisitInfo object 
     * 
     * @param log UserVisitInfo - user log
     * @return true if successfully added
     * @return false if addition to object unsuccessful
     */
    bool updateLog(UserVisitInfo log);

    // Getters:
  
    /**
     * @brief Get the Time Log Map Container
     * 
     * @return std::map<long, int> 
     */
    std::map<long, int> getTimeLog();

    /**
     * @brief Get the Visit Frequency
     * 
     * @return int 
     */
    int getVisitFrequency();

    /**
     * @brief Get the Total Time Spent
     * 
     * @return int 
     */
    int getTotalTimeSpent();

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