#ifndef TIN_CONNECTIONHANDLER_H
#define TIN_CONNECTIONHANDLER_H

#include <sys/socket.h>

#include <vector>
#include <string>

#include "../utilities/Requests.h"
#include "../utilities/Errors.h"
#include "../utilities/Protocol.h"

class ConnectionHandler {
public:
    explicit ConnectionHandler(int connectionFd, const timeval &timeout = {1, 0});

    ~ConnectionHandler();

    [[nodiscard]] Request getReply();

    [[nodiscard]] Request getRequest();

    void sendReply(bool clearData = false);

    void sendReply(Request command, int32_t param1, int32_t param2) {
        setHeader(command, param1, param2);
        sendReply();
    }

    void sendRequest(bool clearData = false);

    [[nodiscard]] Header getHeader() const { return header; }

    char *getData() { return data.data(); } //  Skip command and header
    [[nodiscard]] inline size_t dataSize() const { return data.size(); }

    void setHeader(Request command, int32_t param1, int32_t param2) {
        header = {0, static_cast<uint8_t>(command), param1, param2};
    }

    void setHeader(Header h) { header = h; }

    void setData(const char *buffer, size_t size) { data = std::vector<char>(buffer, buffer + size); }

private:
    int connectionFd{};

    fd_set ready{};
    timeval timeout{};

    Header header{};
    std::vector<char> data;

    Request parseRequest();
};

#endif //TIN_CONNECTIONHANDLER_H
