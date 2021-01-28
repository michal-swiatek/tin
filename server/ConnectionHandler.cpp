//
// Created by micho6 on 03.01.2021.
//

#include "ConnectionHandler.h"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#include <cstring>
#include <iostream>

ConnectionHandler::ConnectionHandler(int connectionFd, const timeval &timeout) : connectionFd(connectionFd),
                                                                                 timeout(timeout) {
    FD_ZERO(&ready);

//    if (setsockopt(connectionFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
//        perror("Set connection socket receive timeout");
}

ConnectionHandler::~ConnectionHandler() {
    std::cout << "Closing socket: " << connectionFd << '\n';
    close(connectionFd);
}

Request ConnectionHandler::getRequest() {
    FD_ZERO(&ready);
    FD_SET(connectionFd, &ready);

    int ret = select(connectionFd + 1, &ready, nullptr, nullptr, &timeout);

    if (FD_ISSET(connectionFd, &ready))
        return parseRequest();
    else
        return REPEAT;
}

void ConnectionHandler::sendReply(bool clearData) {
    if (clearData)
        data.clear();

    char buffer[1024];

    int bytesSent;
    int offset = 0;

    header.size = dataSize();

    while ((bytesSent = send(connectionFd, (&header) + offset, sizeof(header) - offset, 0)) > 0) {
        if (bytesSent == -1) {
            perror(std::string("Error: Sending with socker: " + std::to_string(connectionFd)).c_str());
            return;
        }

        offset += bytesSent;

        if (offset >= sizeof(header))
            break;
    }

    int totalBytesToSend = header.size;
    int currBytesToSend = sizeof(buffer);
    int bytesOffset = 0;
    if (totalBytesToSend > 0) {
        while (totalBytesToSend > 0) {
            if (totalBytesToSend < sizeof(buffer)) {
                currBytesToSend = totalBytesToSend;
            }

            memcpy(buffer, data.data() + bytesOffset, currBytesToSend);

            bytesSent = send(connectionFd, buffer, currBytesToSend, 0);

            if (bytesSent == -1) {
                perror(std::string("Error: Sending with socker: " + std::to_string(connectionFd)).c_str());
                return;
            }

            bytesOffset += bytesSent;

            totalBytesToSend -= bytesSent;

        }
    }
}

void ConnectionHandler::sendRequest(bool clearData) {
    sendReply(clearData);
}

Request ConnectionHandler::parseRequest() {
    static const int BUFFER_SIZE = 1024;

    //
    //  Load header (1 byte - command, 4 bytes - param1, 4 bytes - param2)
    //

    int offset = 0;
    int bytesRead = 0;

    while ((bytesRead = recv(connectionFd, (&header) + offset, sizeof(header) - offset, 0)) > 0) {
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

    if (bytesToRead > 0) {
        while ((bytesRead = recv(connectionFd, buffer, sizeof(buffer), 0)) > 0) {
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

Request ConnectionHandler::getReply() {
    return getRequest();
}
