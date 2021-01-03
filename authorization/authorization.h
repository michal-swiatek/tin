#ifndef TIN_AUTHORIZATION_H
#define TIN_AUTHORIZATION_H

#include <iostream>
#include <map>
#include <string>

using namespace std;

class Authorization{

    private:
    map<string, string> usersRoles;   //user and role
    map <string, string> usersPass;   //user and password


public:
    Authorization();
    ~Authorization();

    //sprawdzaenie czy istnieje użytkownik o zadanej nazwie i haśle
    bool login(string user, string pass);
    //zwraca typ użytkownika: "a", "u" lub " " gdy użytkownik nie istnieje
    string userRole(string user);

};

#endif //TIN_AUTHORIZATION_H
