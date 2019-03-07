#include "CoordinateInformation.h"

// Constructor:
CoordinateInformation::CoordinateInformation(double latitudeReference, double longitudeReference)
{
    latitude = latitudeReference;
    longitude = longitudeReference;
}

// Destructor:
CoordinateInformation::~CoordinateInformation() {}

// Setters:
void CoordinateInformation::setLimits(double northLimit, double southLimit, double eastLimit, double westLimit)
{
    north = northLimit;
    south = southLimit;
    east = eastLimit;
    west = westLimit;
}

// Getters:
double CoordinateInformation::getLatitude()
{
    return latitude;
}

double CoordinateInformation::getLongitude()
{
    return longitude;
}

double CoordinateInformation::getNorthLimit()
{
    return north;
}

double CoordinateInformation::getSouthLimit()
{
    return south;
}

double CoordinateInformation::getEastLimit()
{
    return east;
}

double CoordinateInformation::getWestLimit()
{
    return west;
}