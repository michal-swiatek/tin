#ifndef TIN_REQUESTS_H
#define TIN_REQUESTS_H

// TODO: undefined command

enum Request: char {
    C_OPEN_FILE = 1,
    S_OPEN_FILE = 2,

    C_READ_FILE = 3,
    S_READ_FILE = 4,

    C_WRITE_FILE = 5,
    S_WRITE_FILE = 6,

    C_FILE_LSEEK = 7,
    S_FILE_LSEEK = 8,

    C_CLOSE_FILE = 9,
    S_CLOSE_FILE = 10,

    C_UNLINK_FILE = 11,
    S_UNLINK_FILE = 12,

    C_FILE_STAT = 13,
    S_FILE_STAT = 14,

    C_CONNECT = 15,
    S_CONNECT = 16,

    C_DISCONNECT = 17,
    S_DISCONNECT= 18,

    C_OPEN_DIR = 19,
    S_OPEN_DIR = 20,

    C_READ_DIR = 21,
    S_READ_DIR = 22,

    C_CLOSE_DIR = 23,
    S_CLOSE_DIR = 24,

    REPEAT
};

#endif //TIN_REQUESTS_H
