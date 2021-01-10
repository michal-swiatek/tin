//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_CONNECTIONTHREAD_H
#define TIN_CONNECTIONTHREAD_H

#include <sys/socket.h>

#include <set>
#include <atomic>
#include <thread>

#include "fs/File.h"
#include "fs/Directory.h"
#include "fs/FileManager.h"
#include "ConnectionHandler.h"
#include <memory>

class ConnectionThread
{
public:
    ConnectionThread(std::unique_ptr<ConnectionHandler>&& connectionHandler, std::atomic<bool>& running);

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

    [[nodiscard]] bool isClosed() const { return closed; }
    [[nodiscard]] std::thread& getThread() { return thread; }

private:
    struct FileComp { bool operator () (const File* a, const File* b) const { return a->getFD() < b->getFD(); } };
    struct DirectoryComp { bool operator () (const Directory* a, const Directory* b) const { return a->getFD() < b->getFD(); } };

    using FileTable = std::set<File*, FileComp>;
    using DirectoryTable = std::set<Directory*, DirectoryComp>;

    FileTable files;
    std::unordered_map<int, File*> filesFromFd;
    std::unordered_map<int, Directory*> directoriesFromD;
    DirectoryTable directories;

    std::thread thread;
    ConnectionHandler connectionHandler;

    std::atomic<bool>& running;
    bool closed = false;

    std::string user;
};

#endif //TIN_CONNECTIONTHREAD_H
