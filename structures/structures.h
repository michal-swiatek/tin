#ifndef TIN_STRUCTURES_H
#define TIN_STRUCTURES_H

#include <cstdint>

const int CLIENT_OPEN_FILE=1;
const int SERVER_OPEN_FILE=2;

const int CLIENT_READ_FILE=3;
const int SERVER_READ_FILE=4;

const int CLIENT_WRITE_FILE=5;
const int SERVER_WRITE_FILE=6;

const int CLIENT_LSEEK_FILE=7;
const int SERVER_LSEEK_FILE=8;

const int CLIENT_CLOSE_FILE=9;
const int SERVER_CLOSE_FILE=10;

const int CLIENT_UNLINK_FILE=11;
const int SERVER_UNLINK_FILE=12;

const int CLIENT_STAT_FILE=13;
const int SERVER_STAT_FILE=14;

const int CLIENT_OPEN_SESSION=15;
const int SERVER_OPEN_SESSION=16;

const int CLIENT_CLOSE_SESSION=17;
const int SERVER_CLOSE_SESSION=18;

const int CLIENT_OPEN_DIR=19;
const int SERVER_OPEN_DIR=20;

const int CLIENT_READ_DIR=21;
const int SERVER_READ_DIR=22;

const int CLIENT_CLOSE_DIR=23;
const int SERVER_CLOSE_DIR=24;
=======
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
>>>>>>> 10c3f71d2f0c5a553e13a8b056c65338b760c704

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
