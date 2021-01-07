#ifndef TIN_AUTHORIZATION_H
#define TIN_AUTHORIZATION_H

#include <iostream>
#include <map>
#include <string>
#include <mutex>

using namespace std;

class Authorization{

    private:
    map<string, string> usersRoles;   //user and role
    map <string, string> usersPass;   //user and password
    map<string,bool>  usersActive; //user and bool if user is logged
    mutex m;



public:
    Authorization();
    ~Authorization();

    //sprawdzaenie czy istnieje użytkownik o zadanej nazwie i haśle
    bool login(string user, string pass);
    //zwraca typ użytkownika: "a", "u" lub " " gsy użytkownik nie istnieje
    string userRole(string user);
    bool logOut(string user);

};

#endif //TIN_AUTHORIZATION_H
