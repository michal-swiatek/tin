#include <iostream>
#include "FileManager.h"

void FileManager::init() {
    // Prepare disk path
    // TODO: tutaj zmienna zamiast stalej
    diskPath = "../disk";
    // Prepare file owners
    // TODO: gdzie ma byc ten plik?
    std::ifstream infile("../fileOwners.txt");
    std::string owner;
    std::string filePath;
    while (std::getline(infile, owner, ' '))
    {
        if(!std::getline(infile, filePath)){
            break;
        }
        fileOwners.insert({filePath, owner});
    }

    // Check if all files were added and if not add them as admin's files
    // TODO: zaimplementowac to
}

void FileManager::end() {
    // Save file owners
    // TODO: co z plikami admina?
    std::ofstream ofile("../fileOwners.txt");

    std::unordered_map<std::string, std::string>::iterator it;

    for (it = fileOwners.begin(); it != fileOwners.end(); ++it)
    {
        ofile<<it->second<<' '<<it->first<<'\n';
    }
}


File FileManager::openFile(const char *path, int oflag, const std::string &user) {

    return File(0, "");
}

Directory FileManager::openDirectory(const char *path) {
    // TODO: obsluga bledow
    // nie ma directory
    std::string filePath = diskPath + std::string(path);
    DIR* dir = opendir(filePath.c_str());
    int desc = dirfd(dir);
    openedDirectories.insert({desc, dir});
    return Directory(desc, dir);
}

void FileManager::closeDirectory(int fd) {
    // TODO: obsluga bledow
    // jak nie ma tego co chcemy zamknac
    closedir(openedDirectories.at(fd));
    openedDirectories.erase(fd);
}

void FileManager::closeFile(int fd) {

}

void FileManager::unlinkFile(const std::string &path, const std::string &user) const {

}