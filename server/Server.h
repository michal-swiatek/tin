//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_SERVER_H
#define TIN_SERVER_H

#include "FileManager.h"

class Server
{
public:
    Server(int connections, const char* ipv4 = nullptr);

    void setup();
    void close();

    void run();

private:
    int connections{};
    const char* ipv4 = nullptr;

    int socketFd{};
    FileManager& fm;

    bool running = true;

    void uiThread();
    static void printHelp();
};

#endif //TIN_SERVER_H
