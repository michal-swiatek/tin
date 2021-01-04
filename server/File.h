#ifndef TIN_FILE_H
#define TIN_FILE_H

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include "FilesMonitor.h"

class File {
public:
    enum Flags {
        READ_ONLY = O_RDONLY,
        WRITE_ONLY = O_WRONLY,
        READ_WRITE = O_RDWR,
        CREATE = O_CREAT
    };

    File(const std::string &diskPath, const std::string &filePath, Flags flags, const std::string &user,
         FilesMonitor &filesMonitor);

    ~File();

    File(const File&) = delete;
    File& operator=(const File&) = delete;

    int read(char *buffer, int size) const;

    int write(const char *buffer, int size) const;

    int lseek(int offset, int whence) const;

    int fstat(struct stat *stat) const;

    [[nodiscard]] inline int getFD() const { return descriptor; }

    [[nodiscard]] inline const std::string &getUser() const { return user; }

private:
    FilesMonitor &filesMonitor;
    int descriptor;
    std::string user;
    std::string path;
    Flags flags;
};

#endif //TIN_FILE_H
