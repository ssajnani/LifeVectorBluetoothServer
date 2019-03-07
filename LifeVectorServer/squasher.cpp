/**
 * @file squasher.cpp
 * @brief Squasher to process raw data
 * 
 * Used for processing raw data from front end, which recording
 * the time stamps, latitude, and longitude. Adding unfound location 
 * into archived library and squash continuous points at same 
 * location together
 */

#include "squasher.h" 
#include <vector>
#include <sstream>
#include "CoordinateInformation.h"
#include "ArchivedLocation.h"
#include "googleAPI.h"
#include "UserVisitInfo.h"
#include <iomanip> 

/**
 * @brief Squash unprocessed data
 * 
 * Recording the time spent in each location
 * Save the result into ArchiveLibrary
 */
void squasher::squash() {
	std::vector<long> timeStamps = rawData.getTimeStamps();
	std::map<long, int> log;
	std::vector<long>::iterator itr = timeStamps.begin();

	//Loop through all the data points
	for (itr; itr != timeStamps.end(); itr++) {
		double *coord = (double*) malloc(sizeof(double) * 2);

		rawData.getCoordinates(coord, *itr);
		double lat = *coord;
		double lng = *(coord + 1);

		//Search the location in the library based on coordinates, get 0 if not found
		std::map<int, CoordinateInformation> matchedLocations = std::map<int, CoordinateInformation>();
		int locationID = library.matchNearestLocation(matchedLocations,lat,lng);
		
		if (locationID > 0){
			//Location is found in library
			//Push it into map for later squashing
			log.emplace(*itr,locationID);
		}
		else {
			//Location is not in library
			//Search location from Google Map and Google Place API
			locationID = currentID;

			googleAPI gAPI(std::to_string(lat),std::to_string(lng));
			
			//Setting up the location information
			int id = currentID;
			std::string name = gAPI.getName();
			std::string address = gAPI.getFormattedLocation();
			std::string description = gAPI.getTypes(0);
			double eastbound = std::stod(gAPI.getNorthEastLng());
			double northbound = std::stod(gAPI.getNorthEastLat());
			double westbound = std::stod(gAPI.getSouthWestLng());
			double southbound = std::stod(gAPI.getSouthWestLat());

			//Build the ArchiveLocation object and save it into library
			ArchivedLocation location = library.constructLocation(id,name,address,description,lat,lng,
			northbound,southbound,eastbound,westbound);
			library.saveLocationToDatabase(location);

			//Push into map for later squashing
			log.emplace(*itr, locationID);
			incrementID();
		}
	}

	//Another loop to squash the points
	double lastTime = 0;
	double timeSpent = 0;
	int lastID = 0;
	itr = timeStamps.begin();
	for (itr; itr != timeStamps.end(); itr++) {
		UserVisitInfo uvi;
		int locID = log[*itr];
		//Setting up for the first location
		if (itr == timeStamps.begin()) {
			lastTime = *itr;
			lastID = locID;
			if ((itr + 1) == timeStamps.end()){ //If only one input, push to library
				uvi.addSingleLog(*itr, 0);
				library.archiveUserLog(locID,username,deviceID,uvi);
			}
		}
		else {
			//If found a different location, 
			//add the last location to the user visit info
			if (locID != lastID) {
				timeSpent = *itr - lastTime;

				uvi.addSingleLog(lastTime, (int)timeSpent);
				library.archiveUserLog(lastID,username,deviceID,uvi);

				lastTime = *itr;
				lastID = locID;
				timeSpent = 0;
			}
			//If location is the same with last one, do nothing

			//If it's the last location, also add it to the user visit info
			if ((itr + 1) == timeStamps.end()) {
				timeSpent = *itr - lastTime;

				UserVisitInfo uvi0; //Create a new user visit info to avoid conflict with the last one
				uvi0.addSingleLog(lastTime, (int)timeSpent);
				library.archiveUserLog(locID,username,deviceID,uvi0);
				timeSpent = 0;
			}
		}
	}
}