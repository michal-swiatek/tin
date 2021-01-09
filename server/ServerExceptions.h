//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_SERVEREXCEPTIONS_H
#define TIN_SERVEREXCEPTIONS_H

#include <string>
#include <exception>
#include <stdexcept>

struct ServerError : public std::exception
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Server error!"; }
};

struct ServerRuntimeError : public std::runtime_error
{
    explicit ServerRuntimeError(const std::string& message) : std::runtime_error(message) {}
};

struct NotImplementedError : public ServerError
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Server method not implemented!"; }
};

struct DiskNotCreated : public ServerError
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Disk couldn't be created!"; }
};

struct FilesOwnersDirectoryNotExist : public ServerError
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Directory for filesOwnsers file doesn't exist!"; }
};

struct FilesOwnersDirectoryNotOpened : public ServerError
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Directory for filesOwnsers file couldn't be opened!"; }
};

#endif //TIN_SERVEREXCEPTIONS_H
