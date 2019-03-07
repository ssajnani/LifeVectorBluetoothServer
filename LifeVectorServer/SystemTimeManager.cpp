#include "SystemTimeManager.h"
#include <iostream>

SystemTimeManager::SystemTimeManager(long unixTime)
{
    intTime = unixTime;
}

SystemTimeManager::SystemTimeManager()
{
    time(&intTime);
}

SystemTimeManager::~SystemTimeManager() {}

std::string SystemTimeManager::UNIXtoEST()
{
    ESTformat = localtime(&intTime);
    char buffer[80];
    strftime(buffer, 80, "%F %T", ESTformat);
    std::string formattedTime(buffer);

    return formattedTime;
}

std::string SystemTimeManager::UNIXtoUTC()
{
    UTCformat = gmtime(&intTime);
    char buffer[80];
    strftime(buffer, 80, "%F %T", UTCformat);
    std::string formattedTime(buffer);

    return formattedTime;
}

long SystemTimeManager::getTime()
{
    long t = static_cast<long>(intTime);
    return t;
}

std::string SystemTimeManager::format(int duration)
{
    time_t t = duration;
    struct tm *form = gmtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%R", form);
    std::string formattedTime(buffer);

    return formattedTime;
}