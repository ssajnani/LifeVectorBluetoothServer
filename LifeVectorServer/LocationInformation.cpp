/**
 * @file LocationInformation.cpp
 * 
 * @brief Class storing the location data
 * 
 */

#include "LocationInformation.h"

LocationInformation::LocationInformation(int locationID)
{
    id = locationID;

    name = address = description = "";
}

// Destructor:
LocationInformation::~LocationInformation() {}

// Detail Setter:
void LocationInformation::setName(std::string location_name)
{
    name = location_name;
}

void LocationInformation::setAddress(std::string location_address)
{
    address = location_address;
}

void LocationInformation::setDescription(std::string location_description)
{
    description = location_description;
}

// Getters:
int LocationInformation::getID()
{
    return id;
}

std::string LocationInformation::getName()
{
    return name;
}

std::string LocationInformation::getAddress()
{
    return address;
}

std::string LocationInformation::getDescription()
{
    return description;
}