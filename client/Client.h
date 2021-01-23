#ifndef TIN_CLIENT_H
#define TIN_CLIENT_H

#include "../utilities/Protocol.h"
#include "../utilities/Requests.h"
#include "../utilities/Errors.h"
#include "../utilities/FileStat.h"

#include <cstdint>
#include <vector>

class Client {
private:
    Errors mynfs_error;
    int sockfd;

    void sendMessage( Header& header, std::vector<char>& data ) const;
    void readMessage( Header& header, std::vector<char>& data ) const;
public:
    Client();
    ~Client();

    int mynfs_open(char* path, int oflag );
    int mynfs_read(int fd, char *buf, int size);
    int mynfs_write(int fd, const char *buf, int size);
    int mynfs_lseek(int fd, int offset, int whence);
    int mynfs_close(int fd);
    int mynfs_unlink(char* path);
    int mynfs_fstat(int fd, FileStat *buf);

    int mynfs_opendir(char *path);
    char* mynfs_readdir(int dir_fd);
    int mynfs_closedir(int dir_fd);

    int mynfs_opensession(char *host, char *login, char *passwd);
    int mynfs_closesession();


    int getNFSError() const;
};


#endif //TIN_CLIENT_H
