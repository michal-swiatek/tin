//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_CONNECTIONTHREAD_H
#define TIN_CONNECTIONTHREAD_H

#include <set>

#include "File.h"
#include "Directory.h"
#include "ConnectionHandler.h"

class ConnectionThread
{
    void run();

    void closeConnection();

    void openFile();
    void readFile();
    void writeFile();
    void fileStat();
    void closeFile();
    void unlinkFile();

    void openDirectory();
    void readDirectory();
    void closeDirectory();

    void closeDescriptors();

private:
    struct FileComp { bool operator () (const File& a, const File& b) { return a.getFD() < b.getFD(); } };
    struct DirectoryComp { bool operator () (const Directory& a, const Directory& b) { return a.getFD() < b.getFD(); } };

    using FileTable = std::set<File, FileComp>;
    using DirectoryTable = std::set<Directory, DirectoryComp>;

    FileTable files;
    DirectoryTable directories;

    ConnectionHandler connectionHandler;

    bool running = true;
};

#endif //TIN_CONNECTIONTHREAD_H
