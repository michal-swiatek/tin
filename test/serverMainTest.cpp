//
// Created by micho6 on 09.01.2021.
//

#include "../utilities/Protocol.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <cstring>
#include <vector>

int main() {

    int socketFd;
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Opening stream socket");
        exit(1);
    }

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

    //  Print server port
    int length = sizeof(serverAddress);
    if (getsockname(socketFd, (sockaddr *)(&serverAddress), (socklen_t *)(&length)) == -1)
    {
        perror("Getting socket name");
        exit(2);
    }

    auto address = inet_ntoa(serverAddress.sin_addr);
    int port = serverAddress.sin_port;

    std::cout << address << ' ' << port << '\n';

    listen(socketFd, 5);
    fd_set ready;
    struct timeval timeout{1, 0};

    std::vector<char> data;

    //
    //  Server
    //

    while (true)
    {
        FD_ZERO(&ready);
        FD_SET(socketFd, &ready);

        int ret = select(socketFd + 1, &ready, nullptr, nullptr, &timeout);

        if (FD_ISSET(socketFd, &ready))
        {
            struct sockaddr_in clientAddr{};
            int addrlen = sizeof(clientAddr);
            int connectionFd = accept(socketFd, (sockaddr *)(&clientAddr), (socklen_t *)(&addrlen));

            if (setsockopt(connectionFd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
                perror("Set socket receive timeout");

            if (connectionFd == -1)
            {
                std::cout << strerror(errno) << '\n';
                //  TODO: return Error invalid connection
                exit(1);
            }




            //
            //  Handler - getRequest
            //

            data.clear();

            Header header{};
            char buffer[1024];

            int bytesRead;
            int bytesToRead;
            int totalBytesRead = 0;

            int offset = 0;

            while ((bytesRead = recv(connectionFd, (&header)+offset, sizeof(header) - offset, 0)) > 0)
            {
                offset += bytesRead;

                if (offset >= sizeof(header))
                    break;
            }

            if (bytesRead == -1)
            {
                std::cout << strerror(errno) << '\n';
                exit(3);    //  TODO: return
            }

            std::cout<<header.command<<'\n';
            std::cout<<header.param1<<'\n';
            std::cout<<header.param2<<'\n';

            std::cout<< ntohl(header.param2) <<'\n';

            bytesToRead = header.param1;

            while ((bytesRead = recv(connectionFd, buffer, sizeof(buffer), 0)) > 0)
            {
                if (bytesRead == -1)
                {
                    std::cout << strerror(errno) << '\n';
                    exit(3);
                }

                data.insert(data.end(), buffer, buffer + bytesRead);

                totalBytesRead += bytesRead;

                if (totalBytesRead >= bytesToRead)
                    break;
            }

            if (bytesRead == -1)
            {
                std::cout << strerror(errno) << '\n';
                exit(3);    //  TODO: return
            }

            std::cout << data.size() << ' ' << std::string(data.data(), data.size()) << '\n';








            //
            //  Handler - send Reply
            //

            std::string temp = "micho6";

            int size = temp.length() + sizeof(Header);
            char* buff = new char[size];

            header.param1 = size;
            header.param2 = temp.length();

            memcpy(buff, &header, sizeof(header));
            memcpy(buff+ sizeof(Header), temp.c_str(), temp.length());

            if (send(connectionFd, buff, size, 0) < 0)
                perror("writing on stream socket");
        }
    }

}
