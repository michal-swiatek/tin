#include <fcntl.h>
#include "File.h"

#include "ServerExceptions.h"

File::File(const std::string &diskPath, const std::string &filePath, Flags flags, const std::string &user,
           FilesMonitor &filesMonitor)
        : path(filePath), user(user), filesMonitor(filesMonitor), flags(flags) {
    // TODO: obsluga bledow
    // TODO: rozne tryby otwierania
    filesMonitor.add(filePath, user);
    std::string fullPath = diskPath + filePath;
    int flag = flags;
    if(flags == Flags::CREATE){
        flag = flag | O_RDWR;
    }
    if((descriptor = open(fullPath.c_str(), flag)) == -1){
        // TODO: blad otwarcia
        throw std::exception();
    }
}

File::~File() {
    // TODO: co jak nie ma pliku
    // TODO: czy kazdy moze zamknac?
    close(descriptor);
    filesMonitor.remove(path);
}

int File::read(char *buffer, int size) const {
    // TODO: co jak nie mozesz czytac
    if(this->flags == Flags::WRITE_ONLY){
        throw std::exception();
    }
    return ::read(this->descriptor, buffer, size);
}

int File::write(const char *buffer, int size) const {
    // TODO: co jak nie mozesz pisac
    if(this->flags == Flags::READ_ONLY){
        throw std::exception();
    }
    return ::write(this->descriptor, buffer, size);
}

int File::lseek(int offset, int whence) const {
    return ::lseek(this->descriptor, offset, whence);
}

int File::fstat(struct stat *stat) const {
    return ::fstat(this->descriptor, stat);
}