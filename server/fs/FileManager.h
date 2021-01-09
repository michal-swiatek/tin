#ifndef TIN_FILEMANAGER_H
#define TIN_FILEMANAGER_H

#include <dirent.h>

#include <fstream>
#include <unordered_map>

#include "File.h"
#include "Directory.h"
#include "FilesMonitor.h"
#include "FilesExceptions.h"
#include "../ServerExceptions.h"

class FileManager
{
public:
    static FileManager& getInstance()
    {
        static FileManager manager;
        return manager;
    }

    void init(const std::string& diskPathParam, const std::string& diskNameParam, const std::string& filesOwnersFileNameParam);
    void end();

    File* getFile(const std::string &path, int flags, const std::string& user);

    void unlinkFile(const std::string& path, const std::string& user);

    Directory* getDirectory(const std::string &path, const std::string& user);

    //  Copying nor moving is not allowed in singleton class
    FileManager(const FileManager& other) = delete;
    FileManager(FileManager&& other) = delete;
    FileManager& operator = (const FileManager& other) = delete;
    FileManager& operator = (FileManager&& other) = delete;

private:
    FileManager() = default;

    void listFilesRecursively(const std::string& basePath);

    using OwnerTable = std::unordered_map<std::string, std::string>;

    FilesMonitor openedFiles;
    DirectoriesMonitor openedDirectories;

    OwnerTable filesOwners;

    std::string diskPath;
    std::string diskName;
    std::string filesOwnersFilePath;
    std::string filesOwnersFileName;
};

#endif //TIN_FILEMANAGER_H
