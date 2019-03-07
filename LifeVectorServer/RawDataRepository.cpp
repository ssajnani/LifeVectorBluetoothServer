#include "RawDataRepository.h"
#include <iostream>

/* Constructor */
RawDataRepository::RawDataRepository(std::string sourceFile, std::string fileSpecification)
{
    RawDataRepository::initJSON(sourceFile);

    RawDataRepository::buildRepository(fileSpecification);
}

//Testing constructor
RawDataRepository::RawDataRepository() {}

RawDataRepository::~RawDataRepository()
{
    //dtor
}

/* Helpers */

void RawDataRepository::initJSON(std::string source)
{
    std::ifstream iFile(source);
    jsonFile = nlohmann::json::parse(iFile);
}

void RawDataRepository::mapEntry(long time, RawDataRepository::gpsLog data)
{
    std::pair<long, RawDataRepository::gpsLog> entry(time, data);
    rawData.insert(entry);
}

void RawDataRepository::buildRepository(std::string fileSpecification)
{

    nlohmann::json gpsEntry;                                                      // json object to extract the individual coordinate logs
    std::string tLabel = "time_stamp", xLabel = "latitude", yLabel = "longitude"; // labels in json file identifying the information to be parsed
    int counter = 0;                                                              // iteration counter for traversing json object

    // initialize first entry and iterate through the gps log file
    gpsEntry = jsonFile.at(fileSpecification).at(counter);
    while (!gpsEntry.is_null())
    {
        // initialize struct for entry and insert the relevant information
        long timestamp;
        RawDataRepository::gpsLog newEntry;

        timestamp = gpsEntry.at(tLabel).get<long>();

        newEntry.time = timestamp;
        newEntry.latitude = gpsEntry.at(xLabel).get<double>();
        newEntry.longitude = gpsEntry.at(yLabel).get<double>();

        // insert entry into raw data vector
        logTimes.push_back(timestamp);
        mapEntry(timestamp, newEntry);

        // increment counter and grab next entry
        counter++;
        if ((signed int)jsonFile.at(fileSpecification).size() > counter)
            gpsEntry = jsonFile.at(fileSpecification).at(counter);
        else
            break;
    }
}

/* Fetchers */

void RawDataRepository::dumpData()
{
    std::vector<long>::const_iterator dataIterator;
    std::cout << "Entries:" << std::endl;
    for (dataIterator = logTimes.begin(); dataIterator != logTimes.end(); ++dataIterator)
    {
        gpsLog entry = rawData.at(*dataIterator);
        std::cout.precision(12);
        std::cout << entry.time << " - "
                  << "(" << entry.latitude << ", " << entry.longitude << ")" << std::endl;
    }
}

std::vector<long> RawDataRepository::getTimeStamps()
{
    return logTimes;
}

void RawDataRepository::getCoordinates(double *coordinates, long timestamp)
{
    gpsLog entry = rawData.at(timestamp);
    *coordinates = entry.latitude;
    *(coordinates + 1) = entry.longitude;
}

void RawDataRepository::addTestData(long t, double lat, double lng)
{
	gpsLog log;
	log.latitude = lat;
	log.longitude = lng;
	log.time = t;
	rawData[t] = log;
	logTimes.push_back(t);
}