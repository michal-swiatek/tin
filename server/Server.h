//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_SERVER_H
#define TIN_SERVER_H

#include <atomic>
#include <vector>
#include <memory>

#include "FileManager.h"
#include "authorization/authorization.h"
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

    Authorization authorization{};

    std::atomic<bool> running = true;
    std::vector<std::unique_ptr<ConnectionThread>> threads;

    void uiThread();
    static void printHelp();
};

#endif //TIN_SERVER_H
