#include "StringParser.h"

#include "Database.h"
#include "UserController.h"
#include "fetchReport.h"
#include "json.hpp"

// All Archive Classes
#include "UserVisitInfo.h"
#include "VisitLog.h"
#include "LocationInformation.h"
#include "CoordinateInformation.h"
#include "ArchivedLocation.h"
#include "ArchiveLibrary.h"

#include "googleAPI.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>

Database db;
UserController uc("server", "LifeVector123");

void add_user()
{
    // add user
    
    if (uc.createUser("usr1", "nx5", "LV123"))
    {
        cout << "User " << "usr1" << "created";
    }
    else
    {
        cout << "Unable to create user" << "usr1" << endl;
    }
    
    if (uc.createUser("main_usr", "nx5", "archve"))
    {
        cout << "User " << "main_usr" << "created";
    }
    else
    {
        cout << "Unable to create user" << "main_usr" << endl;
    }
}

Database add_database_entries()
{
    // init local db and add test entries
    
    db.initDB("localhost", "server", "LifeVector123", "LifeVector");

    db.exeSQL("DELETE FROM VisitLog WHERE locationID = 114;");

    db.exeSQL("INSERT INTO VisitLog (visitTime, locationID, duration, username, deviceID) VALUES (1543183200, 114, 900, 'usr1', 'nx5');");

    db.exeSQL("INSERT INTO VisitLog (visitTime, locationID, duration, username, deviceID) VALUES (1540519559, 114, 1800, 'usr1', 'nx5');");

    db.exeSQL("INSERT INTO VisitLog (visitTime, locationID, duration, username, deviceID) VALUES (1542337559, 114, 3000, 'usr1', 'nx5');");

    std::cout << "VisitLog Insert Successful" << std::endl;

    return db;
}

/* void location_db_insertion_test(Database db);
{
    // Insert Location
    std::stringstream query;
    query << "SELECT * FROM ArchivedLocations WHERE locationID = " << 114 << ";";

    std::string result = db.getSQLResult(query.str());

    std::cout << result << std::endl;

    std::vector<std::string> splitted = std::vector<std::string>();

    StringParser::custom_parse(result, splitted, '\t');

    std::cout << "Location Split Print:" << std::endl;

    std::vector<std::string>::iterator it;

    int i = 0;

    for (it = splitted.begin(); it != splitted.end(); ++it)
    {
        std::cout << i++ << ".\t" << (*it) << std::endl;
    }

    /* Field Legend
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

/* ArchivedLocation test(atoi(splitted[0].c_str()), atof(splitted[3].c_str()), atof(splitted[4].c_str()), atof(splitted[5].c_str()), atof(splitted[6].c_str()), atof(splitted[7].c_str()), atof(splitted[8].c_str()));

    test.setLocationInformation(splitted[1], splitted[2], splitted[9]);

    test.printInformation();

    std::stringstream q1;
    q1 << "SELECT locationID FROM ArchivedLocations WHERE locationID=200;";

    result = db.getSQLResult(q1.str());

    std::cout << "|" << result << "|" << std::endl;
} */

void log_output_test(Database db)
{
    // for visitlog
    std::stringstream query;
    query << "SELECT * FROM VisitLog WHERE locationID = " << 114 << ";";

    std::string result = db.getSQLResult(query.str());

    std::cout << result << std::endl;

    std::vector<std::string> splitted;

    std::cout << "Result for all VL entries\n"
              << result << "\n---------" << std::endl;

    splitted = std::vector<std::string>();
    StringParser::custom_parse(result, splitted, '\n');

    std::cout << "VisitLog Split Print:" << std::endl;
    std::vector<std::string>::iterator it;

    int i = 0;
    for (it = splitted.begin(); it != splitted.end(); ++it)
    {
        std::cout << i++ << ". " << (*it) << std::endl;
    }
}

std::string composeID(std::string username, std::string deviceID)
{
    std::string combinationSymbol = "/", userID;
    userID.append(username);
    userID.append(combinationSymbol);
    userID.append(deviceID);

    return userID;
}

void ArchClassTests()
{
    // using namespace std;

    // Location Archiver Tests

    LocationInformation l(200);
    l.setName("Thompson Arena");
    l.setAddress("Western University, Lambton Drive, Stoneybrook Gardens, London, Middlesex County, Ontario, N6G 5K8, Canada");
    l.setDescription("sports centre");

    cout << "LocInfo: \n"
         << l.getID() << " - has been created.\n"
         << l.getAddress() << "\n"
         << l.getName() << "\n"
         << l.getDescription()
         << endl;

    double x = 43.00307, y = -81.27547;
    double n = 43.00341, w = -81.27605, s = 43.00268, e = -81.27488;
    CoordinateInformation c(x, y);
    c.setLimits(n, s, e, w);

    cout << "Coordinate Object created" << endl;

    ArchivedLocation archL(l.getID(), x, y, n, s, e, w);
    archL.setLocationInformation(l.getName(), l.getAddress(), l.getDescription());

    cout << "ArchivedLocation object created: archL : " << endl;
    archL.printInformation();

    // Log class tests

    UserVisitInfo uvi;
    uvi.addSingleLog(1543183200, 900);
    uvi.addSingleLog(1540519559, 3000);
    uvi.addSingleLog(1542337559, 6000);

    cout << "UserVisitInfo object created : uvi" << endl;
    uvi.printLog();

    VisitLog vl;
    bool check = false;

    string uID;
    uID = composeID("usr1", "nx5");

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

    cout << "VisitLog vl is populated with uvi - " << check << endl;

    UserVisitInfo uvi2;

    uvi2.updateLog(uvi);
    /*from here on start creating the json*/

    nlohmann::json report;

    report["location"]["totalVisits"] = vl.getTotalTimesVisited();
    report["location"]["totalTimeSpent"] = vl.getTotalTimeSpent();


    /*
	nlohmann::json report = {
		{"key1_1", "value1_1"},
		{"key1_2", "value1_2"}};
	*/
    cout << "Updating json report structure in database" << endl;
    if (uc.updateReport("usr1", "nx5", report))
    {
        cout << "Report updated for " << "usr1" << endl;
    }

    nlohmann::json *reportFromDB = new json();
    

	if(uc.fetchReport(reportFromDB, "usr1", "nx5")){
		

		cout << "Report: " << endl
		     << *reportFromDB << endl;

    }
    else{
    	cout << "cant print" << endl;
    }
}

