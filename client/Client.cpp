#include "Client.h"

#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>

Client::Client() {
    sockfd=-1;
    mynfs_error = NO_ERROR;
}

Client::~Client()= default;

int Client::mynfs_opensession(const char *host, const char *login, const char *passwd){

    struct sockaddr_in serv_addr{};

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        this->mynfs_error = COULD_NOT_CREATE_SOCKET;
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);

    if(inet_pton(AF_INET, host, &serv_addr.sin_addr)<=0) {
        this->mynfs_error = COULD_NOT_CREATE_SOCKET;
        return -1;
    }

    struct timeval tv{};
    tv.tv_sec = CLIENT_RECV_TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    tv.tv_sec = CLIENT_SEND_TIMEOUT;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv));

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        this->mynfs_error = SERVER_ERROR;
        return -1;
    }

    Header header{};
    header.command = C_CONNECT;

    std::string log(login);
    std::string pas(passwd);
    std::string logPas = log+pas;

    header.size = logPas.size();
    header.param1 = log.size();
    header.param2 = pas.size();

    std::vector<char> data;

    for( char c : logPas ){
        data.emplace_back(c);
    }

    sendMessage(header, data);

    if( header.command != C_CONNECT) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);

    if( header.command == S_CONNECT) {
        if( header.param1 == NO_ERROR){
            return 0;
        } else if( header.param1 == BAD_LOGIN ){
            mynfs_error = BAD_LOGIN;
            return -1;
        } else if( header.param1 == INVALID_COMMAND ){
            mynfs_error = INVALID_COMMAND;
            return -1;
        } else if( header.param1 == CONNECTION_TIMEOUT ){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else{
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client::mynfs_closesession(){
    Header header{};
    header.command = C_DISCONNECT;
    header.param1 = 0;
    header.param2 = 0;
    header.size = 0;

    std::vector<char> data;

    sendMessage(header, data);

    if( header.command != C_DISCONNECT) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);

    if( header.command == S_DISCONNECT) {
        if( header.param1 == NO_ERROR ){
            close(sockfd);
            return 0;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client::mynfs_open(char* path, FileFlag oflag ){

    std::string filePath(path);

    Header header{};
    header.command = C_OPEN_FILE;
    header.param1 = filePath.length();
    header.param2 = oflag;
    header.size = filePath.length();

    std::vector<char> data;

    for( char c : filePath ){
        data.emplace_back(c);
    }

    sendMessage(header, data);

    if( header.command != C_OPEN_FILE) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);

    if( header.command == S_OPEN_FILE) {
        if( header.param1 == NO_ERROR ){
            return header.param2;
        } else if( header.param1 == FILE_NOT_EXISTS || header.param1 == INVALID_FLAG_VALUE || header.param1 == FILE_NOT_PERMITED ){
            mynfs_error = static_cast<Errors>(header.param1);
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client:: mynfs_read(int fd, char *buf, int size){
    Header header{};
    header.command = C_READ_FILE;
    header.param1 = size;
    header.param2 = fd;
    header.size = 0;

    std::vector<char> data;

    sendMessage(header, data);

    if( header.command != C_READ_FILE ) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);


    if( header.command == S_READ_FILE) {
        if( header.param1 == NO_ERROR ){
            int dataSize = header.param2;
            memcpy(buf, data.data(), dataSize);
            return dataSize;
        } else if( header.param1 == FILE_WRITE_ONLY || header.param1 == INVALID_DESCRIPTOR ){
            mynfs_error = static_cast<Errors>(header.param1);
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}



int Client::mynfs_write(int fd, const char *buf, int size) {
    Header header{};
    header.command = C_WRITE_FILE;
    header.param1 = size;
    header.param2 = fd;
    header.size = size;

    std::vector<char> data(buf, buf + size);

    sendMessage(header, data);

    if( header.command != C_WRITE_FILE ) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);


    if( header.command == S_WRITE_FILE) {
        if( header.param1 == NO_ERROR ){
            return header.param2;
        } else if( header.param1 == FILE_READ_ONLY || header.param1 == INVALID_DESCRIPTOR ){
            mynfs_error = static_cast<Errors>(header.param1);
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client::mynfs_lseek(int fd, int offset, Whence whence) {
    Header header{};
    header.command = C_FILE_LSEEK;
    header.param1 = offset;
    header.param2 = fd;
    header.size = sizeof(whence)/sizeof(char);
    std::vector<char> data((char*)&whence, (char*)&whence+header.size);

    sendMessage(header, data);

    if( header.command != C_FILE_LSEEK ) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);


    if( header.command == S_FILE_LSEEK) {
        if( header.param1 == NO_ERROR ){
            return header.param2;
        } else if( header.param1 == INVALID_FLAG_VALUE || header.param1 == INVALID_DESCRIPTOR ){
            mynfs_error = static_cast<Errors>(header.param1);
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client::mynfs_close(int fd) {
    Header header{};
    header.command = C_CLOSE_FILE;
    header.param1 = fd;
    header.param2 = 0;
    header.size = 0;

    std::vector<char> data;

    sendMessage(header, data);

    if( header.command != C_CLOSE_FILE) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);


    if( header.command == S_CLOSE_FILE) {
        if( header.param1 == NO_ERROR ){
            return 0;
        } else if( header.param1 == INVALID_DESCRIPTOR ){
            mynfs_error = INVALID_DESCRIPTOR;
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client::mynfs_unlink(char* path){
    std::string filePath(path);

    Header header{};
    header.command = C_UNLINK_FILE;
    header.param1 = filePath.length();
    header.param2 = 0;
    header.size = filePath.length();

    std::vector<char> data;

    for(char c : filePath){
        data.emplace_back(c);
    }

    sendMessage(header, data);

    if( header.command != C_UNLINK_FILE ) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);

    if( header.command == S_UNLINK_FILE) {
        if( header.param1 == NO_ERROR ){
            return 0;
        } else if( header.param1 == FILE_NOT_EXISTS || header.param1 == FILE_NOT_PERMITED || header.param1 == FILE_NOT_UNLINKED ){
            mynfs_error = static_cast<Errors>(header.param1);
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client::mynfs_fstat(int fd, FileStat *buf) {
    Header header{};
    header.command = C_FILE_STAT;
    header.param1 = fd;
    header.param2 = 0;
    header.size = 0;

    std::vector<char> data;

    sendMessage(header, data);

    if( header.command != C_FILE_STAT ) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);


    if( header.command == S_FILE_STAT ) {
        if( header.param1 == NO_ERROR && header.param2 == sizeof(FileStat) ){
            memcpy(buf, data.data(), header.size);
            return 0;
        } else if( header.param1 == INVALID_DESCRIPTOR ){
            mynfs_error = INVALID_DESCRIPTOR;
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

int Client::mynfs_opendir( char *path){
    std::string dirPath(path);

    Header header{};
    header.command = C_OPEN_DIR;
    header.param1 = dirPath.length();
    header.param2 = 0;
    header.size = dirPath.length();

    std::vector<char> data;

    for(char c : dirPath){
        data.emplace_back(c);
    }

    sendMessage(header, data);

    if( header.command != C_OPEN_DIR ) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);

    if( header.command == S_OPEN_DIR) {
        if( header.param1 == NO_ERROR ){
            return header.param2;
        } else if( header.param1 == DIRECTORY_NOT_OPENED ){
            mynfs_error = DIRECTORY_NOT_OPENED;
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    }else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

char *Client::mynfs_readdir(int dir_fd){
    Header header{};
    header.command = C_READ_DIR;
    header.param1 = dir_fd;
    header.param2 = 0;
    header.size = 0;

    std::vector<char> data;

    sendMessage(header, data);

    if( header.command != C_READ_DIR) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return nullptr;
        } else {
            mynfs_error = SERVER_ERROR;
            return nullptr;
        }
    }

    readMessage(header, data);

    if( header.command == S_READ_DIR) {
        if( header.param1 == NO_ERROR ){
            char* fileName = new char[header.param2 + 1];
            memcpy(fileName, data.data(), header.param2);
            fileName[header.param2 ] = '\0';
            return fileName;
        } else if( header.param1 == INVALID_DESCRIPTOR ){
            mynfs_error = INVALID_DESCRIPTOR;
            return nullptr;
        } else if( header.param1 == DIRECTORY_END_REACHED ){
            mynfs_error = DIRECTORY_END_REACHED;
            return nullptr;
        } else {
            mynfs_error = OTHER_ERROR;
            return nullptr;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return nullptr;
    } else {
        mynfs_error = SERVER_ERROR;
        return nullptr;
    }
}

int Client::mynfs_closedir(int dir_fd){
    Header header{};
    header.command = C_CLOSE_DIR;
    header.param1 = dir_fd;
    header.param2 = 0;
    header.size = 0;

    std::vector<char> data;

    sendMessage(header, data);

    if( header.command != C_CLOSE_DIR) {
        if(header.command == REQUEST_TIMEOUT){
            mynfs_error = CONNECTION_TIMEOUT;
            return -1;
        } else {
            mynfs_error = SERVER_ERROR;
            return -1;
        }
    }

    readMessage(header, data);

    if( header.command == S_CLOSE_DIR) {
        if( header.param1 == NO_ERROR ){
            return 0;
        } else if( header.param1 == INVALID_DESCRIPTOR ){
            mynfs_error = INVALID_DESCRIPTOR;
            return -1;
        } else {
            mynfs_error = OTHER_ERROR;
            return -1;
        }
    } else if(header.command == REQUEST_TIMEOUT){
        mynfs_error = CONNECTION_TIMEOUT;
        return -1;
    } else {
        mynfs_error = SERVER_ERROR;
        return -1;
    }
}

void Client::sendMessage(Header& header, std::vector<char> &data) const {
    char buffer[1024];

    int bytesSent;

    int offset = 0;

    while ((bytesSent = send(sockfd, (&header) + offset, sizeof(header) - offset, 0)) > 0)
    {
        if (bytesSent == -1)
        {
            header.param1 = 0;
            header.param2 = 0;
            header.size = 0;
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                header.command = REQUEST_TIMEOUT;
            }else{
                header.command = INVALID_REQUEST;
            }
            return;
        }

        offset += bytesSent;

        if (offset >= sizeof(header))
            break;
    }
    int totalBytesToSend = header.size;
    int currBytesToSend = sizeof(buffer);
    int bytesOffset = 0;
    if(totalBytesToSend > 0){
        while (totalBytesToSend > 0)
        {
            if(totalBytesToSend < sizeof(buffer)){
                currBytesToSend = totalBytesToSend;
            }

            memcpy(buffer, data.data()+bytesOffset, currBytesToSend);

            bytesSent = send(sockfd, buffer, currBytesToSend, 0);

            if (bytesSent == -1)
            {
                header.param1 = 0;
                header.param2 = 0;
                header.size = 0;
                if(errno == EAGAIN || errno == EWOULDBLOCK){
                    header.command = REQUEST_TIMEOUT;
                }else{
                    header.command = INVALID_REQUEST;
                }
                return;
            }

            bytesOffset += bytesSent;

            totalBytesToSend -= bytesSent;

        }
    }
}

void Client::readMessage(Header &header, std::vector<char> &data) const {
    data.clear();
    memset(&header, 0, sizeof(Header));

    char buffer[1024];

    int bytesRead;
    int bytesToRead;
    int totalBytesRead = 0;

    int offset = 0;

    while ((bytesRead = recv(sockfd, (&header)+offset, sizeof(header)-offset, 0)) > 0)
    {
        if (bytesRead == -1)
        {
            header.param1 = 0;
            header.param2 = 0;
            header.size = 0;
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                header.command = REQUEST_TIMEOUT;
            }else{
                header.command = INVALID_REQUEST;
            }
            return;
        }

        offset += bytesRead;

        if (offset >= sizeof(header))
            break;
    }

    bytesToRead = header.size;
    if(bytesToRead > 0){
        while ((bytesRead = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)
        {
            if (bytesRead == -1)
            {
                header.param1 = 0;
                header.param2 = 0;
                header.size = 0;
                if(errno == EAGAIN || errno == EWOULDBLOCK){
                    header.command = REQUEST_TIMEOUT;
                }else{
                    header.command = INVALID_REQUEST;
                }
                return;
            }

            data.insert(data.end(), buffer, buffer + bytesRead);

            totalBytesRead += bytesRead;

            if (totalBytesRead >= bytesToRead)
                break;
        }
    }
}

const char *mynfs_strerror(Errors error) {
    // TODO: lepsze komunikaty bledow
    switch (error)
    {
        case NO_ERROR:                  return "NO_ERROR";
        case OTHER_ERROR:               return "OTHER_ERROR";
        case FILE_NOT_EXISTS:           return "FILE_NOT_EXISTS";
        case INVALID_DESCRIPTOR:        return "INVALID_DESCRIPTOR";
        case INVALID_FLAG_VALUE:        return "INVALID_FLAG_VALUE";
        case BAD_LOGIN:                 return "BAD_LOGIN";
        case DIRECTORY_END_REACHED:     return "DIRECTORY_END_REACHED";
        case FILE_NOT_PERMITED:         return "FILE_NOT_PERMITED";
        case FILE_WRITE_ONLY:           return "FILE_WRITE_ONLY";
        case FILE_READ_ONLY:            return "FILE_READ_ONLY";
        case FILE_NOT_UNLINKED:         return "FILE_NOT_UNLINKED";
        case DIRECTORY_NOT_OPENED:      return "DIRECTORY_NOT_OPENED";
        case INVALID_CONNECTION:        return "INVALID_CONNECTION";
        case CONNECTION_TIMEOUT:        return "CONNECTION_TIMEOUT";
        case COULD_NOT_CREATE_SOCKET:   return "COULD_NOT_CREATE_SOCKET";
        case INET_PTON_ERROR:           return "INET_PTON_ERROR";
        case INVALID_COMMAND:           return "INVALID_COMMAND";
        case SERVER_ERROR:              return "SERVER_ERROR";
        default:                        return "UNKNOWN_ERROR";
    }
}
