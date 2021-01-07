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
