//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_CONNECTIONHANDLER_H
#define TIN_CONNECTIONHANDLER_H

#include <sys/socket.h>

#include <vector>
#include <string>

class ConnectionHandler
{
public:
    enum Request: char { C_CONNECT, C_DISCONNECT, S_DISCONNECT, C_OPEN_FILE, S_OPEN_FILE, C_READ_FILE, S_READ_FILE, C_WRITE_FILE, S_WRITE_FILE, C_UNLINK_FILE, S_UNLINK_FILE,
                    C_FILE_STAT, S_FILE_STAT, C_FILE_LSEEK, S_FILE_LSEEK, C_CLOSE_FILE, S_CLOSE_FILE, C_OPEN_DIR, S_OPEN_DIR, C_READ_DIR, S_READ_DIR, C_CLOSE_DIR, S_CLOSE_DIR, REPEAT };

    enum Replies { OK };    //  TODO: implement/find usages

    enum Errors { INVALID_COMMAND, SERVER_ERROR };  //  TODO: implement more errors

    struct Header
    {
        int32_t param1;
        int32_t param2;
    };

public:
    explicit ConnectionHandler(int socketFd);
    ~ConnectionHandler();

    [[nodiscard]] Request getRequest();
    void sendReply(Request command, int32_t param1, int32_t param2);

    [[nodiscard]] Header getHeader() const { return header; }

    char* getData() { return data.data() + 9; } //  Skip command and header
    void  setData(char* buffer, size_t size) { data = std::vector<char>(buffer, buffer + size); }
    [[nodiscard]] inline size_t dataSize() const { return data.size(); }

private:
    int socketFd{};

    fd_set ready{};
    timeval timeout{};

    Header header{};
    std::vector<char> data;

    Request parseRequest();
};

#endif //TIN_CONNECTIONHANDLER_H
