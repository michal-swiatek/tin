#ifndef TIN_STRUCTURES_H
#define TIN_STRUCTURES_H

#include <cstdint>

#define ClientOpenFile 1
#define ServerOpenFile 2

#define ClientReadFile 3
#define ServerReadFile 4

#define ClientWriteFile 5
#define ServerWriteFile 6

#define ClientLseekFile 7
#define ServerLseekFile 8

#define ClientCloseFile 9
#define ServerCloseFile 10

#define ClientUnlinkFile 11
#define ServerUnlinkFile 12

#define ClientStatFile 13
#define ServerStatFile 14

#define ClientOpenSession 15
#define ServerOpenSession 16

#define ClientCloseSession 17
#define ServerCloseSession 18

#define ClientOpenDir 19
#define ServerOpenDir 20

#define ClientReadDir 21
#define ServerReadDir 22

#define ClientCloseDir 23
#define ServerCloseDir 24

struct ProtoStructWithoutData {
    uint8_t command;
    int32_t header1;
    uint32_t header2;
};

struct ProtoStructWithData {
    uint8_t command;
    int32_t header1;
    uint32_t header2;
    uint8_t buf[];
};


#endif //TIN_STRUCTURES_H
