//
// Created by micho6 on 03.01.2021.
//

#include "Directory.h"

#include "ServerExceptions.h"

Directory::Directory(int fd, DIR *dir) : descriptor(fd), dir(dir) { }

char* Directory::read()
{
    return ::readdir(this->dir)->d_name;
}
