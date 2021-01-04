#ifndef TIN_DIRECTORY_H
#define TIN_DIRECTORY_H

#include <dirent.h>
#include <string>

class Directory
{
public:
    Directory(int descriptor, DIR* dir);

    char* read();

    [[nodiscard]] inline int getFD() const { return descriptor; }

private:
    int descriptor;
    DIR* dir;
};

#endif //TIN_DIRECTORY_H
