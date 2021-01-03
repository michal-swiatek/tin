#include "File.h"

#include "ServerExceptions.h"

File::File(int fd, std::string  user) : descriptor(fd), user(std::move(user)) { }

int File::read(char *buffer, int size) const
{
    return ::read(this->descriptor, buffer, size);
}

int File::write(const char *buffer, int size) const
{
    return ::write(this->descriptor, buffer, size);
}

int File::lseek(int offset, int whence) const
{
    return ::lseek(this->descriptor, offset, whence);
}

int File::fstat(struct stat *stat) const
{
    return ::fstat(this->descriptor, stat);
}
