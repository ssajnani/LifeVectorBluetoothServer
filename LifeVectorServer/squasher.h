/**
 * @file squasher.h
 * @brief Squasher to process raw data
 * Used for processing raw data from front end, which recording
 * the time stamps, latitude, and longitude. Adding unfound location 
 * into archived library and squash continuous points at same 
 * location together
 * @author Yimin Xu (yxu537@uwo.ca)
 * 
 
 */

#ifndef SQUASHER_H
#define SQUASHER_H

#include "RawDataRepository.h"
#include "ArchiveLibrary.h"
#include "Database.h"

/**
 * @brief Squasher to process raw data
 * Used for processing raw data from front end, which recording
 * the time stamps, latitude, and longitude. Adding unfound location 
 * into archived library and squash continuous points at same 
 * location together * 
 */
class squasher{

private:

	int currentID;
	RawDataRepository rawData;
	ArchiveLibrary library;
	std::string username;
	std::string deviceID;

	/**
	 * @brief Increment static ID
	 */
	void incrementID() {
		currentID++;
	}

public:

	/**
	 * @brief Construct a new squasher object
	 * 
	 * @param data 
	 * @param db_pointer 
	 * 
	 */
	squasher(RawDataRepository data, Database *db_pointer,std::string user,std::string devID) : library(db_pointer){
		rawData = data;	
		currentID = library.getLastLocationID() + 1;
		username = user;
		deviceID = devID;
	}

	/**
	 * @brief Squash unprocessed data
	 * 
	 */
	void squash();
};

#endif
