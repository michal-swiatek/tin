cmake_minimum_required(VERSION 3.17)
project(tin)

set(CMAKE_CXX_STANDARD 20)

add_executable(tin main.cpp server/Server.h server/FileManager.h server/FilesMonitor.h server/File.h server/Directory.h server/File.cpp server/Directory.cpp server/ConnectionThread.cpp server/ConnectionThread.h server/ConnectionHandler.h server/ServerExceptions.h server/ConnectionHandler.cpp client/response.h client/response.cpp structures/structures.h client/functions.h client/functions.cpp client/host.h)