int main()
{
    using namespace std;

    add_user();
    ArchClassTests();

    /* // Archive Library Test
    Database db;
    db = add_database_entries();
    cout << "here" << endl;
    ArchiveLibrary TestLibrary(&db);
    // Add a location
    string loc_name, address, desc;
    int loc_id = 200;
    double x_ref, y_ref, n_border, e_border, s_border, w_border;

    // create some locations:
    map<string, string> loc_coord = map<string, string>();
    
    loc_coord.insert(make_pair("43.009005", "-81.269028"));
    loc_coord.insert(make_pair("43.005586", "-81.276231"));
    loc_coord.insert(make_pair("43.642571", "-79.387057"));
    loc_coord.insert(make_pair("43.653440", "-79.384094"));

    int id[4], temp = 3, cnt = 0;

    // add 4 locations
    for (map<string, string>::iterator i = loc_coord.begin(); i != loc_coord.end(); ++i)
    {
        // get info from google API
        string lat = i->first;
        string lng = i->second;
        googleAPI google(lat, lng);
        loc_name = google.getName();
        address = google.getFormattedLocation();
        int azero = 0;
        desc = google.getTypes(azero);

        x_ref = atof(google.getLat().c_str());
        y_ref = atof(google.getLng().c_str());

        n_border = atof(google.getNorthEastLat().c_str());
        e_border = atof(google.getNorthEastLng().c_str());
        s_border = atof(google.getSouthWestLat().c_str());
        w_border = atof(google.getSouthWestLng().c_str());

        // create ArchiveLocation
        ArchivedLocation loca_loca = TestLibrary.constructLocation(loc_id, loc_name, address, desc, x_ref, y_ref, n_border, s_border, e_border, w_border);

        // print whats found
        loca_loca.printInformation();

        // save to database
        TestLibrary.saveLocationToDatabase(loca_loca);

        cout << "new location added to DB" << endl;

        // make new id
        loc_id += (12 * temp++ - 34);
        id[cnt++] = loca_loca.getID();
    }

    cout << " 4 locations archived" << endl;

    // create some time logs;
    int interval = 5 * 60; // tracking interval 5min => 300s

    // generate some logs
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

    //get locations from DB
    cnt = 0;
    for (temp = 150; temp < 2000; temp++)
    {
        void *found = (ArchivedLocation *)malloc(sizeof(ArchivedLocation));

        if (TestLibrary.getLocationFromDatabase(((ArchivedLocation *)found), temp))
        {
            ((ArchivedLocation *)found)->printInformation();
            id[cnt++] = ((ArchivedLocation *)found)->getID();
        }

        free(found);
    }

    // add the logs to the 4 locations
    temp = 0;
    TestLibrary.archiveUserLog(id[temp++], "main_usr", "nx5", u1);
    TestLibrary.archiveUserLog(id[temp++], "main_usr", "nx5", u2);
    TestLibrary.archiveUserLog(id[temp++], "main_usr", "nx5", u3);
    TestLibrary.archiveUserLog(id[temp], "main_usr", "nx5", u4);

    // get and print log information for each location
    for (temp = 0; temp < 4; temp++)
    {
        void *found = (ArchivedLocation *)malloc(sizeof(ArchivedLocation));

        if (TestLibrary.getLocationFromDatabase(((ArchivedLocation *)found), id[temp]))
        {
            ((ArchivedLocation *)found)->printInformation();

            // User Log
            std::map<int, UserVisitInfo> *u_log = new std::map<int, UserVisitInfo>();
            TestLibrary.getUserLogFromDatabase(u_log, "main_usr", "nx5");
            map<int, UserVisitInfo>::iterator uit = u_log->begin();

            for (uit; uit != u_log->end(); ++uit)
            {
                cout << "User at " << uit->first << endl;
                uit->second.printLog();
                free(u_log);
            }

            // Location Log
            void *vlog = (VisitLog *)malloc(sizeof(VisitLog));
            TestLibrary.getLocationRecordFromDatabase((VisitLog *)vlog, ((ArchivedLocation*) found)->getID());
            ((VisitLog *)vlog)->printLog();
            free(vlog);

            // Visit count function test
            int l_count = TestLibrary.getVisitCount(((ArchivedLocation *)found)->getID());
            int lu_count = TestLibrary.getVisitCount(((ArchivedLocation *)found)->getID(), "main_usr", "nx5");

            if (l_count == lu_count)
            {
                cout << "count functions test passed: " << l_count << endl;
            }

            // Duration function test
            int dur = TestLibrary.getDurationAtLocation(((ArchivedLocation *)found)->getID());
            int udur = TestLibrary.getDurationAtLocation(((ArchivedLocation *)found)->getID(), "main_usr", "nx5");

            if (dur == udur)
            {
                cout << "duration functions test passed : " << dur << endl;
            }
        }

        free(found);
    }

    cout << "end of test" << endl; */
}
