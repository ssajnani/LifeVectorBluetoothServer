/**
 * @file LocationInformation.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief Stores and handles all qualitative Location Information
 * including the location ID, name and address. It also holds the 
 * location descriptions, describing location activity category
 * @version 0.1
 * @date 2018-11-29
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef LOCATION_INFORMATION_H
#define LOCATION_INFORMATION_H

#include <string>

/**
 * @brief Stores and handles all qualitative Location Information
 * including the location ID, name and address. It also holds the 
 * location descriptions, describing location activity category 
 */
class LocationInformation
{
private:
  int id; // shorthand location identifier
  std::string name,
      address,
      description; // describes location activity category

public:
  /**
     * @brief Construct a new Location Information object
     * 
     * @param locationID int
     */
  LocationInformation(int locationID);

  /**
     * @brief Destroy the Location Information object
     * 
     */
  ~LocationInformation();

  /**
     * @brief Set the Name of location
     * 
     * @param location_name string
     */
  void setName(std::string location_name);

  /**
     * @brief Set the Address of location
     * 
     * @param location_address string
     */
  void setAddress(std::string location_address);

  /**
     * @brief Set the Description of location
     * 
     * @param location_description string
     */
  void setDescription(std::string location_description);

  /**
     * @brief Get the location ID
     * 
     * @return int 
     */
  int getID();

  /**
     * @brief Get the Name of location
     * 
     * @return std::string 
     */
  std::string getName();

  /**
     * @brief Get the Address of location
     * 
     * @return std::string 
     */
  std::string getAddress();

  /**
     * @brief Get the Description of location
     * 
     * @return std::string 
     */
  std::string getDescription();
};

#endif