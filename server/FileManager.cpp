#include <iostream>
#include "FileManager.h"

void FileManager::init(const std::string& diskPath, const std::string& diskName, const std::string& filesOwnersFileName) {
    // Prepare paths
    // TODO: tutaj zmienna zamiast stalej
    this->filesOwnersFileName = filesOwnersFileName;
    this->diskPath = diskPath;
    this->diskName = diskName;
    filesOwnersFilePath = diskPath + filesOwnersFileName;
    // Check if paths are valid
    DIR *dir = opendir(diskPath.c_str());
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        mkdir(this->diskPath.c_str(), 0700);
    } else {
        // TODO: obsluga bledow opendir
        throw std::exception();
    }
    std::string filesOwnersFileDir = filesOwnersFilePath.substr(0, filesOwnersFilePath.find(filesOwnersFileName));
    dir = opendir(filesOwnersFileDir.c_str());
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        // TODO: folder w ktorym ma byc plik nie istnieje
        throw std::exception();
    } else {
        // TODO: obsluga bledow opendir
        throw std::exception();
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
    listFilesRecursively(this->diskPath+this->diskName);
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
                std::string filePath = tempPath.substr(tempPath.find(diskPath+diskName) + diskPath.length()+diskName.length());
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


File* FileManager::getFile(const std::string &path, File::Flags flags, const std::string &user) {
    // TODO: co ze sprawdzaniem czy mozna otworzyc
    // TODO: co jak O_CREATE
    try{
        if( filesOwners.at(path) != user){
            // TODO: plik nie nalezy do usera
            throw std::exception();
        }
        if (flags == File::Flags::CREATE) {
            // TODO: co jak create
            auto file = new File(diskPath, path, flags, user, openedFiles);
            filesOwners.insert({path, user});
            return file;
        }
    }catch (std::out_of_range& e){
        // TODO: nie ma takiego pliku
    }catch (std::exception& e){
        // TODO: nie udalo sie stworzyc pliku
    }
    return new File(diskPath, path, flags, user, openedFiles);
}

void FileManager::unlinkFile(const std::string &path, const std::string &user) {
    try {
        if (filesOwners.at(path) == user) {
            std::string fullPath = diskPath + path;
            if (unlink(fullPath.c_str()) == 0) {
                filesOwners.erase(path);
            }else{
                // TODO: co jak nie uda sie usunac
                throw std::exception();
            }
        }
        // TODO: co jak nie wlasciciel probuje usunac
        throw std::exception();
    }
    catch (std::out_of_range &e) {
        // TODO: co jak nie ma takiego pliku
        throw std::exception();
    }
}

Directory* FileManager::getDirectory(const std::string &path, const std::string &user) {
    return new Directory(diskPath, path, user, openedDirectories);
}