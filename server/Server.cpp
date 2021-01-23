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
#include <memory>
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

    //  TODO: remove in final version
    int option = 1;
    setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    //  Bind server address to socket
    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; //  TODO: implement setting up specified connection
    serverAddress.sin_port = htons(8080);

    if (bind(socketFd, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        perror("Binding system socket");
        exit(1);
    }

    //  Check server address
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
    fm.init("./", "disk", "filesOwners.txt");
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
            int addrlen = sizeof(clientAddr);
            int connectionFd = accept(socketFd, (sockaddr *)(&clientAddr), (socklen_t *)(&addrlen));

//            auto connectionHandler = ConnectionHandler(connectionFd);
            auto connectionHandler = std::make_unique<ConnectionHandler>(connectionFd);
            auto request = connectionHandler->getRequest();

            std::cout << "INFO: " << request << '\n';

            if (request == C_CONNECT)
            {
                char* data = connectionHandler->getData();
                auto header = connectionHandler->getHeader();

                //  Get login and password as specified in protocol
                std::string login(data, header.param1);
                std::string password(data + header.param1, header.param2);

                if (authorization.login(login, password)){
                    std::cout << "Udalo sie zalogowac!\n";

                    connectionHandler->setHeader(S_CONNECT, NO_ERROR, 0);
                    connectionHandler->sendReply(true);

                    threads.emplace_back(new ConnectionThread(login, std::move(connectionHandler), running));
                }
                else{
                    std::cout << "Nie udalo sie zalogowac!\n";
                    connectionHandler->setHeader(S_CONNECT, BAD_LOGIN, 0);
                    connectionHandler->sendReply(true);
                }
            }
            else if( request == REQUEST_TIMEOUT )
            {
                std::cout << "Wystapil timeout!\n";
                connectionHandler->setHeader(S_CONNECT, CONNECTION_TIMEOUT, 0);
                connectionHandler->sendReply(true);
            }
            else {
                std::cout << "Invalid request!\n";
                connectionHandler->setHeader(S_CONNECT, INVALID_REQUEST, 0);
                connectionHandler->sendReply(true);
            }

            //  Connection socket will be closed by ConnectionHandler destructor
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

        if (action == "quit" || action == "q")
            running = false;
        else if (action == "connections" ||action == "c")
            std::cout << "Number of connections: " << threads.size() << '\n';
        else if (action == "address" || action == "a")
            std::cout << "Server address: " << address << '\n';
        else if (action == "port" || action == "p")
            std::cout << "Socket port: " << port << '\n';
    }
}

void Server::printHelp()
{
    std::cout << "Server help\n\n";
    std::cout << "quit - close server and exit\n";
    std::cout << std::endl;
}
