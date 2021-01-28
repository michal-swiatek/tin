#ifndef TIN_DIRECTORY_H
#define TIN_DIRECTORY_H

#include <dirent.h>
#include <string>
#include "DirectoriesMonitor.h"
#include "DirectoriesExceptions.h"

class Directory {
public:
    Directory(const std::string &diskPath, const std::string &dirPath, const std::string &user,
              DirectoriesMonitor &directoriesMonitor);

    ~Directory();

    Directory(const Directory &) = delete;

    Directory &operator=(const Directory &) = delete;

    char *read();

    [[nodiscard]] inline int getFD() const { return descriptor; }

private:
    int descriptor;
    DIR *dir;
    std::string user;
    std::string path;
    DirectoriesMonitor &directoriesMonitor;
};

#endif //TIN_DIRECTORY_H
