#include <fcntl.h>
#include <iostream>
#include <cstring>
#include "File.h"

#include "../ServerExceptions.h"

File::File(const std::string &diskPath, const std::string &filePath, int flags, const std::string &user,
           FilesMonitor &filesMonitor)
        : path(filePath), user(user), filesMonitor(filesMonitor), flags(flags) {
    std::string fullPath = diskPath + filePath;
    int flag = flags;
    if (flags == O_CREAT) {
        flag = flag | O_RDWR;
    }
    if ((descriptor = open(fullPath.c_str(), flag)) == -1) {
        std::cout << fullPath << '\n';
        throw FileNotOpened();
    }
    filesMonitor.add(filePath, user);
}

File::~File() {
    close(descriptor);
    filesMonitor.remove(path);
}

int File::read(char *buffer, int size) const {
    if (this->flags == O_WRONLY) {
        throw FileWriteOnly();
    }
    return ::read(this->descriptor, buffer, size);
}

int File::write(const char *buffer, int size) const {
    if (this->flags == O_RDONLY) {
        throw FileReadOnly();
    }
    return ::write(this->descriptor, buffer, size);
}

int File::lseek(int offset, int whence) const {
    return ::lseek(this->descriptor, offset, whence);
}

int File::fstat(FileStat *fileStat) const {
    auto *stat = new struct stat();
    int ret = ::fstat(this->descriptor, stat);

    fileStat->size = stat->st_size;
    memcpy(fileStat->filePath, this->path.c_str(), this->path.size());
    memcpy(fileStat->owner, this->user.c_str(), this->user.size());
    fileStat->lastAccess = stat->st_atime;
    fileStat->lastStatusChange = stat->st_ctime;
    fileStat->lastModification = stat->st_mtime;

    fileStat->flag = this->flags;

    return ret;
}