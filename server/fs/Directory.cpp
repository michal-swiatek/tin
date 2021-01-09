#include "Directory.h"

#include <utility>
#include <iostream>

#include "../ServerExceptions.h"

Directory::Directory(const std::string &diskPath, const std::string &dirPath, const std::string &user,
                     DirectoriesMonitor &directoriesMonitor)
        : directoriesMonitor(directoriesMonitor), path(dirPath), user(user) {
    std::string fullPath = diskPath + dirPath;
    dir = opendir(fullPath.c_str());
    if(dir == nullptr){
        throw DirectoryNotOpened();
    }
    descriptor = dirfd(dir);
    directoriesMonitor.add(path, user);
}

Directory::~Directory() {
    closedir(dir);
    directoriesMonitor.remove(path, user);
}

char* Directory::read() {
    dirent *d;
    while ((d = ::readdir(this->dir)) != nullptr) {
        if (d->d_name != std::string(".") && d->d_name != std::string("..")) {
            return d->d_name;
        }
    }
    throw DirectoryEndReached();
}