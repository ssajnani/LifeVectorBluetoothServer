/**
 * @file CoordinateInformation.h
 * @author Rukun Wang (rwang264@uwo.ca)
 * @brief Stores and handles all quantitative Location Information
 * including the reference gps coordinate and the location boundary
 * limits 
 */
#ifndef COORDINATION_INFORMATION_H
#define COORDINATION_INFORMATION_H

/**
 * @brief Stores and handles all quantitative Location Information
 * including the reference gps coordinate and the location boundary
 * limits 
 */
class CoordinateInformation
{
private:
  double latitude, longitude; // reference coordinate point
  double north, east, south, west; // directional boundaries of the location around reference point

public:
  /**
   * @brief Construct a new Coordinate Information object
   * 
   * @param latitudeReference int
   * @param longitudeReference int 
   */
  CoordinateInformation(double latitudeReference, double longitudeReference);

  /**
   * @brief Destroy the Coordinate Information object
   * 
   */
  ~CoordinateInformation();

  /**
   * @brief Set the location boundary limits
   * 
   * @param northLimit double northern boundary limit
   * @param southLimit double southern boundary limit
   * @param eastLimit double eastern boundary limit
   * @param westLimit double western boundary limit
   */
  void setLimits(double northLimit, double southLimit, double eastLimit, double westLimit);

  /* ~~ GETTERS: ~~ */

  /**
   * @brief Get the Reference Latitude
   * 
   * @return double Latitude
   */
  double getLatitude();

  /**
   * @brief Get the Reference Longitude 
   * 
   * @return double Longitude
   */
  double getLongitude();

  /**
   * @brief Get the North Boundary Limit
   * 
   * @return double 
   */
  double getNorthLimit();

  /**
   * @brief Get the South Boundary Limit
   * 
   * @return double 
   */
  double getSouthLimit();

  /**
   * @brief Get the East Boundary Limit
   * 
   * @return double 
   */
  double getEastLimit();

  /**
   * @brief Get the West Boundary Limit
   * 
   * @return double 
   */
  double getWestLimit();
};

#endif