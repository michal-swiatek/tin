//
// Created by micho6 on 03.01.2021.
//

#include <cstring>
#include "ConnectionThread.h"

#include "ServerExceptions.h"

ConnectionThread::ConnectionThread(ConnectionHandler&& connectionHandler, std::atomic<bool> &running) : running(running),
                                                                               connectionHandler(connectionHandler),
                                                                               thread(&ConnectionThread::run, this) { }

void ConnectionThread::run()
{
    while (running && !closed)
    {
        auto request = connectionHandler.getRequest();

        switch (request) {
            case ConnectionHandler::C_OPEN_FILE:      openFile();         break;
            case ConnectionHandler::C_READ_FILE:      readFile();         break;
            case ConnectionHandler::C_WRITE_FILE:     writeFile();        break;
            case ConnectionHandler::C_FILE_STAT:      fileStat();         break;
            case ConnectionHandler::C_FILE_LSEEK:     fileSeek();         break;
            case ConnectionHandler::C_CLOSE_FILE:     closeFile();        break;
            case ConnectionHandler::C_UNLINK_FILE:    unlinkFile();       break;
            case ConnectionHandler::C_OPEN_DIR:       openDirectory();    break;
            case ConnectionHandler::C_READ_DIR:       readDirectory();    break;
            case ConnectionHandler::C_CLOSE_DIR:      closeDirectory();   break;
            case ConnectionHandler::C_DISCONNECT:     closeConnection();  break;
            case ConnectionHandler::C_CONNECT:
            case ConnectionHandler::REPEAT:                             break;
        }
    }

    closed = true;
}

void ConnectionThread::closeConnection()
{
    closed = true;
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    // Check data and react
    closeDescriptors();
    header.param1 = 0;
    header.param2 = 0;
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_DISCONNECT, header.param1, header.param2);
}

void ConnectionThread::openFile()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    char* data = connectionHandler.getData();
    int dataSize = connectionHandler.dataSize();
    // Check data and react
    if(header.param2 != O_CREAT && header.param2 != O_RDWR && header.param2 != O_RDONLY && header.param2 != O_WRONLY){
        File* file;
        try{
            file = FileManager::getInstance().getFile(std::string(data, dataSize), header.param2, this->user);
            header.param1 = 0;
            header.param2 = file->getFD();
            files.insert(file);
            filesFromFd.insert({file->getFD(), file});
        }catch (FileNotExist& exception){
            // TODO: zamienic na kod bledu
            header.param1 = 2;
            header.param2 = 0;
        }catch(FileNotPermitted& exception){
            // TODO: co jak nie mam uprawnien do otwarcia - kod bledu itp
            header.param1 = 1;
            header.param2 = 0;
        }
    }else{
        // TODO: zmienic na kod bledu
        header.param1 = 5;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_OPEN_FILE, header.param1, header.param2);
}

void ConnectionThread::readFile()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    char* data = nullptr;
    int dataSize = 0;
    // Check data and react
    File* file;
    try{
        file = filesFromFd.at(header.param2);
        data = new char[header.param1];
        dataSize = header.param1;
        header.param1 = 0;
        int res = file->read(data, dataSize);
        if(res < 0){
            // TODO: kod bledu
            header.param1 = 1;
            header.param2 = 0;
            dataSize = 0;
            delete(data);
            data = nullptr;
        }else{
            // TODO: kod bledu
            header.param1 = 0;
            header.param2 = res;
        }

    }catch (std::out_of_range& e){
        // TODO: kod bledu
        header.param1 = 3;
        header.param2 = 0;
    }catch (FileWriteOnly& e){
        // TODO: kod bledu
        header.param1 = 1;
        header.param2 = 0;
    }

    // Send response
    connectionHandler.setData(data, dataSize);
    connectionHandler.sendReply(ConnectionHandler::S_READ_FILE, header.param1, header.param2);
}

void ConnectionThread::writeFile()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    char* data = connectionHandler.getData();
    int dataSize = connectionHandler.dataSize();
    // Check data and react
    File* file;
    try{
        file = filesFromFd.at(header.param2);
        int res = file->write(data, dataSize);
        if(res < 0){
            // TODO: kod bledu
            header.param1 = 1;
            header.param2 = 0;
        }else{
            // TODO: kod bledu
            header.param1 = 0;
            header.param2 = res;
        }

    }catch (std::out_of_range& e){
        // TODO: kod bledu
        header.param1 = 3;
        header.param2 = 0;
    }catch (FileReadOnly& e){
        // TODO: kod bledu
        header.param1 = 1;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_WRITE_FILE, header.param1, header.param2);
}

