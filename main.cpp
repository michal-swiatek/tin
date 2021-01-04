#include <iostream>
#include "server/FileManager.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

int main() {
    FileManager::getInstance().init();

//    std::string dirPath = "/";
//    std::string user = "asia";
//    Directory directory = FileManager::getInstance().getDirectory(dirPath, user);
//
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//    std::cout<<directory.read()<<'\n';
//
//    std::string filePath = "/1.txt";
//
//    File file = FileManager::getInstance().getFile(filePath, File::Flags::READ_ONLY, user);
//    File file2 = FileManager::getInstance().getFile(filePath, File::Flags::READ_ONLY, user);
//
////    const char* buff = std::string("pie").c_str();
//    char buff[3];
//
////    file.write(buff, 3);
//    file.read(buff, 3);
//
//    std::cout<<buff<<'\n';

    FileManager::getInstance().end();
    return 0;
}
