#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdio>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <cstring>
#include <vector>

#include "../utilities/Protocol.h"
#include "../utilities/Requests.h"
#include "../server/ConnectionHandler.h"

const std::string DATA = "user1superhaslo1";

int main()
{
    char* ip = "192.168.0.31";
    int port = 8080;
    int sock;
    struct sockaddr_in server;
    struct hostent *hp, *gethostbyname();

    /* Create socket. */
    sock = socket( AF_INET, SOCK_STREAM, 0 );
    if (sock == -1) {
        perror("opening stream socket");
        return 1;
    }



    /* uzyskajmy adres IP z nazwy . */
    server.sin_family = AF_INET;
    hp = ::gethostbyname(ip);

/* hostbyname zwraca strukture zawierajaca adres danego hosta */
    if (hp == (struct hostent *) 0) {
        fprintf(stderr, "%s: unknown host\n", ip);
        return 2;
    }
    ::memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
    server.sin_port = htons(port);
    if (connect(sock, (struct sockaddr *) &server, sizeof server)
        == -1) {
        perror("connecting stream socket");
        return 1;
    }

    ConnectionHandler connectionHandler(sock, {0, 0});

    connectionHandler.setData(DATA.c_str(), DATA.length());

    connectionHandler.setHeader(C_CONNECT, 5, 11);

    connectionHandler.sendRequest();

    Request request = connectionHandler.getRequest();

    Header h = connectionHandler.getHeader();

    char* data = connectionHandler.getData();

    std::cout<<data<<'\n';

//    int size = DATA.length() + sizeof(Header);
//    char* buff = new char[size];
//
//    Header header;
//    header.size = DATA.length();
//    header.command = C_CONNECT;
//    header.param1 = 5;
//    header.param2 = DATA.length()-5;
//
//    memcpy(buff, &header, sizeof(header));
//    memcpy(buff+ sizeof(Header), DATA.c_str(), DATA.length());
//
//    std::cout<<DATA<<'\n';
//
//    if (::send( sock,  buff , size , 0) == -1)
//        perror("writing on stream socket");
//
//
//    char buffer[1024];
//
//    int bytesRead;
//    int bytesToRead;
//    int totalBytesRead = 0;
//    std::vector<char> data;
//
//    int offset = 0;
//
//    while ((bytesRead = recv(sock, (&header)+offset, sizeof(header)-offset, 0)) > 0)
//    {
//        if (bytesRead == -1)
//        {
//            std::cout << strerror(errno) << '\n';
//            exit(3);
//        }
//
//        offset += bytesRead;
//
//        if (offset >= sizeof(header))
//            break;
//    }
//
//    bytesToRead = header.size;
//    if(bytesToRead > 0){
//        while ((bytesRead = recv(sock, buffer, sizeof(buffer), 0)) > 0)
//        {
//            if (bytesRead == -1)
//            {
//                std::cout << strerror(errno) << '\n';
//                exit(3);
//            }
//
//            data.insert(data.end(), buffer, buffer + bytesRead);
//
//            totalBytesRead += bytesRead;
//
//            if (totalBytesRead >= bytesToRead)
//                break;
//        }
//    }
//
//    std::cout << data.size() << ' ' << std::string(data.data(), data.size()) << '\n';
//
//    ::close(sock);
    return 0;
}