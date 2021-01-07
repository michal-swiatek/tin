//
// Created by micho6 on 03.01.2021.
//

#include "ConnectionHandler.h"

#include <unistd.h>

#include <cstring>

ConnectionHandler::ConnectionHandler(int socketFd) : socketFd(socketFd)
{
    FD_ZERO(&ready);
    timeout = {1, 0};
}

ConnectionHandler::~ConnectionHandler()
{
    close(socketFd);
}

ConnectionHandler::Request ConnectionHandler::getRequest()
{
    FD_ZERO(&ready);
    FD_SET(socketFd, &ready);

    int ret = select(socketFd + 1, &ready, nullptr, nullptr, &timeout);

    if (FD_ISSET(socketFd, &ready))
        return parseRequest();
    else
        return REPEAT;
}

void ConnectionHandler::sendReply(Request command, int32_t param1, int32_t param2)
{
    char buffer[9];
    memcpy(buffer, &command, sizeof(char));
    memcpy(buffer + 1, &param1, sizeof(int));
    memcpy(buffer + 5, &param2, sizeof(int));

    data.insert(data.begin(), buffer, buffer + 9);
    send(socketFd, data.data(), data.size(), 0);
}

ConnectionHandler::Request ConnectionHandler::parseRequest()
{
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    //  Read 1 byte command + 8 bytes header
    int bytesRead = 0;
    int totalRead = 0;
    data.clear();

    while ((bytesRead = recv(socketFd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        totalRead += bytesRead;

        data.insert(data.end(), buffer, buffer + bytesRead);
    }

    //  Parse header and command
    Request command;
    memcpy(&command, buffer, sizeof(char));
    memcpy(&header.param1, buffer + 1, sizeof(int32_t));
    memcpy(&header.param1, buffer + 5, sizeof(int32_t));

    //  TODO: switch(command) instead of copy

    return command;
}
