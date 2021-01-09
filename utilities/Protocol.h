#ifndef TIN_PROTOCOL_H
#define TIN_PROTOCOL_H

#include <cstdint>

// TODO: polaczyc Header z pozostaymi strukturami

// TODO: polaczyc withData z withoutData?

struct ProtoStructWithoutData {
    uint8_t command;
    uint32_t header1;
    uint32_t header2;
};

struct ProtoStructWithData {
    uint8_t command;
    uint32_t header1;
    uint32_t header2;
    uint8_t buf[];
};

struct Header {
    int32_t param1;
    int32_t param2;
};

#endif //TIN_PROTOCOL_H