void ConnectionThread::fileStat()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    char* data = nullptr;
    int dataSize = 0;
    // Check data and react
    File* file;
    FileStat* fileStat = nullptr;
    try{
        file = filesFromFd.at(header.param2);
        fileStat = new FileStat();
        int res = file->fstat(fileStat);
        if(res < 0){
            // TODO: kod bledu
            header.param1 = 1;
            header.param2 = 0;
        }else{
            // TODO: kod bledu
            header.param1 = 0;
            header.param2 = sizeof(FileStat);
            dataSize = header.param2;
            data = (char*) fileStat;
        }
    }catch (std::out_of_range& e){
        // TODO: kod bledu
        header.param1 = 3;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(data, dataSize);
    connectionHandler.sendReply(ConnectionHandler::S_FILE_STAT, header.param1, header.param2);
}

void ConnectionThread::fileSeek()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    int* data = (int*)connectionHandler.getData();
    // Check data and react
    File* file;
    try{
        file = filesFromFd.at(header.param2);
        int offset = header.param1;
        int whence = *data;
        int res = file->lseek(offset, whence);
        if(res < 0){
            // TODO: kod bledu
            header.param1 = 1;
            header.param2 = 0;
        }else{
            // TODO: kod bledu
            header.param1 = 0;
            header.param2 = res;
        }
    }catch (std::out_of_range& e){
        // TODO: kod bledu
        header.param1 = 3;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_FILE_LSEEK, header.param1, header.param2);
}

void ConnectionThread::closeFile()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    // Check data and react
    File* file;
    try{
        file = filesFromFd.at(header.param1);
        header.param1 = 0;
        header.param2 = 0;
        files.erase(file);
        filesFromFd.erase(file->getFD());
        delete(file);
    }catch (std::out_of_range& exception){
        // TODO: zamienic na kod bledu
        header.param1 = 3;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_CLOSE_FILE, header.param1, header.param2);
}

void ConnectionThread::unlinkFile()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    char* data = connectionHandler.getData();
    int dataSize = connectionHandler.dataSize();
    // Check data and react
    try{
        FileManager::getInstance().unlinkFile(std::string(data, dataSize), this->user);
        header.param1 = 0;
        header.param2 = 0;
    }catch (FileNotExist& exception) {
        // TODO: zamienic na kod bledu
        header.param1 = 2;
        header.param2 = 0;
    }
    catch (FileNotPermitted& exception) {
        // TODO: zamienic na kod bledu
        header.param1 = 1;
        header.param2 = 0;
    }
    catch (FileNotUnlinked& exception) {
        // TODO: zamienic na kod bledu
        header.param1 = 1;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_UNLINK_FILE, header.param1, header.param2);
}

void ConnectionThread::openDirectory()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    char* data = connectionHandler.getData();
    int dataSize = connectionHandler.dataSize();
    // Check data and react
    Directory* directory;
    try{
        directory = FileManager::getInstance().getDirectory(std::string(data, dataSize), this->user);
        header.param1 = 0;
        header.param2 = directory->getFD();
        directories.insert(directory);
        directoriesFromD.insert({directory->getFD(), directory});
    }catch (DirectoryNotOpened& exception){
        // TODO: zamienic na kod bledu
        header.param1 = 8;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_OPEN_DIR, header.param1, header.param2);
}

void ConnectionThread::readDirectory()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    char* data = nullptr;
    int dataSize = 0;
    // Check data and react
    Directory* directory;
    try{
        directory = directoriesFromD.at(header.param1);
        data = directory->read();
        header.param1 = 0;
        header.param2 = strlen(data);
        dataSize = header.param2;
    }catch (std::out_of_range& exception){
        // TODO: zamienic na kod bledu
        header.param1 = 3;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(data, dataSize);
    connectionHandler.sendReply(ConnectionHandler::S_READ_DIR, header.param1, header.param2);
}

void ConnectionThread::closeDirectory()
{
    // Read data
    ConnectionHandler::Header header = connectionHandler.getHeader();
    // Check data and react
    Directory* directory;
    try{
        directory = directoriesFromD.at(header.param1);
        header.param1 = 0;
        header.param2 = 0;
        directories.erase(directory);
        directoriesFromD.erase(directory->getFD());
        delete(directory);
    }catch (std::out_of_range& exception){
        // TODO: zamienic na kod bledu
        header.param1 = 3;
        header.param2 = 0;
    }
    // Send response
    connectionHandler.setData(nullptr, 0);
    connectionHandler.sendReply(ConnectionHandler::S_CLOSE_DIR, header.param1, header.param2);
}

void ConnectionThread::closeDescriptors()
{
    for (auto& it: filesFromFd) {
        delete(it.second);
    }
    for (auto& it: directoriesFromD) {
        delete(it.second);
    }
}
