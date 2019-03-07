#include "ArchivedLocation.h"

// Constructors:
ArchivedLocation::ArchivedLocation() : details(0), coordinates(0.0, 0.0) {}

ArchivedLocation::ArchivedLocation(int locationID, double locationLatitude, double locationLongitude, double northPoint, double southPoint, double eastPoint, double westPoint) : details(locationID), coordinates(locationLatitude, locationLongitude)
{
    coordinates.setLimits(northPoint, southPoint, eastPoint, westPoint);
}

ArchivedLocation::ArchivedLocation(int locationID, double locationLatitude, double locationLongitude) : details(locationID), coordinates(locationLatitude, locationLongitude) {}

// Destructor
ArchivedLocation::~ArchivedLocation() {}

// Setters & Updaters:
void ArchivedLocation::setBasicInformation(int locationID, double locationLatitude, double locationLongitude)
{
    LocationInformation loc(locationID);
    details = loc;

    CoordinateInformation coord(locationLatitude, locationLongitude);
    coordinates = coord;
}

void ArchivedLocation::setLocationInformation(std::string name, std::string locationAddress, std::string locationDescription)
{
    details.setName(name);
    details.setAddress(locationAddress);
    details.setDescription(locationDescription);
}

void ArchivedLocation::setBoundaries(double north, double south, double east, double west)
{
    coordinates.setLimits(north, south, east, west);
}

void ArchivedLocation::updateName(std::string name)
{
    details.setName(name);
}
void ArchivedLocation::updateAddress(std::string address)
{
    details.setAddress(address);
}

void ArchivedLocation::updateDescription(std::string description)
{
    details.setDescription(description);
}

//Location Info Getters:
int ArchivedLocation::getID()
{
    return details.getID();
}

LocationInformation ArchivedLocation::getLocationDetails()
{
    return details;
}

double *ArchivedLocation::getLocationReference()
{
    double *gps = (double *)malloc(sizeof(double) * 2);
    *gps = coordinates.getLatitude();
    *(gps + 1) = coordinates.getLongitude();

    return gps;
}

CoordinateInformation ArchivedLocation::getCoordinateData()
{
    return coordinates;
}

// Printer
void ArchivedLocation::printInformation()
{
    std::cout << std::endl
              << "~~: Archived Location Information :~~" << std::endl
              << "Location ID: " << details.getID() << std::endl
              << "Location Name: " << details.getName() << std::endl
              << "Location Address: " << details.getAddress() << std::endl
              << "Location Description: " << details.getDescription() << std::endl
              << "Reference Coordinate: ( " << coordinates.getLatitude() << ", " << coordinates.getLongitude() << " )" << std::endl
              << "Boundary Limits:" << std::endl
              << "\tNorth: " << coordinates.getNorthLimit() << std::endl
              << "\tEast: " << coordinates.getEastLimit() << std::endl
              << "\tSouth: " << coordinates.getSouthLimit() << std::endl
              << "\tWest: " << coordinates.getWestLimit() << std::endl
              << std::endl;
}