//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_FILE_H
#define TIN_FILE_H

#include <sys/stat.h>
#include <string>

class File
{
public:
    File(int fd, std::string user);

    void read(char* buffer, int size) const;
    void write(const char* buffer, int size) const;
    void lseek(int offset, int whence) const;
    void fstat(struct stat* stat) const;

    [[nodiscard]] inline int getFD() const { return descriptor; }
    [[nodiscard]] inline const std::string& getUser() const { return user; }

private:
    int descriptor;
    std::string user;
};

#endif //TIN_FILE_H
