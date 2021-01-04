#include <iostream>
#include "server/FileManager.h"

int main() {
    FileManager::getInstance().init();
//    std::cout<<'\n'<<__FILE__;

    std::string filePath = "/first";
    Directory directory = FileManager::getInstance().openDirectory(filePath.c_str());

    std::cout<<directory.read()<<'\n';
    std::cout<<directory.read()<<'\n';

    FileManager::getInstance().closeDirectory(directory.getFD());

    FileManager::getInstance().end();
    return 0;
}
