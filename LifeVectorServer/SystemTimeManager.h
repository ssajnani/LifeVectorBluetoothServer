/**
 * @file SystemTimeManager.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief Handles the time conversions from UNIX time to a readable
 * time format.
 * @version 0.1
 * @date 2018-11-29
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef SYSTEM_TIME_MANAGER
#define SYSTEM_TIME_MANAGER

#include <time.h>
#include <string>

/**
 * @brief Handles the time conversions from UNIX time to a readable
 * 
 */
class SystemTimeManager
{
private:
  time_t intTime;
  struct tm *UTCformat, *ESTformat;

public:
  /**
   * @brief Construct a new System Time Manager object -
   * initializes object with the current time
   */
  SystemTimeManager();  

  /**
   * @brief Construct a new System Time Manager object - 
   * initializes object with a given UNIX time
   * @param unixTime 
   */
  SystemTimeManager(long unixTime); 

  /**
   * @brief Destroy the System Time Manager object
   */
  ~SystemTimeManager();            

  /**
   * @brief converts the stored time in Eastern Time Zone 
   * into a readable format
   * 
   * @return std::string beautified time
   */
  std::string UNIXtoEST();

  /**
   * @brief converts the stored time in Coordinated Universal 
   * Time into a readable format
   * 
   * @return std::string beautified time
   */
  std::string UNIXtoUTC();

  /**
   * @brief Get the Time in UNIX integer format
   *  
   * @return long 
   */
  long getTime();

  /**
   * @brief static function. converts duration in seconds to
   * a string of hours and minutes
   * 
   * @param duration int - amount of time in seconds
   * @return std::string - the amount of time in HH:MM format
   */
  static std::string format(int duration);
};

#endif
