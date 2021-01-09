#ifndef TIN_CONNECTIONHANDLER_H
#define TIN_CONNECTIONHANDLER_H

#include <sys/socket.h>

#include <vector>
#include <string>

#include "../structures/Requests.h"
#include "../structures/Errors.h"
#include "../structures/Protocol.h"

class ConnectionHandler
{
public:
    enum Replies { OK };    //  TODO: implement/find usages

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
