/** 
 * @brief creates a user or sets values for existing users. Has getters and setters to accomplish this
 */
#include "user.h"

using namespace std;

/** constructor for new user so no report or report time
 * @param username the name of the user
 * @param deviceID the name of the device
 * @param hash the hash value generated and associated to the user
 * @param salt the salt value generated and associated to the userc
 */
User::User(std::string username, std::string deviceID, std::string hash, std::string salt)
{
    self_username = username;
    self_deviceID = deviceID;
    self_hash = hash;
    self_salt = salt;
    self_syncTime = 0;
    self_reportTime = 0;
    self_report = NULL;
}

/** 
 * @brief full constructor
 * @param username the name of the user
 * @param deviceID the name of the device
 * @param hash the hash value generated and associated to the user
 * @param salt the salt value generated and associated to the user
 * @param syncTime the time it takes to sync
 * @param report the json report
 */
User::User(std::string username, std::string deviceID, std::string hash, std::string salt, json report, int syncTime, int reportTime)
{
    self_username = username;
    self_deviceID = deviceID;
    self_hash = hash;
    self_salt = salt;
    self_report = report;
    self_syncTime = syncTime;
    self_reportTime = reportTime;
}

/** 
 * @brief barebones constructor
 * @param username the name of the user
 * @param deviceID the name of the device
 */
User::User(string username, string deviceID)
{
    self_username = username;
    self_deviceID = deviceID;
    self_hash = "";
    self_salt = "";
    self_report = "";
    self_syncTime = 0;
    self_reportTime = 0;
}

/** 
 * @brief destructor
 */
User::~User() {}

/** 
 * @brief returns the name of the user
 * @return self_username
 */
string User::getUsername()
{
    return self_username;
}
/** 
 * @brief returns the id of the device
 * @return self_deviceID
 */
string User::getDeviceID()
{
    return self_deviceID;
}
/** 
 * @brief returns the hash
 * @return self_hash
 */
string User::getHash()
{
    return self_hash;
}
/** 
 * @brief returns the salt
 * @return self_salt
 */
string User::getSalt()
{
    return self_salt;
}
/** 
 * @brief returns the report
 * @return self_report
 */
json User::getReport()
{
    return self_report;
}
/** 
 * @brief returns the sync time
 * @return self_syncTime
 */
long User::getSyncTime()
{
    return self_syncTime;
}
/** 
 * @brief returns the report time
 * @return self_reportTime
 */
long User::getReportTime()
{
    return self_reportTime;
}

/** 
 * @brief sets the username
 * @param username
 */
void User ::setUsername(string username)
{
    self_username = username;
}
/** 
 * @brief sets the device id
 * @param deviceID
 */
void User ::setDeviceID(string deviceID)
{
    self_deviceID = deviceID;
}
/** 
 * @brief sets the hash
 * @param hash
 */
void User ::setHash(string hash)
{
    self_hash = hash;
}
/** 
 * @brief sets the salt
 * @param salt
 */
void User ::setSalt(string salt)
{
    self_salt = salt;
}
/** 
 * @brief sets the report
 * @param report
 */
void User::setReport(json report)
{
    self_report = report;
}
/** 
 * @brief sets the sync time
 * @param syncTime
 */
void User ::setSyncTime(long syncTime)
{
    self_syncTime = syncTime;
}
/** 
 * @brief sets the report time
 * @param reportTime
 */
void User ::setReportTime(long reportTime)
{
    self_reportTime = reportTime;
}

