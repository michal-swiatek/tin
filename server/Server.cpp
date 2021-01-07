//
// Created by micho6 on 05.01.2021.
//

#include "Server.h"

//  Unix libraries
#include <sys/socket.h>
#include <netinet/in.h>

//  Standard C/C++ libraries
#include <cstdio>
#include <iostream>

//  Project libraries
#include "ConnectionThread.h"

Server::Server(int connections, const char* ipv4) : connections(connections),
                                                    ipv4(ipv4),
                                                    fm(FileManager::getInstance()) { }

void Server::setup()
{
    //
    //  Setup connection
    //

    //  Open passive socket for accepting connections
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Opening stream socket");
        exit(1);
    }

    //  Bind server address to socket
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; //  TODO: implement setting up specified connection
    serverAddress.sin_port = 0;

    if (bind(socketFd, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        perror("Binding system socket");
        exit(1);
    }

    //  Print server port
    int length = sizeof(serverAddress);
    if (getsockname(socketFd, (sockaddr *)(&serverAddress), (socklen_t *)(&length)) == -1)
    {
        perror("Getting socket name");
        exit(2);
    }

    std::cout << "Socket port: " << ntohs(serverAddress.sin_port) << '\n';

    //
    //  Setup FilesManager
    //
    fm.init();
}

void Server::close()
{
    fm.end();
}

void Server::run()
{
    //  Start UI thread
    std::thread uiThread(&Server::uiThread, this);

    listen(socketFd, connections);
    fd_set ready;
    struct timeval timeout{1, 0};

    while (running)
    {
        FD_ZERO(&ready);
        FD_SET(socketFd, &ready);

        int ret = select(socketFd + 1, &ready, nullptr, nullptr, &timeout);

        if (FD_ISSET(socketFd, &ready))
        {
            struct sockaddr_in clientAddr{};
            int connectionFd = accept(socketFd, (sockaddr *)(&clientAddr), (socklen_t *)(sizeof(clientAddr)));

            threads.emplace_back(&Server::connectionThread, this, connectionFd);
        }

        //  Check server status
        //  TODO: implement checking server status
    }

    uiThread.join();
    for (auto& thread : threads)
        if (thread.joinable())
            thread.join();
}

void Server::uiThread()
{
    std::string action;

    while (running)
    {
        std::cout << "serv$ ";
        std::cin >> action;

        std::cout << "UI: " << running << '\n';

        if (action == "quit")
            running = false;
    }
}

void Server::printHelp()
{
    std::cout << "Server help\n\n";
    std::cout << "quit - close server and exit\n";
    std::cout << std::endl;
}

void Server::connectionThread(int connectionSocketFd)
{
    ConnectionThread t(connectionSocketFd, running);

    t.run();
}
