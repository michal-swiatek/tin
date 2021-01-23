#ifndef TIN_DIRECTORIESEXCEPTIONS_H
#define TIN_DIRECTORIESEXCEPTIONS_H

struct DirectoryEndReached : public std::exception
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Directory end reached.."; }
};

struct DirectoryNotOpened : public std::exception
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Directory couldn't be opened."; }
};

struct DirectoryAlreadyOpened : public std::exception
{
    [[nodiscard]] inline const char* what() const noexcept override { return "Directory is already opened."; }
};

#endif //TIN_DIRECTORIESEXCEPTIONS_H
