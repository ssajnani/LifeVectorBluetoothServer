#include "DataDepositor.h"
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision

DataDepositor::DataDepositor(Database *db_pointer)
{
    db_link = db_pointer;
}

bool DataDepositor::depositRaw(std::string username, std::string deviceID, RawDataRepository dataset)
{
    // fetch time log of all the GPS points
    std::vector<long> timeLog = dataset.getTimeStamps();

    // initialize bool checker
    bool isUploaded = false;

    // initialize iterator for time log and insert each entry into database
    std::vector<long>::const_iterator i;
    for (i = timeLog.begin(); i != timeLog.end(); ++i)
    {
        // set up SQL command to insert entry
        double gpsXY[2];
        long timeUNIX = *i;
        dataset.getCoordinates(gpsXY, timeUNIX);
        std::string insertCommand = concatRawQuery(timeUNIX, gpsXY[0], gpsXY[1], username, deviceID);

        // insert into database
        if ((*db_link).exeSQL(insertCommand))
        {
            isUploaded = true;
        }
        else
        {
            isUploaded = false;
            break;
        }
    }

    // Print Closing Message & return
    if (isUploaded)
        std::cout << "Raw GPS Data Successfully Uploaded to Database." << std::endl;
    else
        std::cout << "Raw GPS Data Upload Failed." << std::endl;

    return isUploaded;
}

std::string DataDepositor::concatRawQuery(long timestamp, double latitude, double longitude, std::string user, std::string device)
{
    std::stringstream output;
    std::string tValue, xValue, yValue;
    tValue = std::to_string(timestamp);
    xValue = std::to_string(latitude);
    yValue = std::to_string(longitude);

    output << std::setprecision(12) 
           << "INSERT INTO GPSUnprocessed (timeStamp, latitude, longitude, username, deviceID) VALUES (" << tValue << ", "
           << xValue << ", " << yValue << ", '"
           << user << "', '" << device + "');";

    return output.str();
}

DataDepositor::~DataDepositor()
{
    //Destroy
}
