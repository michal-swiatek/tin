#ifndef TIN_ERRORS_H
#define TIN_ERRORS_H

//  TODO: implement more errors
//  TODO: zaimplementowac kody bledow klienta

enum Errors {
    NO_ERROR = 0,
    OTHER_ERROR=1,
    FILE_NOT_EXISTS ,
    INVALID_DESCRIPTOR,
    NOT_OPEN_DESCRIPTOR,
    INVALID_FLAG_VALUE,
    BAD_LOGIN,
    FOLDER_NOT_EXISTS,
    FILE_NOT_PERMITED,
    FILE_WRITE_ONLY,
    FILE_READ_ONLY,
    FILE_NOT_UNLINKED,

    INVALID_COMMAND,
    SERVER_ERROR
};

#endif //TIN_ERRORS_H
