#ifndef TIN_FILE_H
#define TIN_FILE_H

#include <sys/stat.h>
#include <unistd.h>
#include <string>

class File
{
public:
    File(int fd, std::string user);

    int read(char* buffer, int size) const;
    int write(const char* buffer, int size) const;
    int lseek(int offset, int whence) const;
    int fstat(struct stat* stat) const;

    [[nodiscard]] inline int getFD() const { return descriptor; }
    [[nodiscard]] inline const std::string& getUser() const { return user; }

private:
    int descriptor;
    std::string user;
};

#endif //TIN_FILE_H
