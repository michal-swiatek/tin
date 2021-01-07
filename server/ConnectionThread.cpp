//
// Created by micho6 on 03.01.2021.
//

#include "ConnectionThread.h"

#include "ServerExceptions.h"

void ConnectionThread::run()
{
    //  TODO: close parent socket

    while (running && !closed)
    {
        auto request = connectionHandler.getRequest();

        switch (request) {
            case ConnectionHandler::OPEN_FILE:      openFile();         break;
            case ConnectionHandler::READ_FILE:      readFile();         break;
            case ConnectionHandler::WRITE_FILE:     writeFile();        break;
            case ConnectionHandler::FILE_STAT:      fileStat();         break;
            case ConnectionHandler::CLOSE_FILE:     closeFile();        break;
            case ConnectionHandler::UNLINK_FILE:    unlinkFile();       break;
            case ConnectionHandler::OPEN_DIR:       openDirectory();    break;
            case ConnectionHandler::READ_DIR:       readDirectory();    break;
            case ConnectionHandler::CLOSE_DIR:      closeDirectory();   break;
            case ConnectionHandler::DISCONNECT:     closeConnection();  break;
            case ConnectionHandler::CONNECT:
            case ConnectionHandler::REPEAT:                             break;
        }
    }
}

void ConnectionThread::closeConnection()
{
    closed = true;
}

void ConnectionThread::openFile()
{
    throw NotImplementedError();
}

void ConnectionThread::readFile()
{
    throw NotImplementedError();
}

void ConnectionThread::writeFile()
{
    throw NotImplementedError();
}

void ConnectionThread::fileStat()
{
    throw NotImplementedError();
}

void ConnectionThread::closeFile()
{
    throw NotImplementedError();
}

void ConnectionThread::unlinkFile()
{
    throw NotImplementedError();
}

void ConnectionThread::openDirectory()
{
    throw NotImplementedError();
}

void ConnectionThread::readDirectory()
{
    throw NotImplementedError();
}

void ConnectionThread::closeDirectory()
{
    throw NotImplementedError();
}

void ConnectionThread::closeDescriptors()
{
    throw NotImplementedError();
}
