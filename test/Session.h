//
// Created by micho6 on 24.01.2021.
//

#ifndef TIN_SESSION_H
#define TIN_SESSION_H

#include <set>
#include <vector>
#include <string>

#include "../client/Client.h"

class Session
{
public:
    explicit Session(Client* client);

    void start();

    void executeCommand(const std::vector<std::string>& tokens);

    static std::vector<std::string> parseLine(const std::string& line);

private:
    static void printHelp();

    Client* client = nullptr;

    std::set<int> fileDescriptors;
    std::set<int> directoryDescriptors;

    bool opened = true;
};


#endif //TIN_SESSION_H
