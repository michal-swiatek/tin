#ifndef TIN_STRUCTURES_H
#define TIN_STRUCTURES_H

#include <cstdint>

const int ClientOpenFile=1;
const int ServerOpenFile=2;

const int ClientReadFile=3;
const int ServerReadFile=4;

const int ClientWriteFile=5;
const int ServerWriteFile=6;

const int ClientLseekFile=7;
const int  ServerLseekFile=8;

const int  ClientCloseFile=9;
const int  ServerCloseFile=10;

const int ClientUnlinkFile=11;
const int  ServerUnlinkFile=12;

const int ClientStatFile=13;
const int ServerStatFile=14;

const int ClientOpenSession=15;
const int ServerOpenSession=16;

const int  ClientCloseSession=17;
const int  ServerCloseSession=18;

const int  ClientOpenDir=19;
const int  ServerOpenDir=20;

const int  ClientReadDir=21;
const int ServerReadDir=22;

const int  ClientCloseDir=23;
const int ServerCloseDir=24;

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


#endif //TIN_STRUCTURES_H
