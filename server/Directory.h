//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_DIRECTORY_H
#define TIN_DIRECTORY_H

#include <dirent.h>

class Directory
{
public:
    Directory(int fd, DIR* dir);

    char* read();

    [[nodiscard]] inline int getFD() const { return descriptor; }

private:
    int descriptor;
    DIR* dir;
};

#endif //TIN_DIRECTORY_H
