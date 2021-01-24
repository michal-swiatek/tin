#ifndef TIN_CLIENT_H
#define TIN_CLIENT_H

#include "../utilities/Protocol.h"
#include "../utilities/Requests.h"
#include "../utilities/Errors.h"
#include "../utilities/FileStat.h"

#include <cstdint>
#include <vector>
#include <fcntl.h>

const char *mynfs_strerror(Errors error);

class Client {
private:
    int sockfd;
    void sendMessage( Header& header, std::vector<char>& data ) const;

    void readMessage( Header& header, std::vector<char>& data ) const;
public:
    enum FileFlag{
        O_RO = O_RDONLY,
        O_WO = O_WRONLY,
        O_RW = O_RDWR,
        O_CR = O_CREAT
    };

    enum Whence{
        S_SET = SEEK_SET,
        S_CUR = SEEK_CUR,
        S_END = SEEK_END
    };

    Client();
    ~Client();
    int mynfs_open(char* path, FileFlag oflag );

    int mynfs_read(int fd, char *buf, int size);
    int mynfs_write(int fd, const char *buf, int size);
    int mynfs_lseek(int fd, int offset, Whence whence);
    int mynfs_close(int fd);
    int mynfs_unlink(char* path);
    int mynfs_fstat(int fd, FileStat *buf);
    int mynfs_opendir(char *path);

    char* mynfs_readdir(int dir_fd);
    int mynfs_closedir(int dir_fd);
    int mynfs_opensession(const char *host, const char *login, const char *passwd);

    int mynfs_closesession();

    Errors mynfs_error;
};

#endif //TIN_CLIENT_H
