//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_FILEMANAGER_H
#define TIN_FILEMANAGER_H

#include <dirent.h>

#include <unordered_map>

#include "File"
#include "Directory.h"
#include "FilesMonitor.h"

class FileManager
{
public:
    static FileManager& getInstance()
    {
        static FileManager manager;
        return manager;
    }

    void init();

    File openFile(const char* path, int oflag, const std::string& user);
    Directory openDirectory(const char* path, const std::string& user);

    void closeFile(int fd);
    void closeDirectory(int fd);

    void unlinkFile(const std::string& path, const std::string& user) const;

    //  Copying nor moving is not allowed in singleton class
    FileManager(const FileManager& other) = delete;
    FileManager(FileManager&& other) = delete;
    FileManager& operator = (const FileManager& other) = delete;
    FileManager& operator = (FileManager&& other) = delete;

private:
    FileManager() = default;

    using OwnerTable = std::unordered_map<std::string, std::string>;
    using DirectoryMap = std::unordered_map<int, DIR*>;

    FilesMonitor openedFiles;
    DirectoryMap openedDirectories;

    OwnerTable fileOwners;
};

#endif //TIN_FILEMANAGER_H
