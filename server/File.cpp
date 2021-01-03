//
// Created by micho6 on 03.01.2021.
//

#include "File.h"

#include "ServerExceptions.h"

File::File(int fd, std::string  user) : descriptor(fd), user(std::move(user)) { }

void File::read(char *buffer, int size) const
{
    throw NotImplementedError();
}

void File::write(const char *buffer, int size) const
{
    throw NotImplementedError();
}

void File::lseek(int offset, int whence) const
{
    throw NotImplementedError();
}

void File::fstat(struct stat *stat) const
{
    throw NotImplementedError();
}
