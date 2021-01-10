cmake_minimum_required(VERSION 3.17)
project(tin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(test test/test.cpp)
add_executable(testClient test/clientMainTest.cpp server/ConnectionHandler.cpp)
add_executable(testServer test/serverMainTest.cpp)
add_executable(server serverMain.cpp server/Server.h server/fs/FileManager.h server/fs/FilesMonitor.h server/fs/File.h server/fs/Directory.h server/fs/File.cpp server/fs/Directory.cpp server/ConnectionThread.cpp server/ConnectionThread.h server/ConnectionHandler.h server/ServerExceptions.h server/ConnectionHandler.cpp server/fs/FileManager.cpp server/fs/DirectoriesMonitor.h server/Server.cpp server/authorization/Authorization.cpp utilities/Requests.h utilities/Protocol.h utilities/Errors.h)
add_executable(client clientMain.cpp server/Server.h server/fs/FileManager.h server/fs/FilesMonitor.h server/fs/File.h server/fs/Directory.h server/fs/File.cpp server/fs/Directory.cpp server/ConnectionThread.cpp server/ConnectionThread.h server/ConnectionHandler.h server/ServerExceptions.h server/ConnectionHandler.cpp server/fs/FileManager.cpp server/fs/DirectoriesMonitor.h server/Server.cpp server/authorization/Authorization.cpp utilities/Requests.h utilities/Protocol.h utilities/Errors.h client/Client.h client/Client.cpp)