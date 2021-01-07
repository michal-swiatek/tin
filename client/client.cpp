#include "client.h"
#include "../structures/structures.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <string>


Client::Client() {
    sockfd=0;
}

Client::~Client(){}

//private methods

int Client::send_message_and_wait_for_response( uint8_t *msg, uint32_t msg_len, char *recvBuff,
                                        uint32_t expected_response_size){

    send(sockfd, msg, msg_len, 0);
    read(sockfd, recvBuff, expected_response_size);

    return 0;
}

int Client::create_socket_and_open(char *host){
    struct sockaddr_in serv_addr{};

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error: Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);

    if(inet_pton(AF_INET, host, &serv_addr.sin_addr)<=0) {
        printf("\n Error: inet_pton error occured\n");
        return 1;
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error: Connection failed \n");
        return 1;
    }

    return 0;
}


//functions

int Client::mynfs_open( char* path, int oflag ){
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = CLIENT_OPEN_FILE;
    req_proto_struct.header1 = htonl(sizeof(path));
    req_proto_struct.header2 = htonl(oflag);
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    uint32_t descriptor = ntohl(resp_proto_struct->header2);
    return descriptor;
}

int Client::mynfs_read( int fd, char *buf, int size){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithData *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = CLIENT_READ_FILE;
    req_proto_struct.header1 = htonl(size);
    req_proto_struct.header2 = htonl(fd);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithData) );

    resp_proto_struct = (ProtoStructWithData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    int32_t buf_len = ntohl(resp_proto_struct->header2);
    memcpy(buf, resp_proto_struct->buf, buf_len);
    return buf_len;
}



int Client::mynfs_write(int fd, const char *buf, int size) {
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;

    //TODO czy tutaj size, czy ResponseSize?
    char response[RESPONSE_SIZE];
    req_proto_struct.command = CLIENT_WRITE_FILE;
    req_proto_struct.header1 = htonl(sizeof(buf));
    req_proto_struct.header2 = htonl(fd);
    memcpy(req_proto_struct.buf, (uint8_t *) buf, size);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    int buf_len = ntohl(resp_proto_struct->header2);
    return buf_len;
}

int Client::mynfs_lseek(int fd, int offset, int whence) {
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = CLIENT_LSEEK_FILE;
    req_proto_struct.header1 = htonl(offset);
    req_proto_struct.header2 = htonl(fd);

    memcpy(req_proto_struct.buf,  (uint8_t) whence, sizeof( (uint8_t) whence );

    send_message_and_wait_for_response(
            (uint8_t *) &req_proto_struct,
            sizeof(req_proto_struct),
            response,
            sizeof(ProtoStructWithoutData)
    );

    resp_proto_struct = (ProtoStructWithoutData *) response;

    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    int length = ntohl(resp_proto_struct->header2);
    return length;
}

int Client::mynfs_close(int fd) {
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = CLIENT_CLOSE_FILE;
    req_proto_struct.header1 = htonl(fd);
    req_proto_struct.header2 = htonl(0);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }

    return 0;
}

int Client::mynfs_unlink(  char* path){

    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = CLIENT_UNLINK_FILE;
    req_proto_struct.header1 = htonl(sizeof(path));
    req_proto_struct.header2 = htonl(0);
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    return 0;
}

int Client::mynfs_fstat(int fd, struct stat *buf){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithData *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = CLIENT_STAT_FILE;
    req_proto_struct.header1 = htonl(fd);
    req_proto_struct.header2 = htonl(0);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithData));

    resp_proto_struct = (ProtoStructWithData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    int32_t buf_len = ntohl(resp_proto_struct->header2);
    memcpy(buf, resp_proto_struct->buf, buf_len);
    return 0;
}


int Client::mynfs_opensession(char *host, char *login, char *passwd){

    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = CLIENT_OPEN_SESSION;

    std::string log(login);
    std::string pas(passwd);
    std::string logPas = log+pas;

    req_proto_struct.header1 = htonl(logPas.size());
    req_proto_struct.header2 = htonl(0);
    strcpy((char*)req_proto_struct.buf, (char*) logPas.c_str());

    if(create_socket_and_open(host)!=0) return -1;
    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    global_host=host;
    return 0;
}

int Client::mynfs_closesession(){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = CLIENT_CLOSE_SESSION;
    req_proto_struct.header1 = htonl(0);
    req_proto_struct.header2 = htonl(0);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }

    close(sockfd);

    return 0;
}


int Client::mynfs_opendir( char *path){
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = CLIENT_OPEN_DIR;
    req_proto_struct.header1 = htonl(sizeof(path));
    req_proto_struct.header2 = htonl(0);
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    int descriptor = ntohl(resp_proto_struct->header2);
    return descriptor;
}

char *Client::mynfs_readdir(int dir_fd){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithData *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = CLIENT_READ_DIR;
    req_proto_struct.header1 = htonl(dir_fd);
    req_proto_struct.header2 = htonl(0);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response,  sizeof(ProtoStructWithData));

    resp_proto_struct = (ProtoStructWithData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return nullptr;
    }
    int32_t buf_len = ntohl(resp_proto_struct->header2);
    char *folderName = nullptr;
    memcpy(folderName, resp_proto_struct->buf, buf_len);
    return folderName;
}

int Client::mynfs_closedir(int dir_fd){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = CLIENT_CLOSE_DIR;
    req_proto_struct.header1 = htonl(dir_fd);
    req_proto_struct.header2 = htonl(0);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }

    return 0;
}