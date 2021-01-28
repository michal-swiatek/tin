//
// Created by micho6 on 24.01.2021.
//

#include "Session.h"

#include <unistd.h>
#include <sys/stat.h>

#include <memory>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

Session::Session(NFSClient *client) : client(client) {}

void Session::start()
{
    std::cout << std::endl;
    do
    {
        std::cout << ">> ";

        std::string line;
        std::getline(std::cin, line);

        auto tokens = parseLine(line);
        if (!tokens.empty())
            executeCommand(tokens);

    } while (opened);
}

std::vector<std::string> Session::parseLine(const std::string& line)
{
    std::vector<std::string> tokens;
    std::string token;
    auto sstr = std::stringstream(line);

    while (sstr >> token)
    {
        //  Replace special characters
        try {
            if (token[0] == '$')
                token = std::to_string(*std::next(fileDescriptors.begin(), std::stoi(token.substr(1)) - 1));
            else if (token[0] == '#')
                token = std::to_string(*std::next(directoryDescriptors.begin(), std::stoi(token.substr(1)) - 1));
        }
        catch (std::invalid_argument& e) {
            std::cout << "Invalid substitution argument: " << e.what() << '\n';
        }

        tokens.emplace_back(token);
    }

    return tokens;
}

void Session::executeCommand(const std::vector<std::string> &tokens)
{
    auto command = tokens.front();
    auto ret = 0;

    auto fileFlag = [](const std::string& str) {
        if (str == "O_RDONLY")      return O_RDONLY;
        else if (str == "O_WRONLY") return O_WRONLY;
        else if (str == "O_RDWR")   return O_RDWR;
        else if (str == "O_CREAT")  return O_CREAT;
        else                        return -1;
    };

    auto getWhence = [](const std::string& str) {
        if (str == "SEEK_SET")      return SEEK_SET;
        else if (str == "SEEK_CUR") return SEEK_CUR;
        else if (str == "SEEK_END") return SEEK_END;
        else                        return -1;
    };

    if (command == "q" || command == "quit")
        opened = false;
    else if (command == "h" || command == "help")
        printHelp();
    else if (command == "open") {
        if (tokens.size() != 3)     std::cout << "Invalid command\n";
        else {
            auto flag = fileFlag(tokens[2]);
            if (flag == -1)
                std::cout << "Invalid flag\n";
            else {
                ret = client->mynfs_open(const_cast<char *>(tokens[1].c_str()), static_cast<NFSClient::FileFlag>(flag));

                if (ret != -1) {
                    fileDescriptors.insert(ret);
                    std::cout << "Recieved file descriptor: " << ret << '\n';
                }
            }
        }
    }
    else if (command == "close") {
        if (tokens.size() != 2)     std::cout << "Invalid command\n";
        else {
            int fd;

            try {
                fd = std::stoi(tokens[1]);
                ret = client->mynfs_close(fd);

                if (ret != -1) {
                    fileDescriptors.erase(fd);
                    std::cout << "Closed file descriptor: " << fd << '\n';
                }
            }
            catch (std::invalid_argument& e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
        }
    }
    else if (command == "read") {
        if (tokens.size() != 3)     std::cout << "Invalid command\n";
        else {
            int fd, bufferSize;

            try {
                fd = std::stoi(tokens[1]);
                bufferSize = std::stoi(tokens[2]);

                auto buffer = std::unique_ptr<char[]>(new char[bufferSize + 1]);
                buffer[bufferSize] = '\0';

                ret = client->mynfs_read(fd, buffer.get(), bufferSize);

                std::cout << "Received data: " << (ret > 0 ? buffer.get() : "") << '\n';
            }
            catch (std::invalid_argument& e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
        }
    }
    else if (command == "write") {
        if (tokens.size() != 3)     std::cout << "Invalid command\n";
        else {
            int fd, bufferSize;

            try {
                fd = std::stoi(tokens[1]);
                bufferSize = tokens[2].size();

                auto buffer = std::unique_ptr<char[]>(new char[bufferSize]);
                memcpy(buffer.get(), tokens[2].c_str(), bufferSize);

                ret = client->mynfs_write(fd, buffer.get(), bufferSize);
            }
            catch (std::invalid_argument& e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
        }
    }
    else if (command == "lseek") {
        if (tokens.size() != 4)     std::cout << "Invalid command\n";
        else {
            int fd, offset, whence = getWhence(tokens[3]);

            if (whence == -1)
                std::cout << "Invalid flag\n";
            else {
                try {
                    fd = std::stoi(tokens[1]);
                    offset = tokens[2].size();

                    ret = client->mynfs_lseek(fd, offset, static_cast<NFSClient::Whence>(whence));
                }
                catch (std::invalid_argument &e) {
                    std::cout << "Invalid argument: " << e.what() << '\n';
                }
            }
        }
    }
    else if (command == "unlink") {
        if (tokens.size() != 2)     std::cout << "Invalid command\n";
        else
            ret = client->mynfs_unlink(const_cast<char *>(tokens[1].c_str()));
    }
    else if (command == "fstat") {
        if (tokens.size() != 2)     std::cout << "Invalid command\n";
        else {
            int fd;
            FileStat stat{};

            try {
                fd = std::stoi(tokens[1]);

                ret = client->mynfs_fstat(fd, &stat);
            }
            catch (std::invalid_argument &e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }

            if (ret != -1)
            {
                std::cout << "File stat:\n";
                std::cout << "size - " << stat.size << '\n';
                std::cout << "last access - " << stat.lastAccess << '\n';
                std::cout << "last modification - " << stat.lastModification << '\n';
                std::cout << "last status change - " << stat.lastStatusChange << '\n';
                std::cout << "flag - " << stat.flag << '\n';
                std::cout << "file path - " << stat.filePath << '\n';
                std::cout << "owner - " << stat.owner << '\n';
            }
        }
    }
    else if (command == "opendir") {
        if (tokens.size() != 2)     std::cout << "Invalid command\n";
        else {
            ret = client->mynfs_opendir(const_cast<char *>(tokens[1].c_str()));

            if (ret != -1) {
                directoryDescriptors.insert(ret);
                std::cout << "Received directory descriptor: " << ret << '\n';
            }
        }
    }
    else if (command == "closedir") {
        if (tokens.size() != 2)     std::cout << "Invalid command\n";
        else {
            int fd;

            try {
                fd = std::stoi(tokens[1]);

                ret = client->mynfs_closedir(fd);

                if (ret != -1) {
                    directoryDescriptors.erase(fd);
                    std::cout << "Closed directory at descriptor: " << fd << '\n';
                }
            }
            catch (std::invalid_argument &e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
        }
    }
    else if (command == "readdir") {
        if (tokens.size() != 2)     std::cout << "Invalid command\n";
        else {
            int fd;

            try {
                fd = std::stoi(tokens[1]);

                auto file = client->mynfs_readdir(fd);

                if (file)
                    std::cout << "File: " << file << '\n';
                else
                    ret = -1;
            }
            catch (std::invalid_argument &e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
        }
    }
    else if (command == "download") {
        if (tokens.size() != 3)     std::cout << "Invalid command\n";
        else    ret = download(tokens[1], tokens[2]);
    }
    else if (command == "upload") {
        if (tokens.size() != 3)     std::cout << "Invalid command\n";
        else    ret = upload(tokens[1], tokens[2]);
    }
    else if (command == "list") {
        if (tokens.size() != 1)     std::cout << "Invalid command\n";
        else {
            auto print = [](const auto& elem) { std::cout << elem << ' '; };

            std::cout << "File descriptors: ";
            std::for_each(fileDescriptors.begin(), fileDescriptors.end(), print);
            std::cout << '\n';

            std::cout << "Directory descriptors: ";
            std::for_each(directoryDescriptors.begin(), directoryDescriptors.end(), print);
            std::cout << '\n';
        }
    }
    else if (command == "sleep") {
        if (tokens.size() != 2)     std::cout << "Invalid command\n";
        else {
            try {
                int seconds = std::stoi(tokens[1]);
                sleep(seconds);
            }
            catch (std::invalid_argument &e) {
                std::cout << "Invalid argument: " << e.what() << '\n';
            }
        }
    }
    else
        std::cout << "Invalid command\n";

    if (ret == -1)
        std::cout << mynfs_strerror(client->mynfs_error) << '\n';
}

int Session::download(const std::string &serverSrc, const std::string &localDest)
{
    int fd = client->mynfs_open(const_cast<char *>(serverSrc.c_str()), NFSClient::O_RO);

    if (fd != -1)
    {
        FileStat stat{};
        int ret = client->mynfs_fstat(fd, &stat);

        if (ret != -1)
        {
            std::ofstream file(localDest);

            if (!file.good())
            {
                std::cout << "Could not open local file\n";
                return 0;
            }

            auto buffer = std::unique_ptr<char[]>(new char[stat.size]);
            ret = client->mynfs_read(fd, buffer.get(), stat.size);

            if (ret != -1)
            {
                file.write(buffer.get(), stat.size);

                return client->mynfs_close(fd);
            }
        }
    }

    return -1;
}

int Session::upload(const std::string &localSrc, const std::string &serverDest)
{
    int fd = client->mynfs_open(const_cast<char *>(serverDest.c_str()), NFSClient::O_CR);

    if (fd != -1)
    {
        std::ifstream file(localSrc);

        if (!file.good())
        {
            std::cout << "Could not open local file\n";
            return 0;
        }

        struct stat st{};
        stat(localSrc.c_str(), &st);

        auto buffer = std::unique_ptr<char[]>(new char[st.st_size]);
        file.read(buffer.get(), st.st_size);

        if (client->mynfs_write(fd, buffer.get(), st.st_size) != -1)
            return client->mynfs_close(fd);
    }

    return -1;
}

void Session::printHelp()
{
    std::cout << std::endl;

    std::cout << "List of possible commands:\n";
    std::cout << "help (h)                      - print help\n\n";

    std::cout << "open filePath flag            - opens file at filePath and returns file descriptor\n";
    std::cout << "                                available flags: O_RDONLY, O_WRONLY, O_RDWR, O_CREAT\n";
    std::cout << "close fd                      - close file with associated file descriptor\n";
    std::cout << "unlink fd                     - remove file from disk\n\n";

    std::cout << "read fd size                  - read size bytes from file associated with file descriptor\n";
    std::cout << "write fd message              - write size bytes to file associated with file descriptor\n";
    std::cout << "lseek fd offset whence        - \n";
    std::cout << "fstat fd                      - \n\n";

    std::cout << "opendir path                  - open directory at given path and return directory descriptor\n";
    std::cout << "closedir path                 - close directory at given directory descriptor\n";
    std::cout << "readdir fd                    - read directory associated with given directory descriptor\n\n";

    std::cout << "download serverSrc localDest  - download file from serverSrc and store it in localSrc\n";
    std::cout << "upload localSrc serverDest    - upload file from localSrc to serverDest\n\n";

    std::cout << "sleep seconds                 - sleep specified number of seconds\n";
    std::cout << "list                          - list all opened file and directory descriptors\n\n";

    std::cout << "quit (q)                      - close interactive session\n\n\n";


    std::cout << "Commands can contain special characters that will be substituted during parsing:\n";
    std::cout << "Use $N to get Nth acquired file descriptor, eg. $1 - first acquired file descriptor\n";
    std::cout << "Use #N to get Nth acquired directory descriptor, eg. #2 - second acquired directory descriptor\n\n";
}
