#include <iostream>
#include <sstream>
#include <functional>
#include <algorithm>
#include "UserLibrary.h"
#include "user.h"
#include "hashfunc.h"
using namespace std;
using json = nlohmann::json;

/*
 User Table Creating Query
 CREATE TABLE User(
 username CHAR(10) NOT NULL,
 deviceID CHAR(10) NOT NULL,
 hash CHAR(128),
 salt CHAR(128),
 report JSON,
 syncTime BIGINT,
 reportTime BIGINT,
 PRIMARY KEY (username,deviceID)
 );
 */

//Building connection to LifeVector database
UserLibrary::UserLibrary(std::string userName, std::string password)
{
    db.initDB("localhost", userName, password, "LifeVector");
}

//Print all user information from database
bool UserLibrary::printAllUsers()
{
    string sql = "SELECT * FROM User;";

    if (db.exeSQL(sql))
    {
        return true;
    }
    else
    {
        cout << "Error: Failed to display user" << endl;
        return false;
    }
}

//Store the user information into database
bool UserLibrary::createUserInDB(User user)
{
    string username = user.getUsername();
    string deviceID = user.getDeviceID();
    string hash = user.getHash();
    string salt = user.getSalt();
    json report = user.getReport();
    int syncTime = user.getSyncTime();
    int reportTime = user.getReportTime();

    if (!isRegistered(username, deviceID))
    {
        stringstream ss;
        ss << "INSERT INTO User VALUES ('"
           << username << "','"
           << deviceID << "','"
           << hash << "','"
           << salt << "','"
           << report << "',"
           << syncTime << ","
           << reportTime << ");";
        string sql = ss.str();

        if (db.exeSQL(sql))
        {
            cout << "User created" << endl;
            return true;
        }
        else
        {
            cout << "Fail: User not Created" << endl;
            return false;
        }
    }
    else
        return false;
}

//Delete user information from database, based on username and deviceID to search
bool UserLibrary::deleteUserFromDB(string username, string deviceID)
{
    if (isRegistered(username, deviceID))
    {
        stringstream ss;
        ss << "DELETE FROM User WHERE deviceID = '" << deviceID
           << "' AND username = '" << username << "';";

        string sql = ss.str();
        if (db.exeSQL(sql))
        {
            cout << "User deleted" << endl;
            return true;
        }
        else
        {
            cout << "Fail: Could not delete user from Database" << endl;
            return false;
        }
    }
    else
        return false;
}

//Get user object from database based on username and deviceID
bool UserLibrary::retrieveUserFromDB(User *user, std::string username, std::string deviceID)
{
    stringstream ss;
    if (isRegistered(username, deviceID))
    {
        // retrieve salt
        ss.str("");
        ss << "SELECT salt FROM User WHERE deviceID = '" << deviceID
           << "' AND username = '" << username << "';";
        string sql = ss.str();
        string salt = db.getSQLResult(sql);

        // retrieve report
        ss.str("");
        ss << "SELECT report FROM User WHERE deviceID = '" << deviceID << "' AND username = '" << username << "';";
        sql = ss.str();
        json report;
        ss.str("");
        ss << db.getSQLResult(sql);
        ss >> report;

        // retireve last sync time
        ss.str("");
        ss << "SELECT syncTime FROM User WHERE deviceID = '" << deviceID << "' AND username = '" << username << "';";
        sql = ss.str();
        long syncTime = atol(db.getSQLResult(sql).c_str());

        // retrieve last report generation time
        ss.str("");
        ss << "SELECT reportTime FROM User WHERE deviceID = '" << deviceID << "' AND username = '" << username << "';";
        sql = ss.str();
        long reportTime = atol(db.getSQLResult(sql).c_str());
        
        // retrieve hash
        ss.str("");
        ss << "SELECT hash FROM User WHERE deviceID = '" << deviceID
        << "' AND username = '" << username << "';";
        sql = ss.str();
        string hash = db.getSQLResult(sql);

        

        // insert retrieved information into User object
        (*user).setUsername(username);
        (*user).setDeviceID(deviceID);
        (*user).setSalt(salt);
        (*user).setHash(hash);
        (*user).setReport(report);
        (*user).setSyncTime(syncTime);
        (*user).setReportTime(reportTime);
        
        return true;
    }
    else
    {
        return false;
    }
}

//Update user's last synchronization time
bool UserLibrary::updateUserSyncTime(User user, int syncTime)
{
    if (isRegistered(user.getUsername(), user.getDeviceID()))
    {
        stringstream ss;
        ss << "UPDATE User SET syncTime = " << syncTime
           << " WHERE deviceID = '" << user.getDeviceID()
           << "' AND username = '" + user.getUsername() + "';";

        string sql = ss.str();

        if (db.exeSQL(sql))
        {
            cout << "SyncTime updated" << endl;
            return true;
        }
        else
        {
            cout << "Fail: Could not update syncTime" << endl;
            return false;
        }
    }
    else
        return false;
}

//Update the user's report content
bool UserLibrary::updateReport(User user, json report)
{
    if (isRegistered(user.getUsername(), user.getDeviceID()))
    {
        stringstream ss;
        ss << "UPDATE User SET report = '" << report
           << "' WHERE deviceID = '" << user.getDeviceID()
           << "' AND username = '" << user.getUsername() << "';";

        string sql = ss.str();

        if (db.exeSQL(sql))
        {
            cout << "Report updated" << endl;
            return true;
        }
        else
        {
            cout << "Fail: Could not update report" << endl;
            return false;
        }
    }
    else
        return false;
}

bool UserLibrary::uploadRawGPSData(std::string username, std::string deviceID, RawDataRepository rawDataSet)
{
    DataDepositor uploader = DataDepositor(&db);

    if (!isPresent(deviceID))
    {
        cout << "Warning: User/Device not present in Database, Please register device!" << endl;
        return false;
    }

    return uploader.depositRaw(username, deviceID, rawDataSet);
}

bool UserLibrary::isPresent(std::string deviceID)
{
    stringstream ss;
    ss << "SELECT username FROM User WHERE deviceID = '" << deviceID << "';";

    string sql = ss.str();
    string result = db.getSQLResult(sql);

    //Check if user exists
    if (result.length() < 3)
    {
        // cout << "Cannot find user" << endl;
        return false;
    }
    else
    {
        // cout << result << endl;
        return true;
    }
}

bool UserLibrary::isRegistered(std::string username, std::string deviceID)
{
    stringstream ss;
    ss << "SELECT * FROM User WHERE deviceID = '" << deviceID
       << "' AND username = '" << username << "';";
    string sql = ss.str();

    string result = db.getSQLResult(sql);

    //Check if user exists
    if (result.length() < 3)
    {
        cout << "User and Device queried not in Database" << endl;
        return false;
    }
    else
    {
        cout << "User found in Database" << endl;
        return true;
    }
}

Database *UserLibrary::getDBConnection()
{
    return &db;
}