#include <iostream>
#include <cstring>
#include <utility>
#include "FileManager.h"

void FileManager::init(const std::string& diskPathParam, const std::string& diskNameParam,
                       const std::string& filesOwnersFileNameParam, std::shared_ptr<Authorization> ptr) {
    // Prepare paths
    this->filesOwnersFileName = filesOwnersFileNameParam;
    this->diskPath = diskPathParam + diskNameParam;
    this->diskName = diskNameParam;
    filesOwnersFilePath = diskPathParam + filesOwnersFileNameParam;
    authorization = std::move(ptr);
    // Check if paths are valid
    DIR *dir = opendir(diskPathParam.c_str());
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        mkdir(this->diskPath.c_str(), 0700);
    } else {
        throw DiskNotCreated();
    }
    std::string filesOwnersFileDir = filesOwnersFilePath.substr(0, filesOwnersFilePath.find(filesOwnersFileNameParam));
    dir = opendir(filesOwnersFileDir.c_str());
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        throw FilesOwnersDirectoryNotExist();
    } else {
        throw FilesOwnersDirectoryNotOpened();
    }
    // Prepare file owners
    std::ifstream infile(filesOwnersFilePath);
    std::string owner;
    std::string filePath;
    while (std::getline(infile, owner, ':')) {
        if (!std::getline(infile, filePath)) {
            break;
        }
        filesOwners.insert({filePath, owner});
    }

    // Check if all files were added and if not add them with empty ownership
    listFilesRecursively(this->diskPath);
}

void FileManager::listFilesRecursively(const std::string &basePath) {
    std::string tempPath;
    struct dirent *dp;
    DIR *dir = opendir(basePath.c_str());

    if (!dir)
        return;

    while ((dp = readdir(dir)) != nullptr) {
        if (dp->d_name != std::string(".") && dp->d_name != std::string("..")) {
            tempPath = basePath + "/" + dp->d_name;
            if (dp->d_type == DT_DIR) {
                listFilesRecursively(tempPath);
            } else if (dp->d_type == DT_REG) {
                std::string filePath = tempPath.substr(tempPath.find(diskPath) + diskPath.length());
                try {
                    filesOwners.at(filePath);
                } catch (std::out_of_range &e) {
                    filesOwners.insert({filePath, ""});
                }
            }
        }
    }
    closedir(dir);
}

void FileManager::end() {
    // Save file owners
    std::ofstream ofile(filesOwnersFilePath);

    for (auto &filesOwner : filesOwners) {
        ofile << filesOwner.second << ':' << filesOwner.first << '\n';
    }
}


File* FileManager::getFile(const std::string &path, int flags, const std::string &user) {
    if (flags == O_CREAT){
        try{
            if(filesOwners.at(path) != user && authorization->userRole(user) != "a"){
                throw FileNotPermitted();
            }
        }catch (std::out_of_range& e){
            auto file = new File(diskPath, path, flags, user, openedFiles);
            filesOwners.insert({path, user});
            return file;
        }
    }
    try{
        if( filesOwners.at(path) != user && authorization->userRole(user) != "a"){
            throw FileNotPermitted();
        }
        return new File(diskPath, path, flags, user, openedFiles);
    }catch (std::out_of_range& e){
        throw FileNotExist();
    }
}

void FileManager::unlinkFile(const std::string &path, const std::string &user) {
    try {
        if (filesOwners.at(path) == user) {
            std::string fullPath = diskPath + path;
            if ( unlink(fullPath.c_str()) == 0 ) {
                filesOwners.erase(path);
            }else{
                throw FileNotUnlinked();
            }
        }else{
            throw FileNotPermitted();
        }
    }
    catch (std::out_of_range &e) {
        throw FileNotExist();
    }
}

Directory* FileManager::getDirectory(const std::string &path, const std::string &user) {
    return new Directory(diskPath, path, user, openedDirectories);
}