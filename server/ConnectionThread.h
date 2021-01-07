//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_CONNECTIONTHREAD_H
#define TIN_CONNECTIONTHREAD_H

#include <sys/socket.h>

#include <set>
#include <atomic>

#include "File.h"
#include "Directory.h"
#include "ConnectionHandler.h"
#include "FileManager.h"

class ConnectionThread
{
public:
    ConnectionThread(int socketFd, std::atomic<bool>& running) : running(running), connectionHandler(socketFd) { }

    void run();

    void closeConnection();

    void openFile();
    void readFile();
    void writeFile();
    void fileStat();
    void fileSeek();
    void closeFile();
    void unlinkFile();

    void openDirectory();
    void readDirectory();
    void closeDirectory();

    void closeDescriptors();

private:
    struct FileComp { bool operator () (const File& a, const File& b) { return a.getFD() < b.getFD(); } };
    struct DirectoryComp { bool operator () (const Directory& a, const Directory& b) { return a.getFD() < b.getFD(); } };

    using FileTable = std::set<File*, FileComp>;
    using DirectoryTable = std::set<Directory*, DirectoryComp>;

    FileTable files;
    std::unordered_map<int, File*> filesFromFd;
    std::unordered_map<int, Directory*> directoriesFromD;
    DirectoryTable directories;

    ConnectionHandler connectionHandler;

    std::atomic<bool>& running;
    bool closed = false;

    std::string user;
};

#endif //TIN_CONNECTIONTHREAD_H
