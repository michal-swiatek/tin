#ifndef TIN_ERRORS_H
#define TIN_ERRORS_H

//  TODO: implement more errors
//  TODO: zaimplementowac kody bledow klienta


enum Errors {
    NO_ERROR = 0,

    OTHER_ERROR = 1,

    FILE_NOT_EXISTS = 2,

    INVALID_DESCRIPTOR = 3,

    INVALID_FLAG_VALUE = 4,

    BAD_LOGIN = 5,

    DIRECTORY_END_REACHED = 6,

    FILE_NOT_PERMITED = 7,

    FILE_WRITE_ONLY = 8,

    FILE_READ_ONLY = 9,

    FILE_NOT_UNLINKED = 10,

    DIRECTORY_NOT_OPENED = 11,

    INVALID_CONNECTION = 12,

    CONNECTION_TIMEOUT = 13,

    COULD_NOT_CREATE_SOCKET = 14,

    INET_PTON_ERROR = 15,

    INVALID_COMMAND = 16,

    SERVER_ERROR = 17
};

#endif //TIN_ERRORS_H