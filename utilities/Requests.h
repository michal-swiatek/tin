#ifndef TIN_REQUESTS_H
#define TIN_REQUESTS_H

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
