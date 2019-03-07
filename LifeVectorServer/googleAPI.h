/**
 * @file googleAPI.h
 * @author Abha Sajnani(asajnani@uwo.ca) & Jorge Fernandez
 * @brief This file reads from google maps API JSON and retrieves address details.
 * @version 0.1
 *
 */
#ifndef GOOGLEAPI_H
#define GOOGLEAPI_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>


#include "CURLplusplus.h"
#include "json.hpp"
#include "placesAPI.h"


/**
* @brief This class reads from google maps API JSON and retrieves address details.
*
*/
class googleAPI
{
	private:

	    std::string lat;
	    std::string lng;
	    nlohmann::json doc;
	    std::string name;
        std::string location;
        std::string apostrophe = "\'";
        //when parsing the types of locations, to ensure we find the place NAME we want to remove the types of locations that will not result in a place name but with an address instead
        std::string unwantedTypes = "administrative_area_level_1, administrative_area_level_2, administrative_area_level_3, administrative_area_level_4, administrative_area_level_5, country, intersection, locality, neighborhood, political, post_box, postal_code, postal_code_prefix, postal_code_suffix, postal_town, premise, route, street_address, street_number, sublocality, sublocality_level_4, sublocality_level_5, sublocality_level_3, sublocality_level_2, sublocality_level_1, subpremise";
        /**
        * @brief Helper method that parses the google maps JSON and returns the correct
        *        place ID based on the first set of appropriate 'types'
        *
        * @return std::string the place ID
        */
        std::string getValidPlace();

        /**
        * @brief Helper method that parses the google maps JSON and checks whether the address
        *        types will return a place name instead of a place address
        *
        * @param int j - array index for google maps JSON
        * @return bool that returns true if appropriate types and returns false in unwanted types
        */
        bool checkTypes(int j);

        /**
        * @brief Helper method that erases all instances of a particular string within another string
        *
        * @param mainStr string - main string
        * @param toErase string - sub string that will be erased from main string
        */
        void eraseAllSubStr(std::string & mainStr, const std::string & toErase);

        /**
        * @brief Helper method splits a string according to a
        *        delimiter and adds the resulting strings to a vector
        *
        * @param std::string& str - string that needs to be split
        * @param std::vector<std::string>& container - vector that will hold all the resulting strings
        * @param char delim - delimiter that splits the strings
        */
        void splitstr(const std::string& str, std::vector<std::string>& container, char delim);

	public:

        /**
        * @brief Construct a new Google API object that queries google maps API to
        *        reverse geocode a latitude and longitude to an address and
        *        returns results to a json formatted text
        *
        * @param std::string latitude - latitude that is used to reverse geocode
        * @param std::string longitude - longitude that is used to reverse geocode
        */
		googleAPI(std::string latitude, std::string longitude){
			
			lat = latitude;
        	lng = longitude;

			CURLplusplus client;


	        std::string latlng = lat + "," + lng;
	        std::string key = "&key=AIzaSyDhWjQTwKez6FFd4V6WOC8RCgNUUvIf22Y";

            //google maps api URL
	        std::string url = "https://maps.googleapis.com/maps/api/geocode/json?latlng=" +  latlng + key;

            //curl get request with google maps url
	        std::string x = client.Get(url);

            //convert resulting string into a json
	        doc = nlohmann::json::parse(x);

	        std::ofstream o("output.json");

        	o << doc << std::endl;
		}
         /**
         * @brief Destroy the Google API object
         *
         */
		~googleAPI(){
			//destructor
		}

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the formatted address
        *
        * @return string the formatted location address
        */
        std::string getFormattedLocation();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct place name, uses placesAPI if name not found in google maps API
        *
        * @return string the place name
        */
		std::string getName();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location latitude
        *
        * @return string the location latitude
        */
		std::string getLat();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location longitude
        *
        * @return string the location longitude
        */
        std::string getLng();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location north east latitude bound
        *
        * @return string the location north east latitude bound
        */
        std::string getNorthEastLat();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location north east longitude bound
        *
        * @return string the location north east longitude bound
        */
        std::string getNorthEastLng();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location south west latitude bound
        *
        * @return string the location south west latitude bound
        */
        std::string getSouthWestLat();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location south west longitude bound
        *
        * @return string the location south west longitude bound
        */
        std::string getSouthWestLng();

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location types of the first address in the JSON array
        *
        * @param int j - address index for google maps JSON array
        * @return string the location types
        */
        std::string getTypes(int j);

        /**
        * @brief Parses through the the JSON file returned from the google maps API call
        *        to get the correct location types of the first address in the JSON array
        *
        * @param int j - address index for google maps JSON array
        * @return string the location types that describe the location
        */
        std::string getPlaceID(int j);


};

#endif

