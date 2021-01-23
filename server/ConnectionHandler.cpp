//
// Created by micho6 on 03.01.2021.
//

#include "ConnectionHandler.h"

#include <unistd.h>

#include <cstring>
#include <iostream>

ConnectionHandler::ConnectionHandler(int connectionFd, const timeval& timeout) : connectionFd(connectionFd), timeout(timeout)
{
    FD_ZERO(&ready);

//    if (setsockopt(connectionFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
//        perror("Set connection socket receive timeout");
}

ConnectionHandler::~ConnectionHandler()
{
    std::cout << "Closing socket: " << connectionFd << '\n';
    close(connectionFd);
}

Request ConnectionHandler::getRequest()
{
    FD_ZERO(&ready);
    FD_SET(connectionFd, &ready);

    int ret = select(connectionFd + 1, &ready, nullptr, nullptr, &timeout);

    if (FD_ISSET(connectionFd, &ready))
        return parseRequest();
    else
        return REPEAT;
}

void ConnectionHandler::sendReply(bool clearData)
{
    if (clearData)
        data.clear();

    //  TODO: Create send loop

    int replySize = dataSize() + sizeof(header);
    char buffer[replySize];

    header.size = dataSize();

    memcpy(buffer, &header, sizeof(header));
    memcpy(buffer, data.data(), dataSize());

    int ret = send(connectionFd, buffer, replySize, 0);
    if (ret == 0)
        perror("Writing empty connection socket");
    else if (ret < 0)
        perror("Writing on connection socket");
}

void ConnectionHandler::sendRequest(bool clearData)
{
    sendReply(clearData);
}

Request ConnectionHandler::parseRequest()
{
    static const int BUFFER_SIZE = 1024;

    //
    //  Load header (1 byte - command, 4 bytes - param1, 4 bytes - param2)
    //

    int offset = 0;
    int bytesRead = 0;

    while ((bytesRead = recv(connectionFd, (&header) + offset, sizeof(header) - offset, 0)) > 0)
    {
        offset += bytesRead;

        if (offset >= sizeof(header))
            break;
    }

    if (bytesRead == -1)
        return REQUEST_TIMEOUT;

    //
    //  Load associated data
    //

    char buffer[BUFFER_SIZE];
    data.clear();   //  Clear previous data

    int bytesToRead = header.size;
    int totalBytesRead = 0;

    if (bytesToRead > 0)
    {
        while ((bytesRead = recv(connectionFd, buffer, sizeof(buffer), 0)) > 0)
        {
            data.insert(data.end(), buffer, buffer + bytesRead);

            totalBytesRead += bytesRead;
            if (totalBytesRead >= bytesToRead)
                break;
        }
    }

    if (bytesRead == -1)
        return REQUEST_TIMEOUT;

    return static_cast<Request>(header.command);
}

Request ConnectionHandler::getReply()
{
    return getRequest();
}
