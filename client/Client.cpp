#include "Client.h"
#include "../utilities/Protocol.h"
#include "../utilities/Requests.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <string>
#include <stdint.h>
#include <sys/stat.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdio.h>

#include <cstdint>


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
    //serv_addr.sin_port = htons(DEFAULT_PORT);
    serv_addr.sin_port = DEFAULT_PORT;

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

int Client::mynfs_open(char* path, int oflag ){
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = Request(C_OPEN_FILE);
    Header header{};
    header.param1 = htonl(sizeof(path));
    header.param2 = htonl(oflag);
    req_proto_struct.header = header;
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }
    uint32_t descriptor = ntohl(resp_proto_struct->header.param2);
    return descriptor;
}

int Client:: mynfs_read(int fd, char *buf, int size){
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = Request(C_READ_FILE);
    Header header{};
    header.param1 = htonl(size);
    header.param2 = htonl(fd);
    req_proto_struct.header = header;

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStruct) );

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }
    int32_t buf_len = ntohl(resp_proto_struct->header.param2);
    memcpy(buf, resp_proto_struct->buf, buf_len);
    return buf_len;
}



int Client::mynfs_write(int fd, const char *buf, int size) {
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;

    //TODO czy tutaj size, czy ResponseSize?
    char response[RESPONSE_SIZE];
    req_proto_struct.command = Request(C_WRITE_FILE);
    Header header{};
    header.param1 = htonl(sizeof(buf));
    header.param2 = htonl(fd);
    req_proto_struct.header = header;
    memcpy(req_proto_struct.buf, (uint8_t *) buf, size);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }
    int buf_len = ntohl(resp_proto_struct->header.param2);
    return buf_len;
}

//int Client::mynfs_lseek(int fd, int offset, int whence) {
//    ProtoStruct req_proto_struct{};
//    ProtoStruct *resp_proto_struct;
//    char response[RESPONSE_SIZE];
//
//   req_proto_struct.command = Request(C_FILE_LSEEK);
//    req_proto_struct.header1 = htonl(offset);
//    req_proto_struct.header2 = htonl(fd);
//
//   memcpy(req_proto_struct.buf,  (uint8_t ) whence, sizeof( (uint8_t) whence ) );
//   // memcpy(req_proto_struct.buf,   whence, sizeof( whence ) );
//
//    send_message_and_wait_for_response(
//            (uint8_t *) &req_proto_struct,
//            sizeof(req_proto_struct),
//            response,
//            sizeof(ProtoStruct)
//    );
//
//    resp_proto_struct = (ProtoStruct *) response;
//
//    uint32_t error = ntohl(resp_proto_struct->header1);
//    if (error != 0) {
//        return -1;
//    }
//    int length = ntohl(resp_proto_struct->header2);
//    return length;
//}

int Client::mynfs_close(int fd) {
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = Request(C_CLOSE_FILE);
    Header header{};
    header.param1 = htonl(fd);
    header.param2 = htonl(0);
    req_proto_struct.header = header;

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }

    return 0;
}

int Client::mynfs_unlink(char* path){

    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = Request(C_UNLINK_FILE);
    Header header{};
    header.param1 = htonl(sizeof(path));
    header.param2 = htonl(0);
    req_proto_struct.header = header;
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }
    return 0;
}

int Client::mynfs_fstat(int fd, struct stat *buf){
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = Request(C_FILE_STAT);
    Header header{};
    header.param1 = htonl(fd);
    header.param2 = htonl(0);
    req_proto_struct.header = header;

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }
    int32_t buf_len = ntohl(resp_proto_struct->header.param2);
    memcpy(buf, resp_proto_struct->buf, buf_len);
    return 0;
}


int Client::mynfs_opensession(char *host, char *login, char *passwd){

    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = Request(C_CONNECT);

    std::string log(login);
    std::string pas(passwd);
    std::string logPas = log+pas;

    Header header{};
    header.param1 = htonl(logPas.size());
    header.param2 = htonl(0);
    req_proto_struct.header = header;
    strcpy((char*)req_proto_struct.buf, (char*) logPas.c_str());

    if(create_socket_and_open(host)!=0) return -1;
    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }
    return 0;
}

int Client::mynfs_closesession(){
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = Request(C_DISCONNECT);
    Header header{};
    header.param1 = htonl(0);
    header.param2 = htonl(0);
    req_proto_struct.header = header;

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }

    close(sockfd);

    return 0;
}


int Client::mynfs_opendir( char *path){
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];

    req_proto_struct.command = Request(C_OPEN_DIR);
    Header header{};
    header.param1 = htonl(sizeof(path));
    header.param2 = htonl(0);
    req_proto_struct.header = header;
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }
    int descriptor = ntohl(resp_proto_struct->header.param2);
    return descriptor;
}

char *Client::mynfs_readdir(int dir_fd){
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = Request(C_READ_DIR);
    Header header{};
    header.param1 = htonl(dir_fd);
    header.param2 = htonl(0);
    req_proto_struct.header = header;

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response,  sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return nullptr;
    }
    int32_t buf_len = ntohl(resp_proto_struct->header.param2);
    char *folderName = nullptr;
    memcpy(folderName, resp_proto_struct->buf, buf_len);
    return folderName;
}

int Client::mynfs_closedir(int dir_fd){
    ProtoStruct req_proto_struct{};
    ProtoStruct *resp_proto_struct;
    char response[RESPONSE_SIZE];
    req_proto_struct.command = Request(C_CLOSE_DIR);
    Header header{};
    header.param1 = htonl(dir_fd);
    header.param2 = htonl(0);
    req_proto_struct.header = header;

    send_message_and_wait_for_response( (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStruct));

    resp_proto_struct = (ProtoStruct *) response;
    uint32_t error = ntohl(resp_proto_struct->header.param1);
    if (error != 0) {
        return -1;
    }

    return 0;
}
