#include "ArchiveLibrary.h"

/* Initialize Archive Library. Done through a connected DB pointed */
ArchiveLibrary::ArchiveLibrary(Database *db_pointer)
{
    connected_db = db_pointer;
}

/* Destructor: */
ArchiveLibrary::~ArchiveLibrary() {}

/* Helpers: */
std::string ArchiveLibrary::composeID(std::string username, std::string deviceID)
{
    std::string combinationSymbol = "/", userID;
    userID.append(username);
    userID.append(combinationSymbol);
    userID.append(deviceID);

    return userID;
}

void ArchiveLibrary::userLogDBtoObj(std::map<int, UserVisitInfo> *userLog, std::string results)
{
    std::vector<std::string> entries, fields;

    // separate each entry to interate through
    StringParser::custom_parse(results, entries, '\n');
    std::vector<std::string>::iterator row = entries.begin();

    for (row; row != entries.end(); ++row)
    {
        // split entry to individual fields
        StringParser::custom_parse(*row, fields, '\t');

        // check if location is in userLog already
        std::map<int, UserVisitInfo>::iterator target;
        target = userLog->find(std::stoi(fields[1]));

        if (target != userLog->end()) // location already in userLog
        {
            UserVisitInfo *visits = &(target->second);
            visits->addSingleLog(std::stod(fields[0]), std::stoi(fields[2]));
        }
        else // create new UserVisitInfo for entry and add to userLog
        {
            UserVisitInfo newVisits;
            newVisits.addSingleLog(std::stod(fields[0]), std::stoi(fields[2]));

            userLog->emplace(std::stoi(fields[1]), newVisits);
        }
    }
}

void ArchiveLibrary::locationLogDBtoOBJ(VisitLog *record, std::string results)
{
    // Split into entries (rows)
    std::vector<std::string> entries, fields;
    StringParser::custom_parse(results, entries, '\n');
    std::vector<std::string>::iterator row = entries.begin();

    for (row; row != entries.end(); ++row)
    {
        // Split into indiviual fields
        StringParser::custom_parse(*row, fields, '\t');
        std::string uID = composeID(fields[3], fields[4]);

        // check for user in record
        if (record->userFound(uID))
        {
            // add entry
            record->addSingleEntry(uID, std::stod(fields[0]), std::stoi(fields[2]));
        }
        else
        {
            // insert new user and log into record
            record->addNewUser(uID);
            record->addSingleEntry(uID, std::stod(fields[0]), std::stoi(fields[2]));
        }
    }
}

/* Location Functions: */
ArchivedLocation ArchiveLibrary::constructLocation(int id, std::string name, std::string address, std::string description, double latitude, double longitude, double northBound, double southBound, double eastBound, double westBound)
{
    ArchivedLocation location(id, latitude, longitude, northBound, southBound, eastBound, westBound);

    location.setLocationInformation(name, address, description);

    return location;
}

bool ArchiveLibrary::saveLocationToDatabase(ArchivedLocation location)
{
    if (isKnown(location.getID()))
    {
        std::cout << "Save to Database Failed: Location already in Database." << std::endl;
        return false;
    }

    std::stringstream command;
    double *gps = location.getLocationReference();

    command << std::setprecision(12) << "INSERT INTO ArchivedLocations "
            << "(locationID, locationName, address, latitudePosition, longitudePosition, northBound, southBound, eastBound, westBound, description) VALUES ("
            << location.getID() << ", '"
            << location.getLocationDetails().getName() << "', '"
            << location.getLocationDetails().getAddress() << "', "
            << *gps << ", " << *(gps + 1) << ", "
            << location.getCoordinateData().getNorthLimit() << ", "
            << location.getCoordinateData().getSouthLimit() << ", "
            << location.getCoordinateData().getEastLimit() << ", "
            << location.getCoordinateData().getWestLimit() << ", '"
            << location.getLocationDetails().getDescription() << "');";

    if (connected_db->exeSQL(command.str()))
    {
        std::cout << "New Location, " << location.getLocationDetails().getName() << ", has been added to Database" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Location could not save to Database" << std::endl;
        return false;
    }
}

