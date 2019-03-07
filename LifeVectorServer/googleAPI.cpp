#include "googleAPI.h"


using namespace std;

     string googleAPI :: getValidPlace(){

         //check to see if type is of importance
         for(int i = 0; i < doc.at("results").size(); i++){
             if(checkTypes(i)){
                 return getPlaceID(i);
                 break;
             }

         }
     }

     bool googleAPI :: checkTypes(int j) {

          bool flag = false;
          //check to see if type is unwanted
          for(int i=0; i < doc.at("results")[j].at("types").size(); i++){
              if (unwantedTypes.find(doc.at("results")[j].at("types")[i]) == std::string::npos) {
                  flag = true;
                  break;
              }
          }

          return flag;

     }


     void googleAPI::eraseAllSubStr(std::string & mainStr, const std::string & toErase)
     {
        // Search for the substring in string
        size_t pos = mainStr.find(toErase);

        if (pos != std::string::npos)
        {
            // If found then erase it from string
            mainStr.erase(pos, toErase.length());
        }
     }

     void googleAPI :: splitstr(const std::string& str, std::vector<std::string>& container, char delim)
     {
         std::stringstream ss(str);
         std::string token;
         while (std::getline(ss, token, delim)) {
             container.push_back(token);
         }
     }

     string googleAPI :: getFormattedLocation(){

         std::vector<std::string> c;

         string result = "";

         name = "";

         result = doc.at("results")[0].at("formatted_address");
         splitstr(result, c, ',');

         //check if address starts with a name, if so add to name
         if (!isdigit((c[0].c_str())[0])){
             string n = c[0];
             name = n;
             result = "";
             for (int i = 1; i < c.size(); i++){
                 result = result + c[i];
             }
         }

         location = result;

         if(location.find(apostrophe) != std::string::npos){
             eraseAllSubStr(location, apostrophe);
         }

         return location;
     }

     string googleAPI :: getName(){
         //if no name received from maps api, use google places api to find location name
         if(name.size() == 0){
             placesAPI place(getValidPlace());
             name = place.getLocationName();
         }

         if(name.find(apostrophe) != std::string::npos){
             eraseAllSubStr(name, apostrophe);
         }
         return name;
     }

     string googleAPI :: getLat(){

         // Create an output string stream
         std::ostringstream streamObj;
         //Add double to stream
         streamObj << doc.at("results")[0].at("geometry").at("location").at("lat");
         // Get string from output string stream
         std::string strObj = streamObj.str();

         return strObj;
     }

     string googleAPI :: getLng(){

         // Create an output string stream
         std::ostringstream streamObj;
         //Add double to stream
         streamObj << doc.at("results")[0].at("geometry").at("location").at("lng");
         // Get string from output string stream
         std::string strObj = streamObj.str();

         return strObj;
     }

     string googleAPI :: getNorthEastLat(){

         // Create an output string stream
         std::ostringstream streamObj;
         //Add double to stream
         streamObj << doc.at("results")[0].at("geometry").at("viewport").at("northeast").at("lat");
         // Get string from output string stream
         std::string strObj = streamObj.str();

         return strObj;
     }

     string googleAPI :: getNorthEastLng(){

         // Create an output string stream
         std::ostringstream streamObj;
         //Add double to stream
         streamObj << doc.at("results")[0].at("geometry").at("viewport").at("northeast").at("lng");
         // Get string from output string stream
         std::string strObj = streamObj.str();

         return strObj;
     }


     string googleAPI :: getSouthWestLat(){

         // Create an output string stream
         std::ostringstream streamObj;
         //Add double to stream
         streamObj << doc.at("results")[0].at("geometry").at("viewport").at("southwest").at("lat");
         // Get string from output string stream
         std::string strObj = streamObj.str();

         return strObj;
     }

     string googleAPI :: getSouthWestLng(){

         // Create an output string stream
         std::ostringstream streamObj;
         //Add double to stream
         streamObj << doc.at("results")[0].at("geometry").at("viewport").at("southwest").at("lng");
         // Get string from output string stream
         std::string strObj = streamObj.str();

         return strObj;
     }

     string googleAPI :: getTypes(int j){

         string result;

         //convert vector to string
         for (int i = 0; i < doc.at("results")[j].at("types").size(); i++){

             result = result.append(doc.at("results")[j].at("types")[i]);
             if (i + 1 != doc.at("results")[j].at("types").size()){
                 result = result.append(", ");
             }
         }

         if(result.find(apostrophe) != std::string::npos){
             eraseAllSubStr(result, apostrophe);
         }

         return result;

     }
     string googleAPI :: getPlaceID(int j){

         // Create an output string stream
         std::ostringstream streamObj;
         //Add double to stream
         streamObj << doc.at("results")[j].at("place_id");
         // Get string from output string stream
         std::string strObj = streamObj.str();

         strObj.erase(std::remove(strObj.begin(), strObj.end(), '\"'), strObj.end());


         return strObj;
     }






