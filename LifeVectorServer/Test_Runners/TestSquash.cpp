/**
 * @file TestSquash.cpp
 * 
 * @brief Testing class for squasher
 * 
 * Mannually input raw data repository into squasher
 * Connecting to Archive Library
 */

#include "../googleAPI.h"
#include "../squasher.h"
#include <iomanip>
#include "../RawDataRepository.h"
#include "../Database.h"
#include "../UserController.h"

using namespace std;

int main(int argc, char** argv) {

	cout << "Squasher Algorothm Test: " << endl;

	//Create user in database for testing
	UserController uc("server","LifeVector123");
	
	cout << "Create Test User..." << endl;
	uc.createUser("Yimin","01","LifeVector123");
	cout << "Test User Created" << endl;

	//Create database object and Raw data repository	
	Database db;
	db.initDB("localhost","server","LifeVector123","LifeVector");
	RawDataRepository rdr("./SampleFiles/sample_coordinates.json","gps_coordinates_dump");
	rdr.dumpData();

	
	//Create squasher
	
	cout << "Squash sample raw dataset" <<endl;
	squasher sq(rdr, &db,"Yimin","01");
	sq.squash();
	
	cout << "Squash successful." << endl;
	return 0;
}

