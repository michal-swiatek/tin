//
// Created by ula on 07.01.2021.
//

#ifndef TIN_CLIENT_H
#define TIN_CLIENT_H

#include <stdint.h>

const int DEFAULT_PORT=8080;
const int RESPONSE_SIZE=4096;

class Client {
private:
    int sockfd;
    int send_message_and_wait_for_response( uint8_t *msg, uint32_t msg_len, char *recvBuff,
                                           uint32_t expected_response_size);
    int create_socket_and_open(char *host);
public:
    Client();
    ~Client();

    int mynfs_open( char* path, int oflag );
    int mynfs_read( int fd, char *buf, int size);
    int mynfs_write(int fd, const char *buf, int size);
    int mynfs_lseek(int fd, int offset, int whence);
    int mynfs_close(int fd);
    int mynfs_unlink( char* path);
    int mynfs_fstat(int fd, struct stat *buf);

    int mynfs_opendir( char *path);
    char *mynfs_readdir(int dir_fd);
    int mynfs_closedir(int dir_fd);

    int mynfs_opensession(char *host, char *login, char *passwd);
    int mynfs_closesession();

};


#endif //TIN_CLIENT_H
