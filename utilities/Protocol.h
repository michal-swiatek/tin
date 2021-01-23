#ifndef TIN_PROTOCOL_H
#define TIN_PROTOCOL_H

#include <cstdint>

const int DEFAULT_PORT = 8080;

const int CLIENT_SEND_TIMEOUT = 0;

const int CLIENT_RECV_TIMEOUT = 0;

struct Header {
    int32_t size;
    uint8_t command;
    int32_t param1;
    int32_t param2;
};

#endif //TIN_PROTOCOL_H