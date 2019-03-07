#include "placesAPI.h"

using namespace std;

    string placesAPI :: getLocationName(){

        //parse through json to find place name
        string result = doc.at("result").at("name");

        return result;
    }



