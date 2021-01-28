//
// Created by micho6 on 24.01.2021.
//

#ifndef TIN_SESSION_H
#define TIN_SESSION_H

#include <set>
#include <vector>
#include <string>

#include "../client/NFSClient.h"

class Session {
public:
    explicit Session(NFSClient *client);

    void start();

    void executeCommand(const std::vector<std::string> &tokens);

    std::vector<std::string> parseLine(const std::string &line);

private:
    int download(const std::string &serverSrc, const std::string &localDest);

    int upload(const std::string &localSrc, const std::string &serverDest);

    static void printHelp();

    NFSClient *client = nullptr;

    std::set<int> fileDescriptors;
    std::set<int> directoryDescriptors;

    bool opened = true;
};


#endif //TIN_SESSION_H
