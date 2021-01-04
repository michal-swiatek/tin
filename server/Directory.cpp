#include "Directory.h"

#include <utility>

#include "ServerExceptions.h"

Directory::Directory(int descriptor, DIR* dir) : descriptor(descriptor), dir(dir)) {}

char* Directory::read()
{
    return ::readdir(this->dir)->d_name;
}
