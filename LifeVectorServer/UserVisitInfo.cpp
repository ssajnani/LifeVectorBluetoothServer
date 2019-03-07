#include "UserVisitInfo.h"

UserVisitInfo::UserVisitInfo()
{
    visitFrequency = 0;
    totalVisitDuration = 0;

    timeLog = std::map<long, int>();
}

UserVisitInfo::~UserVisitInfo() {}

// New Visit
void UserVisitInfo::addSingleLog(long time_stamp, int time_spent)
{
    timeLog.emplace(time_stamp, time_spent);

    visitFrequency++;
    totalVisitDuration += time_spent;
}

bool UserVisitInfo::updateLog(UserVisitInfo log)
{
    int initial_count = visitFrequency, initial_duration = totalVisitDuration;
    std::map<long, int> umap = log.getTimeLog();
    std::map<long, int>::iterator it = umap.begin();

    for (it; it != umap.end(); ++it)
    {
        // std::cout << "Inserting : " << it->first << ", " << it->second << std::endl;
        addSingleLog(it->first, it->second);
    }

    // std::cout << "Input : " << log.getVisitFrequency() << ", " << log.getTotalTimeSpent() << std::endl
    //           << "Before : " << initial_count << ", " << initial_duration << std::endl
    //           << "After : " << visitFrequency << ", " << totalVisitDuration << std::endl;

    if ((visitFrequency - initial_count == log.getVisitFrequency()) && (totalVisitDuration - initial_duration == log.getTotalTimeSpent()))
    {
        std::cout << "User Visiting Information Updated Successfully." << std::endl;
        return true;
    }
    else
    {
        std::cout << "User Visiting Information Update Failed" << std::endl;
        return false;
    }
}

// Getters:
int UserVisitInfo::getVisitFrequency()
{
    return visitFrequency;
}

int UserVisitInfo::getTotalTimeSpent()
{
    return totalVisitDuration;
}

std::map<long, int> UserVisitInfo::getTimeLog()
{
    return timeLog;
}

std::string UserVisitInfo::beautify()
{
    std::map<long, int>::iterator it;
    std::stringstream output;

    output << "Time Log:\n";

    for (it = timeLog.begin(); it != timeLog.end(); ++it)
    {
        SystemTimeManager t_stamp(it->first);

        output << t_stamp.UNIXtoEST() << "\t Duration: " << SystemTimeManager::format(it->second) << "\n";
    }

    output << "Visit Count: " << visitFrequency
              << "\nTotal Time Spent: " << SystemTimeManager::format(totalVisitDuration) << "\n";
    
    return output.str();
}

void UserVisitInfo::printLog()
{
    std::cout << beautify() << std::endl;
}