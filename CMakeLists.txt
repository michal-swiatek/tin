cmake_minimum_required(VERSION 3.17)
project(tin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(server main.cpp server/Server.h server/FileManager.h server/FilesMonitor.h server/File.h server/Directory.h server/File.cpp server/Directory.cpp server/ConnectionThread.cpp server/ConnectionThread.h server/ConnectionHandler.h server/ServerExceptions.h server/ConnectionHandler.cpp server/FileManager.cpp server/Server.cpp)