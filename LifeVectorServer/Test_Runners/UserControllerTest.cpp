#include "../UserController.h"
#include "../json.hpp"

int main()
{
	cout << "Testing User Controller Functionality\n\n\n"; 

    string username = "John Smith";
    string devID = "nexus5";
    string password = "1234";
    nlohmann::json report = {
        {"key1_1", "value1_1"},
        {"key1_2", "value1_2"}};

    cout << "Attempting to Connect to Database..." << endl;
    UserController uc("server", "LifeVector123");
    cout << "Connected to Database as: server." << endl << endl;

    cout << "Creating New User: John Smith" << endl;
    if (uc.createUser(username, devID, password))
    {
        cout << "User " << username << "created" << endl << endl;
    }
    else
    {
        cout << "Could not create user, John Smith, in database." << username << endl << endl;
    }

    cout << "Updating json report in database with test report:" << endl;
    if (uc.updateReport(username, devID, report))
    {
        cout << "Report updated for " << username << "." << endl << endl;
    }


    cout << "Testing Fetching report from database:" << endl;
    nlohmann::json *reportFromDB = new json();
    
    if (uc.fetchReport(reportFromDB, username, devID))
    {
        cout << "Report for " << username << " fetched: " << endl
             << *reportFromDB << endl << endl;
    }

    cout << "Testing hash function and hash reproducibility for " << username << "with password: " << password << endl;
    if (uc.compareUserHash(username, devID, password))
    {
        cout << "Hash matched" << endl << endl;
    }
    else
        cout << "Hash not matched" << endl << endl;

    // test raw data uploader

    cout << "Reading and saving sample raw gps data from file to database:" << endl
         << "File Sample: ./SampleFiles/sample_coordinates.json:" << endl;
    RawDataRepository sampledata("./SampleFiles/sample_coordinates.json", "gps_coordinates_dump");
    
    cout << "Sample file read.\nData read from file:" << endl;
    sampledata.dumpData();

    cout << "Saving data to database..." << endl;
    if (uc.saveRawGPStoDB(username, devID, sampledata))
    {
        cout << "Sample Tracking data from sample_coordinates.json for ( " << username << ", " << devID << " ) is saved to database" << endl << endl;
    }
    else
    {
        cout << "GPS Data failed to save to database." << endl << endl;
    }

    cout << "Delete User: " << username << endl;
    if (uc.deleteUser(username, devID))
    {
        cout << "User Deletion Successful: " << username << " has been removed." << endl << endl;
    }
    else
    {
        cout << "USer Deletion Failed: " << username << " not deleted." << endl << endl;
    }

    return 0;
}
