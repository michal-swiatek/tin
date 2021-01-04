#include <fcntl.h>
#include "File.h"

#include "ServerExceptions.h"

File::File(const std::string &diskPath, const std::string &filePath, Flags flags, const std::string &user,
           FilesMonitor &filesMonitor)
        : path(filePath), user(user), filesMonitor(filesMonitor), flags(flags) {
    // TODO: obsluga bledow
    // TODO: rozne tryby otwierania
    std::string fullPath = diskPath + filePath;
    descriptor = open(fullPath.c_str(), flags);
    filesMonitor.add(filePath, user);
}

File::~File() {
    // TODO: co jak nie ma pliku
    // TODO: czy kazdy moze zamknac?
    close(descriptor);
    filesMonitor.remove(path);
}

int File::read(char *buffer, int size) const
{
    // TODO: co jak nie mozesz czytac
    return ::read(this->descriptor, buffer, size);
}

int File::write(const char *buffer, int size) const
{
    // TODO: co jak nie mozesz pisac
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