ArchivedLocation *ArchiveLibrary::getLocationFromDatabase(int id)
{
    if (isKnown(id))
    {
        std::stringstream query;
        std::string result;

        // Fetch Full Location Entry
        query << "SELECT * FROM ArchivedLocations WHERE locationID = " << id << ";";
        result = connected_db->getSQLResult(query.str());

        // Parse result into vector
        std::vector<std::string> fields;
        StringParser::custom_parse(result, fields, '\t');

        // std::cout << result << std::endl;

        /* Field Legend : 
         * 0. locationID, 
         * 1. locationName, 
         * 2. address, 
         * 3. latitudePosition, 
         * 4. longitudePosition, 
         * 5. northBound, 
         * 6. southBound, 
         * 7. eastBound, 
         * 8. westBound, 
         * 9. description
         */

        // Create Archive Location

        /* for (std::vector<std::string>::iterator i = fields.begin(); i != fields.end(); ++i)
        {
            std::cout << *i << std::endl;
        } */

        int l_id = std::stoi(fields[0]);
        std::string name = fields[1],
                    address = fields[2],
                    descr = fields[9];
        double lat = std::stod(fields[3]),
               lng = std::stod(fields[4]),
               n = std::stod(fields[5]),
               s = std::stod(fields[6]),
               e = std::stod(fields[7]),
               w = std::stod(fields[8]);

        // std::cout << name << std::endl;

        ArchivedLocation *location = new ArchivedLocation(l_id, lat, lng, n, s, e, w);
        location->setLocationInformation(name, address, descr);

        // location->printInformation();

        std::cout << "Location retrieved successfully from Database" << std::endl;
        return location;
    }
    else
    {
        std::cout << "No Location at id: " << id << std::endl;
        return NULL;
    }
}

