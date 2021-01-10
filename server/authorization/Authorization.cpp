#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <mutex>
#include "Authorization.h"

Authorization::Authorization(){
  fstream f;
  string word;
  string filename = "users.txt";
  f.open(filename.c_str());

  int i = 0;
  string userN, userP, userR;
  while (f>>word) {
    switch (i%3){
      case 0:
        userN = word;
        break;
      case 1:
        userP = word;
        break;
      case 2:
        userR = word;
        usersRoles.insert(pair<string, string>(userN, userR));
        usersPass.insert(pair<string, string>(userN, userP));
        usersActive.insert(pair<string, bool>(userN, false));
        break;
      default:
        break;
    }
    i++;
  }

  // Close the file
  f.close();
}

Authorization::~Authorization(){}

bool Authorization::login(string user, string pass){
  std::lock_guard<std::mutex> lockGuard(m);

  map<string,string>::iterator it = usersPass.begin();
  while (it != usersPass.end())
  {
    string userN = it->first;
    string userP = it->second;
    if(userN.compare( user) == 0 && userP.compare(pass) == 0 ) return true;
    it++;
  }
  return false;
}

bool Authorization::logOut(string user)
{
  std::lock_guard<std::mutex> lockGuard(m);
  map<string,bool>::iterator it = usersActive.begin();
  while (it != usersActive.end())
  {
    string userN = it->first;
    bool isLogged = it->second;
    if(userN.compare(user) == 0 && isLogged == true){it->second = false; return true;}
    if(userN.compare(user) == 0 && isLogged == false) return false;
    it++;
  }

  return false;
}

string Authorization::userRole(string user){
  map<string,string>::iterator it = usersRoles.begin();
  while (it != usersRoles.end())
  {
    string userN = it->first;
    string userR = it->second;
    if(userN.compare( user) == 0) return userR;
    it++;
  }
  return " ";
}
