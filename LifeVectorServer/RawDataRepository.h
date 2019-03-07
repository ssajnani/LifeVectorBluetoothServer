/**
 * @file RawDataRepository.h
 * @author Rukun Wang (rwang263@uwo.ca)
 * @brief This class is responsible for the reading and parsing of the 
 * raw GPS data file received from mobile device for processing. It 
 * parses the .json file into a vector container, holding the timestamp 
 * and coordinate data. It is also capable of storing the raw data into 
 * the server database, if needed.
 * @version 0.1
 * @date 2018-11-29
 * 
 * @copyright Copyright (c) 2018
 *
 */
#ifndef RAW_DATA_REPOSITORY_H
#define RAW_DATA_REPOSITORY_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <iostream>

#include "json.hpp"

/**
 * @brief This class is responsible for the reading and parsing of the 
 * raw GPS data file received from mobile device for processing. It 
 * parses the .json file into a vector container, holding the timestamp 
 * and coordinate data. It is also capable of storing the raw data into 
 * the server database, if needed.
 * 
 */
class RawDataRepository
{
private:
  /**
   * @brief gpsLog is a structure holding the raw gps coordinate data
   * "time" is the time stamp of the log, saved in UNIX time format
   * "latitude" and "longitude" are the coordinates, in decimal degrees.     * They range from -90 (S) to 90 (N) and -180 (W) to 180 (E), respectively
   */
  struct gpsLog
  {
    long time;
    double latitude, longitude;
  };

  nlohmann::json jsonFile;        // the object holding the imported .json file
  std::map<long, gpsLog> rawData; // map holding the raw gps coordinates
  std::vector<long> logTimes;     // vector of times with logged gps data

  /**
   * @brief function to import the .json file
   * 
   * @param source string filename of .json file
   */
  void initJSON(std::string source);

  /**
   * @brief function used to parse the json object to extract 
   * the timestamp and coordinate data to be stored in the vector
   * 
   * @param fileSpec string - the json attribute of name identifying
   * the GPS array
   */
  void buildRepository(std::string fileSpec);

  /**
   * @brief helper function to insert a single gps entry into the rawData map
   * 
   * @param time long - time stamp in UNIX format
   * @param data gpsLog - the struct holding the 
   */
  void mapEntry(long time, gpsLog data);

public:
  /**
   * @brief Construct a new Raw Data Repository object
   * 
   * @param sourceFile string filename of .json file
   * @param fileSpecification string - the json attribute of name identifying
   * the GPS array
   */
  RawDataRepository(std::string sourceFile, std::string fileSpecification);
  
  /**
   * @brief Construct a new Raw Data Repository object
   * Alternative constructor only for testing purpose
   */
  RawDataRepository();

  /**
   * @brief Destroy the Raw Data Repository object
   * 
   */
  ~RawDataRepository();

  /**
   * @brief Get the Time Stamps
   * 
   * @return std::vector<long> list of all time stamps in entries in 
   * the raw data set. All in UNIX time format
   */
  std::vector<long> getTimeStamps();

  /**
   * @brief Get the GPS Coordinates pair
   * 
   * @param coordinates double* coordinate pair stored in a pointer
   * @param timestamp long time stamp in UNIX time format
   */
  void getCoordinates(double *coordinates, long timestamp);

  /**
   * @brief prints out all the entries held in the raw data
   * repository
   * 
   */
  void dumpData();

  /**
   * @brief Testing method to add single point
   * 
   * @param t 
   * @param lat 
   * @param lng 
   */
  void addTestData(long t, double lat, double lng);
};

#endif