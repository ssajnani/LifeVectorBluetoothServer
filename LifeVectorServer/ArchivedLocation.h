/**
 * @file ArchivedLocation.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief Made up of LocationInformation and CoordinateInformation 
 * objects. Used to store information detailing the location. 
 * This includes the qualitative information and quantitative GPS 
 * coordinate information outlining reference coordinate and GPS 
 * boundary in all 4 directions. 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef ARCHIVED_LOCATION_H
#define ARCHIVED_LOCATION_H

#include <string>
#include <iostream>

#include "LocationInformation.h"
#include "CoordinateInformation.h"

/**
 * @brief Made up of LocationInformation and CoordinateInformation 
 * objects. Used to store information detailing the location. 
 * This includes the qualitative information and quantitative GPS 
 * coordinate information outlining reference coordinate and GPS 
 * boundary in all 4 directions. 
 */

class ArchivedLocation
{
private:
  // Attributes:
  LocationInformation details;       // the qualitative information
  CoordinateInformation coordinates; // the quantitative information

public:
  // Constructors:

  /**
   * @brief Passes all empty strings to qualitative attributes and 
   * 0 or 0.0 to quantitative attribute fields
   */
  ArchivedLocation();

  /**
   * @brief Comprehensive Constructor for a new Archived Location 
   * object initialzes all essential and quantitative attributes 
   * for the Location to be archived.
   * 
   * @param locationID int - Location identifier
   * @param locationLatitude double - the reference latitude of the 
   *                         location
   * @param locationLongitude double - the reference longitude of 
   *                          the location 
   * @param northPoint double - the northern boundary of the location
   * @param southPoint double - the southern boundary of the location
   * @param eastPoint double - the eastern boundary of the location 
   * @param westPoint double - the western boundary of the location
   */
  ArchivedLocation(int locationID, double locationLatitude, double locationLongitude, double northPoint, double southPoint, double eastPoint, double westPoint);

  /**
   * @brief Basic Constructor for a new Archived Location object
   * Initializes only the essential attributes
   * 
   * @param locationID int - Location identifier
   * @param locationLatitude double - the reference latitude of the 
   *                         location
   * @param locationLongitude double - the reference longitude of 
   *                          the location 
   */
  ArchivedLocation(int locationID, double locationLatitude, double locationLongitude);

  /**
   * @brief Destroy the Archived Location object
   * 
   */
  ~ArchivedLocation();

  // Setters & Updaters:

  /**
   * @brief Set the Basic Information of the location object
   * 
   * @param locationID int - Location identifier
   * @param locationLatitude double - the reference latitude of the 
   *                         location
   * @param locationLongitude double - the reference longitude of 
   *                          the location 
   */
  void setBasicInformation(int locationID, double locationLatitude, double locationLongitude);

  /**
   * @brief Set the Location Information 
   * 
   * @param name string - name of the location.
   * @param locationAddress string - the physical address of the location
   * @param locationDescription string - information that adds more 
   *                            qualitative descriptors for the location
   *                            using location types and categories from
   *                            the Maps and Places API
   */
  void setLocationInformation(std::string name, std::string locationAddress, std::string locationDescription);

  /**
   * @brief Set the Location Boundaries 
   * 
   * @param north double - the northern boundary of the location
   * @param south double - the southern boundary of the location
   * @param east double - the eastern boundary of the location 
   * @param west double - the western boundary of the location 
   */
  void setBoundaries(double north, double south, double east, double west);

  /**
   * @brief Change the name of the location
   * 
   * @param name string - the new name for the location
   */
  void updateName(std::string name);

  /**
   * @brief Change the address of the location
   * 
   * @param address string - the new address for the location
   */
  void updateAddress(std::string address);

  /**
   * @brief Change the qualitative descriptors for the location
   * these are the location types and categories
   * 
   * @param description string - new information that describes 
   *                    the location
   */
  void updateDescription(std::string description);

  //Location Info Getters:

  /**
   * @brief Fetch the Location Details object
   * 
   * @return LocationInformation the object of all qualitative
   *                             information for location
   */
  LocationInformation getLocationDetails();

  /**
   * @brief Fetches and returns the locations ID
   * 
   * @return int the location identifier
   */
  int getID();

  /**
   * @brief Get the Coordinate Data object
   * 
   * @return CoordinateInformation the object composed of the 
   *                               GPS reference coordinates &
   *                               the 4 boundary points
   */
  CoordinateInformation getCoordinateData();
  double *getLocationReference();

  /**
   * @brief Prints a beautified rendition of all the location
   * information.  
   */
  void printInformation();
};

#endif
