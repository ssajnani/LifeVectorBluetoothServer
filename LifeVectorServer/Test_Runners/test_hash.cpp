

#include "../hashfunc.h"
#include <string>

using namespace std;
 

 
int main(){

    std::string str = "1234";

    hashfunc a ;

    a.hashString(str);

    string salt = a.getSalt();
    string hash = a.getHash();
    string specific_salt = a.getHash(str, salt);

    cout << "Salt: " << salt << endl;
    cout << "Hash: " << hash << endl;

    cout << "Return specific hash: " << specific_salt << endl;

}
