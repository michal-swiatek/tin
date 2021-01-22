#ifndef TIN_PROTOCOL_H
#define TIN_PROTOCOL_H

#include <cstdint>

struct Header {
    int32_t size;
    uint8_t command;
    int32_t param1;
    int32_t param2;
};

struct ProtoStruct {
    uint8_t command;
    Header header;
    uint8_t buf[];
};


#endif //TIN_PROTOCOL_H