bool ArchiveLibrary::getAdjacentLocations(std::map<int, CoordinateInformation> &matchedLocations, double latitude, double longitude)
{
    std::stringstream query;
    std::string results;
    std::vector<std::string> locations, coordinates;
    std::vector<std::string>::iterator row;

    query << std::setprecision(12) << "SELECT locationID, latitudePosition, longitudePosition, northBound, southBound, eastBound, westBound FROM ArchivedLocations WHERE northBound >= " << latitude << " AND southBound <= " << latitude << " AND eastBound >= " << longitude << " AND westBound <= " << longitude << ";";
    results = connected_db->getSQLResult(query.str());

    StringParser::custom_parse(results, locations, '\n');

    if (locations.size() < 1)
    {
        return false;
    }

    row = locations.begin();
    for (row; row != locations.end(); ++row)
    {
        StringParser::custom_parse(*row, coordinates, '\t');

        CoordinateInformation match(std::stod(coordinates[1]), std::stod(coordinates[2]));
        match.setLimits(std::stod(coordinates[3]), std::stod(coordinates[4]), std::stod(coordinates[5]), std::stod(coordinates[6]));

        matchedLocations.emplace(std::stoi(coordinates[0]), match);
    }

    if (matchedLocations.size() < 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int ArchiveLibrary::matchNearestLocation(std::map<int, CoordinateInformation> &matchedLocations, double latitude, double longitude)
{
    if (matchedLocations.size() < 1)
    {
        getAdjacentLocations(matchedLocations, latitude, longitude);
    }

    int closestMatch = 0;
    double proximity = 1000.00;
    std::map<int, CoordinateInformation>::iterator matches = matchedLocations.begin();

    if (matchedLocations.size() < 2)
    {
        closestMatch = matches->first;
        return closestMatch;
    }

    for (matches; matches != matchedLocations.end(); ++matches)
    {
        double x_diff, y_diff, current_prox;

        x_diff = abs(latitude - matches->second.getLatitude());
        y_diff = abs(longitude - matches->second.getLongitude());

        current_prox = sqrt(pow(x_diff, 2.0) * pow(y_diff, 2.0));

        if (current_prox < proximity)
        {
            proximity = current_prox;
            closestMatch = matches->first;
        }
    }

    return closestMatch;
}

/* Location Visit Records Functions: */
bool ArchiveLibrary::archiveUserLog(int locationID, std::string user, std::string device, UserVisitInfo userVisitLog)
{
    // check for presence of location
    if (!isKnown(locationID))
    {
        std::cout << "Could not archive user log: Location not known" << std::endl;
        return false;
    }

    bool check = false;

    // extract iterator of user's time log
    std::map<long, int> log = userVisitLog.getTimeLog();
    std::map<long, int>::iterator record_iter = log.begin();

    // iterate through log and enter each entry into DB
    for (; record_iter != log.end(); record_iter++)
    {
        /* Fields Legend : VisitLog
        `visitTime` bigint(20) NOT NULL,
        `locationID` int(11) NOT NULL,
        `duration` int(11) NOT NULL,
        `username` varchar(10) NOT NULL,
        `deviceID` varchar(10) NOT NULL,
        */

       // Check if the time stamp is duplicate
        std::stringstream checkQuery;
        checkQuery << "SELECT * FROM VisitLog WHERE visitTime = " << record_iter->first
                   << ";";
        std::string checkResult = connected_db->getSQLResult(checkQuery.str());
        if (checkResult.length() >= 1){
            std::cout << "Duplicate time in visit log" << std::endl;
            return false; //If time exists, return false
        }

        std::stringstream entry;
        entry << "INSERT INTO VisitLog"
              << " (visitTime, locationID, duration, username, deviceID) VALUES ("
              << record_iter->first << ", "
              << locationID << ", "
              << record_iter->second << ", '"
              << user << "', '"
              << device << "');";

        if (connected_db->exeSQL(entry.str()))
        {
            SystemTimeManager t(record_iter->first);
            std::cout << "User Record for " << t.UNIXtoEST() << " at LocID: " << locationID << " added to Database" << std::endl;
            check = true;

            // std::cout << "check" << std::endl;
        }
        else
        {
            std::cout << "User Record could not save to Database" << std::endl;
            check = false;
            break;
        }
    }

    // std::cout << "returning" << std::endl;
    return check;
}

std::map<int, UserVisitInfo> *ArchiveLibrary::getUserLogFromDatabase(std::string user, std::string device)
{
    std::map<int, UserVisitInfo> *userLog = new std::map<int, UserVisitInfo>();

    if (!isRegistered(user, device))
    {
        return NULL;
    }

    std::stringstream query;
    query << "SELECT * FROM VisitLog WHERE username = '"
          << user << "' AND deviceID = '" << device << "';";

    std::string results = connected_db->getSQLResult(query.str());

    userLogDBtoObj(userLog, results);

    return userLog;
}

std::map<int, UserVisitInfo> *ArchiveLibrary::getUserLogBetween(std::string user, std::string device, long start, long fin)
{
    std::map<int, UserVisitInfo> *userLog = new std::map<int, UserVisitInfo>();

    if (!isRegistered(user, device))
    {
        return NULL;
    }

    std::stringstream query;
    query << "SELECT * FROM VisitLog WHERE username = '"
          << user << "' AND deviceID = '" << device
          << "' AND visitTime BETWEEN " << start << " AND " << fin << ";";

    std::string results = connected_db->getSQLResult(query.str());

    userLogDBtoObj(userLog, results);

    return userLog;
}

VisitLog *ArchiveLibrary::getLocationRecordFromDatabase(int locationID)
{
    // VisitLog initialization
    VisitLog *record = new VisitLog();

    if (!isKnown(locationID))
    {
        return NULL;
    }

    // Variable Declaration
    std::stringstream query;

    // Query database for entires related to locations
    query << "SELECT * FROM VisitLog WHERE locationID = " << locationID << ";";
    std::string results = connected_db->getSQLResult(query.str());

    locationLogDBtoOBJ(record, results);

    return record;
}

VisitLog *ArchiveLibrary::getLocationRecordFBetween(int locationID, long start, long fin)
{
    // VisitLog initialization
    VisitLog *record = new VisitLog();

    if (!isKnown(locationID))
    {
        return NULL;
    }

    // Variable Declaration
    std::stringstream query;

    // Query database for entires related to locations
    query << "SELECT * FROM VisitLog WHERE locationID = " << locationID
          << " AND visitTime BETWEEN " << start << " AND " << fin << ";";
    std::string results = connected_db->getSQLResult(query.str());

    locationLogDBtoOBJ(record, results);

    return record;
}

int ArchiveLibrary::getVisitCount(int locationID)
{
    std::stringstream query;
    query << "SELECT count(*) FROM VisitLog WHERE locationID = " << locationID << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getVisitCount(int locationID, std::string user, std::string device)
{
    std::stringstream query;
    query << "SELECT count(*) FROM VisitLog WHERE locationID = " << locationID
          << " AND username = '" << user << "' AND deviceID = '" << device << "';";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getCountBetween(int locationID, long start, long fin)
{
    std::stringstream query;
    query << "SELECT count(*) FROM VisitLog WHERE locationID = " << locationID
          << " AND visitTime BETWEEN " << start << " AND " << fin << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getCountBetween(int locationID, std::string user, std::string device, long start, long fin)
{
    std::stringstream query;
    query << "SELECT count(*) FROM VisitLog WHERE locationID = " << locationID
          << " AND username = '" << user << "' AND deviceID = '" << device
          << "' AND visitTime BETWEEN " << start << " AND " << fin << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getTotalVisitCount()
{
    std::stringstream query;
    query << "SELECT count(*) FROM VisitLog;";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getTotalCountBetween(long start, long fin)
{
    std::stringstream query;
    query << "SELECT count(*) FROM VisitLog WHERE"
          << " visitTime BETWEEN " << start << " AND " << fin << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getDurationAtLocation(int locationID)
{
    std::stringstream query;
    query << "SELECT sum(duration) FROM VisitLog WHERE locationID = " << locationID << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getDurationAtLocation(int locationID, std::string user, std::string device)
{
    std::stringstream query;
    query << "SELECT sum(duration) FROM VisitLog WHERE locationID = " << locationID
          << " AND username = '" << user << "' AND deviceID = '" << device << "';";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getDurationBetween(int locationID, long start, long fin)
{
    std::stringstream query;
    query << "SELECT sum(duration) FROM VisitLog WHERE locationID = " << locationID
          << " AND visitTime BETWEEN " << start << " AND " << fin << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getDurationBetween(int locationID, std::string user, std::string device, long start, long fin)
{
    std::stringstream query;
    query << "SELECT sum(duration) FROM VisitLog WHERE locationID = " << locationID
          << " AND username = '" << user << "' AND deviceID = '" << device
          << "' AND visitTime BETWEEN " << start << " AND " << fin << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getTotalDuration()
{
    std::stringstream query;
    query << "SELECT sum(duration) FROM VisitLog;";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

int ArchiveLibrary::getTotalDurationBetween(long start, long fin)
{
    std::stringstream query;
    query << "SELECT sum(duration) FROM VisitLog WHERE"
          << " visitTime BETWEEN " << start << " AND " << fin << ";";
    std::string results = connected_db->getSQLResult(query.str());

    return std::stoi(results);
}

/* Checks */
bool ArchiveLibrary::isKnown(int locationID)
{
    std::stringstream query;
    query << "SELECT locationID FROM ArchivedLocations WHERE locationID = " << locationID << ";";
    std::string result = connected_db->getSQLResult(query.str());

    //Check if location exists
    if (result.length() < 2)
    {
        std::cout << "Location is not Known" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Location is Known in Database" << std::endl;
        return true;
    }
}

bool ArchiveLibrary::isRegistered(std::string user, std::string device)
{
    std::stringstream query;
    query << "SELECT locationID FROM ArchivedLocations WHERE username = '" << user
          << "' AND deviceID = '" << device << "';";
    std::string result = connected_db->getSQLResult(query.str());

    //Check if location exists
    if (result.length() < 2)
    {
        std::cout << "User is not in Database" << std::endl;
        return false;
    }
    else
    {
        std::cout << "User is in Database" << std::endl;
        return true;
    }
}

/**
 * @brief Get the Last Location ID
 * 
 * @return int 
 */
int ArchiveLibrary::getLastLocationID()
{
    std::stringstream query;
    query << "SELECT MAX(locationID) FROM ArchivedLocations;";

    std::string result = connected_db->getSQLResult(query.str());

    //Check if table is empty
    if (result.length() < 1)
        return 0;
    else
        return std::stoi(result);
}