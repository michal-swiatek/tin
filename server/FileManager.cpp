#include <iostream>
#include "FileManager.h"

void FileManager::init() {
    // Prepare paths
    // TODO: tutaj zmienna zamiast stalej
    diskPath = "../disk";
    filesOwnersFilePath = "../filesOwners.txt";
    // Prepare file owners
    std::ifstream infile(filesOwnersFilePath);
    std::string owner;
    std::string filePath;
    while (std::getline(infile, owner, ' '))
    {
        if(!std::getline(infile, filePath)){
            break;
        }
        filesOwners.insert({filePath, owner});
    }

    // Check if all files were added and if not add them as admin's files
    listFilesRecursively(diskPath);
}

void FileManager::listFilesRecursively(const std::string& basePath)
{
    std::string tempPath;
    struct dirent *dp;
    DIR *dir = opendir(basePath.c_str());

    if (!dir)
        return;

    while ((dp = readdir(dir)) != nullptr){
        if (dp->d_name != std::string(".") && dp->d_name != std::string("..")){
            tempPath = basePath + "/" + dp->d_name;
            if(dp->d_type == DT_DIR){
                listFilesRecursively(tempPath);
            }else if(dp->d_type == DT_REG){
                std::string filePath = tempPath.substr(tempPath.find(diskPath)+diskPath.length());
                try{
                    filesOwners.at(filePath);
                }catch (std::out_of_range& e){
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

    std::unordered_map<std::string, std::string>::iterator it;

    for (it = filesOwners.begin(); it != filesOwners.end(); ++it){
        ofile<<it->second<<' '<<it->first<<'\n';
    }
}


File FileManager::getFile(const std::string &path, File::Flags flags, const std::string &user) {
    // TODO: co ze sprawdzaniem czy mozna otworzyc
    // TODO: co jak juz jest
    // TODO: co jak O_CREATE
    return File(diskPath, path, flags, user, openedFiles);
}

void FileManager::unlinkFile(const std::string &path, const std::string &user) {
    try{
        if(filesOwners.at(path) == user){
            std::string fullPath = diskPath + path;
            // TODO: co jak nie uda sie usunac
            if(unlink(fullPath.c_str())>0){
                filesOwners.erase(path);
            }
        }
        // TODO: co jak nie wlasciciel probuje usunac
    }
    catch(std::out_of_range& e){
        // TODO: co jak nie ma takiego pliku
    }
}

Directory FileManager::getDirectory(const std::string &path, const std::string& user) {
    // TODO: co jak juz jest
    return Directory(diskPath, path, user, openedDirectories);
}