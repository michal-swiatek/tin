//
// Created by micho6 on 05.01.2021.
//

#include "Server.h"

//  Unix libraries
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    serverAddress.sin_port = 8080;

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

    address = inet_ntoa(serverAddress.sin_addr);
    port = serverAddress.sin_port;

    //
    //  Setup FilesManager
    //
    fm.init("../", "disk", "filesOwners.txt");
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

            auto connectionHandler = ConnectionHandler(connectionFd);
            auto request = connectionHandler.getRequest();

            if (request == ConnectionHandler::CONNECT)
            {
                char* data = connectionHandler.getData();
                auto header = connectionHandler.getHeader();

                std::string login(data, header.param1);
                std::string password(data + header.param1, header.param2);

                if (authorization.login(login, password))
                    threads.emplace_back(new ConnectionThread(std::move(connectionHandler), running));
                else
                    ::close(connectionFd);  //  Don't start session if authorization failed
            }
            else
                ::close(connectionFd);  //  Don't start session if authorization failed
        }

        //  Check server status
        for (int i = (int)threads.size() - 1; i >= 0; --i)
            if (threads[i]->isClosed())
            {
                threads[i]->getThread().join();
                threads.erase(threads.begin() + i);
            }
    }

    uiThread.join();
    for (auto& thread : threads)
        if (thread->getThread().joinable())
            thread->getThread().join();
}

void Server::uiThread()
{
    std::string action;

    while (running)
    {
        std::cout << "serv$ ";
        std::cin >> action;

        if (action == "quit")
            running = false;
        else if (action == "connections")
            std::cout << "Number of connections: " << threads.size() << '\n';
        else if (action == "address")
            std::cout << "Server address: " << address << '\n';
        else if (action == "port")
            std::cout << "Socket port: " << port << '\n';
    }
}

void Server::printHelp()
{
    std::cout << "Server help\n\n";
    std::cout << "quit - close server and exit\n";
    std::cout << std::endl;
}
