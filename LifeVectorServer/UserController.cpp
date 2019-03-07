#include "UserController.h"

using namespace std;

// Constructor
UserController::UserController(std::string dbUsername, std::string dbPassword) : userLibrary(dbUsername, dbPassword) {}

// Destructor
UserController::~UserController() {}

/* User Functions */
bool UserController::createUser(string username, string devID, string password)
{
	// Test if User already in database
	if (userLibrary.isRegistered(username, devID))
	{
		//the user is found in database
		cout << "User Creation Failed: User is already registered in Database" << endl;
		return false;
	}

	// hash and salt password
	hashfunc hf;
	hf.hashString(password);
	string hash = hf.getHash();
	string salt = hf.getSalt();

	// create user object
	User newUser = User(username, devID, hash, salt);

	// save user to DB
	return userLibrary.createUserInDB(newUser);
}

bool UserController::retrieveUser(User *target, std::string username, std::string devID)
{
	if (userLibrary.retrieveUserFromDB(target, username, devID))
	{
		return true;
	}
	else
		return false;
}

bool UserController::deleteUser(std::string username, std::string devID)
{
	return userLibrary.deleteUserFromDB(username, devID);
}

/* User Information Editors */
bool UserController::updateReport(string username, string deviceID, json newReport)
{
	User *target = new User(username,deviceID);
	if (retrieveUser(target, username, deviceID))
	{
		return userLibrary.updateReport(*target, newReport);
	}
	else
		return false;
}

bool UserController::fetchReport(json *dbReport, string username, string deviceID)
{
	User *target = new User(username,deviceID);

	if (retrieveUser(target, username, deviceID))
	{
		*dbReport = (*target).getReport();

		// Check if report is empty
		if (dbReport->empty())
		{
			cout << "Warning: No reports stored in Database" << endl;
			return false;
		}

		return true;
	}
	else
		return false;
}

//Get the hash value user's input password and stored salt, and compare it with the stored hash, return true if matches
bool UserController::compareUserHash(std::string username, std::string deviceID, std::string password)
{
	User *target = new User(username,deviceID);
	if (retrieveUser(target, username, deviceID))
	{
		// store hash from DB
		string dbHash = target->getHash();

		// initialize hash function for password parameter
		hashfunc hf;
		string inputHash = hf.getHash(password, target->getSalt());

		// Remove the ******* white space and \n from hash strings
		dbHash.erase(std::remove(dbHash.begin(), dbHash.end(), ' '), dbHash.end());
		dbHash.erase(std::remove(dbHash.begin(), dbHash.end(), '\n'), dbHash.end());

		inputHash.erase(std::remove(inputHash.begin(), inputHash.end(), ' '), inputHash.end());
		inputHash.erase(std::remove(inputHash.begin(), inputHash.end(), '\n'), inputHash.end());

		// Print input hash and database hash - check
		cout << dbHash << endl
			 << inputHash << endl;

		// Compare and return result
		if (dbHash.compare(inputHash) == 0)
			return true;
		else
			return false;
	}
	else
		return false;
}

bool UserController::saveRawGPStoDB(std::string username, std::string deviceID, RawDataRepository rawDataSet){
	if(userLibrary.uploadRawGPSData(username, deviceID, rawDataSet))
	{
		cout << "Raw GPS data saved to Database" << endl;
		return true;
	}
	else return false;
}

Database *UserController::getDBConnection(){
	return userLibrary.getDBConnection();
}