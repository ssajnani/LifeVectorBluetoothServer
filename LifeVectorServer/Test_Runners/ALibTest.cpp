#ifndef STRING_PARSER_H
#include "../StringParser.h"
#endif

#include "../Database.h"
#include "../UserController.h"
#include "../json.hpp"
#include "../StringParser.h"

// All Archive Classes
#include "../UserVisitInfo.h"
#include "../VisitLog.h"
#include "../LocationInformation.h"
#include "../CoordinateInformation.h"
#include "../ArchivedLocation.h"
#include "../ArchiveLibrary.h"

#include "../googleAPI.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>
#include <cmath>
#include <stdlib.h>

int main()
{
    cout << "Testing Archive Library Functionalities\n\n";
    // Fresh Test Ground

    using namespace std;

    UserController uc("server", "LifeVector123");
    uc.getDBConnection()->exeSQL("DELETE FROM User;");

    cout << "Adding users, main_usr and usr1, for Archive testing.\n"
         << endl;
    uc.createUser("usr1", "nx5", "LV123");
    uc.createUser("main_usr", "nx5", "archve");

    // Location Archiver Tests

    cout << "Testing Object Creation: LocationInformation" << endl;

    LocationInformation l(200);
    l.setName("Thompson Arena");
    l.setAddress("Western University, Lambton Drive, Stoneybrook Gardens, London, Middlesex County, Ontario, N6G 5K8, Canada");
    l.setDescription("sports centre");

    cout << "LocationInformation Obj creation of ID : "
         << l.getID() << " - was successful.\n"
         << endl;

    cout << "Creating CoordinateInformation Obj" << endl;

    double x = 43.00307, y = -81.27547;
    double n = 43.00341, w = -81.27605, s = 43.00268, e = -81.27488;
    CoordinateInformation c(x, y);
    c.setLimits(n, s, e, w);

    cout << "CoordinateInformation Object creation successful" << endl
         << endl;

    cout << "Creating ArchivedLocation Object..." << endl;

    ArchivedLocation archL(l.getID(), x, y, n, s, e, w);
    archL.setLocationInformation(l.getName(), l.getAddress(), l.getDescription());

    cout << "ArchivedLocation object, archL, created : " << endl;
    archL.printInformation();

    // Log class tests

    cout << "Testing tracking log stroage obj containers \n\n"
         << "Creating UserVisitInfo obj for user-specific squash data..." << endl;

    UserVisitInfo uvi;
    uvi.addSingleLog(1543183200, 900);
    uvi.addSingleLog(1540519559, 3000);
    uvi.addSingleLog(1542337559, 6000);

    cout << "UserVisitInfo object, uvi, successfully created:\n"
         << endl;
    uvi.printLog();

    cout << "Creating VisitLog obj for squashed location..." << endl;
    VisitLog vl;
    bool check = false;

    string uID = "usr1/nx5";

    if (vl.userFound(uID))
    {
        check = vl.addFullLog(uID, uvi);
    }
    else
    {
        if (vl.addNewUser(uID))
        {
            check = vl.addFullLog(uID, uvi);
        }
    }
    cout << "uvi log addition to vl for " << uID << "\t: bool = " << check << endl;

    UserVisitInfo uvi2;
    uvi2.updateLog(uvi);
    bool bl = vl.addFullLog("main_usr/nx5", uvi2);
    cout << "uvi2 created with uvi obj, directly" << endl
         << "uvi2 log addition to vl for 'main_usr/nx5'\t bool = " << bl << endl;

    vl.printLog();

    // Archive Library Test
    cout << "\n~~~~~ Testing Archive Library Functionalities~~~~~" << endl;
    ArchiveLibrary TestLibrary(uc.getDBConnection());
    uc.getDBConnection()->exeSQL("DELETE FROM ArchivedLocations;");
    uc.getDBConnection()->exeSQL("DELETE FROM VisitLog;");
    cout << "ArchiveLibrary obj created.\n"
         << endl;

    // single instance
    cout << "Adding location of ID - 200 to database..." << endl;

    TestLibrary.saveLocationToDatabase(archL);
    ArchivedLocation *frmDB_l = TestLibrary.getLocationFromDatabase(200);
    cout << "Location of ID " << frmDB_l->getID() << "has been successfully save to database\n"
         << endl;

    cout << "Create 4 locations for archiving..." << endl;
    // Add a location
    string loc_name, address, desc;
    int loc_id = 400, id[4] = {0, 0, 0, 0}, temp = 3, cnt;
    double x_ref, y_ref, n_border, e_border, s_border, w_border;

    // create some locations:
    cout << "Generate coordinates of locations\n";
    string lat[4] = {"43.009005", "43.005586", "43.642571", "43.653440"};
    string lng[4] = {"-81.269028", "-81.276231", "-79.387057", "-79.384094"};

    ArchivedLocation loca_loca;

    cout << "Create locations with Google API's\n";
    // add 4 locations
    for (cnt = 0; cnt < 4; cnt++)
    {
        // get info from google API
        googleAPI google(lat[cnt], lng[cnt]);
        loc_name = google.getName();
        address = google.getFormattedLocation();

        int azero = 0;
        desc = google.getTypes(azero);

        x_ref = std::stod(google.getLat());
        y_ref = std::stod(google.getLng());

        n_border = std::stod(google.getNorthEastLat());
        e_border = std::stod(google.getNorthEastLng());
        s_border = std::stod(google.getSouthWestLat());
        w_border = std::stod(google.getSouthWestLng());

        // create ArchiveLocation
        loca_loca = TestLibrary.constructLocation(loc_id, loc_name, address, desc, x_ref, y_ref, n_border, s_border, e_border, w_border);

        cout << "Location " << loc_id << " has been found and created.\n";

        // print whats found
        loca_loca.printInformation();

        // save to database
        TestLibrary.saveLocationToDatabase(loca_loca);

        if (loc_id == loca_loca.getID())
        {
            id[cnt] = loca_loca.getID();
            cout << "Location " << id[cnt] << " has been saved to database.\n";
        }

        // make new id
        loc_id += temp;

        // cout << "check - reached" << endl;
    }

    cout << "4 locations has been archived to the database" << endl;
    /******************************/

    //fetchReport fr();

    //fr.getReport();

    /***********************************/

    // create some time logs;
    int interval = 5 * 60; // tracking interval 5min => 300s

    // generate some logs

    cout << "Generatign and inserting time logs for locations into database..." << endl;

    UserVisitInfo u1;
    u1.addSingleLog(1538745600, (4 * interval));  // 2018-10-05 09:20:00, 20min
    u1.addSingleLog(1538756100, (20 * interval)); // 2018-10-05 12:15:00, 100min
    u1.addSingleLog(1539895800, (15 * interval)); // 2018-10-18 16:50:00, 75min

    UserVisitInfo u2;
    u2.addSingleLog(1540039500, (45 * interval)); // 2018-10-20 08:45:00, 225min
    u2.addSingleLog(1540831500, (25 * interval)); // 2018-10-29 12:45:00, 125min

    UserVisitInfo u3;
    u3.addSingleLog(1542767520, (31 * interval)); // 2018-11-20 21:32:00, 155min

    UserVisitInfo u4;
    u4.addSingleLog(1541472300, (48 * interval)); // 2018-11-5 21:45:00, 240min

    cout << "time logs generated." << endl;

    //get locations from DB

    cout << "Fetch a location from database..." << endl;

    ArchivedLocation *sloc = TestLibrary.getLocationFromDatabase(200);

    /*     string testline = "400\tDelaware Hall NB - #510\tLondon ON N6G Canada\t43.0086000\t-81.2690000\t43.01000000\t43.00730000\t-81.26760000\t-81.27030000\tpremise";
    string testint = "400";
    string testd = "43.01000000";
    int frmstr = stoi(testint);
    double wasstr = stod(testd);

    cout << testint << frmstr << endl
         << testd << wasstr << endl;
 */
    cout << "Location " << sloc->getID() << " retrieved from database.\n"
         << endl;

    sloc->printInformation();

    cout << "Find Locations from database with various IDs. Present and not present in DB..." << endl;

    cnt = 0;
    for (temp = 399; temp < 410; temp++)
    {
        ArchivedLocation *found = TestLibrary.getLocationFromDatabase(temp);

        if (found)
        {
            if (cnt < 4)
            {
                id[cnt] = found->getID();
                cout << "Location " << id[cnt] << "found in DB." << endl;
                cnt++;

                found->printInformation();
            }
        }
    }

    cout << "End of Location search test\n"
         << endl;

    //uc.getDBConnection()->exeSQL("DELETE FROM VisitLog;"); // <-------------------------------

    cout << "Test Saving Logs to DB..." << endl;

    // add the logs to the 4 locations
    TestLibrary.archiveUserLog(200, "usr1", "nx5", uvi2);
    cout << "uvi2 log data successfully saved to database, for usr1" << endl;

    temp = 0;
    UserVisitInfo userlogs[4] = {u1, u2, u3, u4};
    for (; temp < 4; temp++)
    {
        TestLibrary.archiveUserLog(id[temp], "main_usr", "nx5", userlogs[temp]);
        cout << "Log dataset " << temp + 1 << " has been seved to Location " << id[temp] << " in database, for main_usr." << endl;
    }

    cout << "All log datasets successfully archived to database" << endl
         << endl;

    cout << "Full Archive Data retrieval from database each location: " << endl;
    // get and print log information for each location
    for (temp = 0; temp < 4; temp++)
    {
        ArchivedLocation *found = TestLibrary.getLocationFromDatabase(id[temp]);

        if (found)
        {
            found->printInformation();

            // Location Log
            cout << "Get location visit logs from database: " << endl;
            VisitLog *vlog = TestLibrary.getLocationRecordFromDatabase(found->getID());
            // vlog->printLog();

            // Visit count function test

            cout << "Test Visit Frequency and Total Visit Duration getters:" << endl;
            int l_count = TestLibrary.getVisitCount(found->getID());
            int lu_count = TestLibrary.getVisitCount(found->getID(), "main_usr", "nx5");

            if (l_count == lu_count)
            {
                cout << "Frequency function test passed." << endl
                     << "total count: " << l_count << endl
                     << "main_usr count: " << lu_count << endl;
            }

            // Duration function test
            int dur = TestLibrary.getDurationAtLocation(found->getID());
            int udur = TestLibrary.getDurationAtLocation(found->getID(), "main_usr", "nx5");

            if (dur == udur)
            {
                cout << "Visit duration functions test passed. " << endl
                     << "total duration: " << dur << endl
                     << "main_usr duration: " << udur << endl;
            }
        }
    }

    // User Log
    cout << "User Visit Logs for main_usr:" << endl;
    std::map<int, UserVisitInfo> *u_log = TestLibrary.getUserLogFromDatabase("main_usr", "nx5");

    if (u_log)
    {
        cout << "user log retrieved" << endl;

        map<int, UserVisitInfo>::iterator uit = u_log->begin();
        for (uit; uit != u_log->end(); ++uit)
        {
            cout << "User at LocID: " << uit->first << endl;
            // uit->second.printLog();
        }
    }

    cout << "end of archive tests" << endl;

    return 0;
}
