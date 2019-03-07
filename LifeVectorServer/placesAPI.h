/**
 * @file placesAPI.h
 * @author group 3
 * @brief header file for placesAPI class
 *
 * 
 * 
 */


#ifndef PLACESAPI_H
#define PLACESAPI_H

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <sstream>
#include "json.hpp"
#include <fstream>
#include "CURLplusplus.h"
#include <stdio.h>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>


/**
 * @brief header file for placesAPI class
 * 
 */
class placesAPI{

	private:

	    std::string placeid;
	    nlohmann::json doc;
	    std::string name;
        std::string location;
        long time;

	public:

        /**
        * @brief Construct a new places API object that queries google places API that
        *        takes a place ID of an address and returns place details of that address
        *        and puts results into a json formatted text
        *
        * @param std::string place_id - place id that represents an address, used to find place details
        */
		placesAPI(std::string place_id){

			placeid = place_id;

			CURLplusplus client;

	        std::string key = "&key=AIzaSyBJ6CP4wYrqVvOVq-ohJw-0aX6yU0mLu5c";

            //google places api URL
	        std::string url = "https://maps.googleapis.com/maps/api/place/details/json?placeid=" +  placeid + key;

            //curl get request with google places url
	        std::string x = client.Get(url);

            //convert resulting string into a json
	        doc = nlohmann::json::parse(x);

	        std::ofstream o("placesOutput.json");

        	o << doc << std::endl;
		}

        /**
         * @brief Destroy the places API object
         *
         */
		~placesAPI(){
		}

        /**
        * @brief Parses through the the JSON file returned from the google places API call
        *        to get the correct place name (if not found from maps API)
        *
        * @return string the place name
        */
	    std::string getLocationName();


};

#endif

