#ifndef TIN_REQUESTS_H
#define TIN_REQUESTS_H

// TODO: undefined command

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

enum Request: char {
    C_OPEN_FILE = 1,
    S_OPEN_FILE = 2,

    C_READ_FILE = 3,
    S_READ_FILE = 4,

    C_WRITE_FILE = 5,
    S_WRITE_FILE = 6,

    C_FILE_LSEEK = 7,
    S_FILE_LSEEK = 8,

        C_WRITE_FILE,
        S_WRITE_FILE,

        C_UNLINK_FILE,
        S_UNLINK_FILE,

        C_FILE_STAT,
        S_FILE_STAT,

        C_FILE_LSEEK,
        S_FILE_LSEEK,

        C_CLOSE_FILE,
        S_CLOSE_FILE,

        C_OPEN_DIR,
        S_OPEN_DIR,

        C_READ_DIR,
        S_READ_DIR,

        C_CLOSE_DIR,
        S_CLOSE_DIR,

        REPEAT
};

#endif //TIN_REQUESTS_H
