#include "Directory.h"

#include <utility>
#include <iostream>

#include "ServerExceptions.h"

Directory::Directory(const std::string &diskPath, const std::string &dirPath, const std::string &user,
                     DirectoriesMonitor &directoriesMonitor)
        : directoriesMonitor(directoriesMonitor), path(dirPath), user(user) {
    // TODO: obsluga bledow
    // nie ma directory
    directoriesMonitor.add(path, user);
    std::string fullPath = diskPath + dirPath;
    dir = opendir(fullPath.c_str());
    descriptor = dirfd(dir);
}

Directory::~Directory() {
    // TODO: obsluga bledow
    // jak nie ma tego co chcemy zamknac
    closedir(dir);
    directoriesMonitor.remove(path, user);
}

char *Directory::read() {
    dirent *d;
    while ((d = ::readdir(this->dir)) != nullptr) {
        if (d->d_name != std::string(".") && d->d_name != std::string("..")) {
            return d->d_name;
        }
    }
    // TODO: brak kolejnych pozycji
    throw std::exception();
    // return nullptr;
}