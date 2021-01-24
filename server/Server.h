//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_SERVER_H
#define TIN_SERVER_H

#include <atomic>
#include <vector>
#include <memory>

#include "fs/FileManager.h"
#include "authorization/Authorization.h"
#include "ConnectionThread.h"

class Server
{
public:
    explicit Server(int connections, const char* ipv4 = nullptr);

    void setup();
    void close();

    void run();

private:
    int connections{};
    const char* ipv4 = nullptr;

    int socketFd{};
    FileManager& fm;

    std::shared_ptr<Authorization> authorization;

    std::atomic<bool> running = true;
    std::vector<std::unique_ptr<ConnectionThread>> threads;

    void uiThread();
    static void printHelp();

    //  Info
    int port{};
    std::string address;
};

#endif //TIN_SERVER_H
