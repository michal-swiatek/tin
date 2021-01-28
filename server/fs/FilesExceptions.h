#ifndef TIN_FILESEXCEPTIONS_H
#define TIN_FILESEXCEPTIONS_H

struct FileNotExist : public std::exception {
    [[nodiscard]] inline const char *what() const noexcept override { return "File doesn't exist."; }
};

struct FileNotPermitted : public std::exception {
    [[nodiscard]] inline const char *
    what() const noexcept override { return "User doesn't have permission to open this file."; }
};

struct FileNotOpened : public std::exception {
    [[nodiscard]] inline const char *what() const noexcept override { return "File couldn't be opened."; }
};

struct FileReadOnly : public std::exception {
    [[nodiscard]] inline const char *what() const noexcept override { return "File is read only."; }
};

struct FileWriteOnly : public std::exception {
    [[nodiscard]] inline const char *what() const noexcept override { return "File is write only."; }
};

struct FileNotUnlinked : public std::exception {
    [[nodiscard]] inline const char *what() const noexcept override { return "File couldn't be unlinked."; }
};

struct FileAlreadyOpened : public std::exception {
    [[nodiscard]] inline const char *what() const noexcept override { return "File is already opened."; }
};

#endif //TIN_FILESEXCEPTIONS_H
