//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_SERVER_H
#define TIN_SERVER_H

#include "FileManager.h"

#include <atomic>
#include <vector>
#include <thread>

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

    std::atomic<bool> running = true;
    std::vector<std::thread> threads;

    void uiThread();
    static void printHelp();

    void connectionThread(int connectionSocketFd);
};

#endif //TIN_SERVER_H
