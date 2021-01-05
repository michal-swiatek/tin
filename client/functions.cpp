#include "functions.h"
#include "response.h"
#include "host.h"
#include "../structures/structures.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/stat.h>
#include <cstring>
#include <string>

#define ResponseSize 4096

int mynfs_open(char *host, char* path, int oflag, int mode ) {
    // TODO Co z mode???
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[ResponseSize];

    req_proto_struct.command = ClientOpenFile;
    req_proto_struct.header1 = htonl(sizeof(path));
    req_proto_struct.header2 = htonl(oflag);
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response(host, (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    uint32_t descriptor = ntohl(resp_proto_struct->header2);
    return descriptor;
}

int mynfs_read( int fd, char *buf, int size) {
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithData *resp_proto_struct;
    char response[size];
    req_proto_struct.command = ClientReadFile;
    req_proto_struct.header1 = htonl(sizeof(buf));
    req_proto_struct.header2 = htonl(fd);

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, (uint32_t) size);

    resp_proto_struct = (ProtoStructWithData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    int32_t buf_len = ntohl(resp_proto_struct->header2);
    memcpy(buf, resp_proto_struct->buf, buf_len);
    return buf_len;
}

int mynfs_write(int fd, const char *buf, int size) {
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;

    //czy tutaj size, czy ResponseSize?
    char response[size];
    req_proto_struct.command = ClientWriteFile;
    req_proto_struct.header1 = htonl(sizeof(buf));
    req_proto_struct.header2 = htonl(fd);
    memcpy(req_proto_struct.buf, (uint8_t *) buf, size);

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    int buf_len = ntohl(resp_proto_struct->header2);
    return buf_len;
}

int mynfs_lseek(int fd, int offset, int whence) {
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[ResponseSize];

    req_proto_struct.command = ClientLseekFile;
    req_proto_struct.header1 = htonl(offset);
    req_proto_struct.header2 = htonl(fd);
    uint8_t arr[] = { (uint8_t) whence };
//    memcpy(req_proto_struct.buf, arr, sizeof(arr));
//
//    send_message_and_wait_for_response(
//            (char*)global_host.c_str(),
//            (uint16_t)global_port,
//            (uint8_t *) &req_proto_struct,
//            sizeof(req_proto_struct),
//            response,
//            sizeof(ResponseOperation)
//    );
//
//    res_message = (ResponseOperation *) response;
//    int buf_len = ntohl(res_message->buf_len);
//
//    return buf_len;
}

int mynfs_close(int fd) {
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[ResponseSize];
    req_proto_struct.command = ClientCloseFile;
    req_proto_struct.header1 = htonl(fd);
    req_proto_struct.header2 = 0;

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }

    return 0;
}

int mynfs_unlink(  char* path){

    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[ResponseSize];

    req_proto_struct.command = ClientOpenFile;
    req_proto_struct.header1 = htonl(sizeof(path));
    req_proto_struct.header2 = 0;
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    return 0;
}

int mynfs_fstat(int fd, struct stat *buf){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithData *resp_proto_struct;
    char response[ResponseSize];
    req_proto_struct.command = ClientStatFile;
    req_proto_struct.header1 = htonl(fd);
    req_proto_struct.header2 = 0;

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct,
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




int mynfs_opensession(char *host, char *login, char *passwd){

    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[ResponseSize];

    req_proto_struct.command = ClientOpenSession;

    std::string log(login);
    std::string pas(passwd);
    std::string logPas = log+pas;

    req_proto_struct.header1 = htonl(logPas.size());
    req_proto_struct.header2 = 0;
    strcpy((char*)req_proto_struct.buf, (char*) logPas.c_str());

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    global_host=host;
    return 0;
}

//TODO czy my nie powinniśmy tutaj przesyłać protokołu coby ustawić, że użytkownik jest już nie zalogowany?
int mynfs_closesession(char *host){

}


int mynfs_opendir( char *path){
    ProtoStructWithData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[ResponseSize];

    req_proto_struct.command = ClientOpenDir;
    req_proto_struct.header1 = htonl(sizeof(path));
    req_proto_struct.header2 = 0;
    strcpy((char*)req_proto_struct.buf, path);

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct, sizeof(req_proto_struct), response,
                                       sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }
    uint32_t descriptor = ntohl(resp_proto_struct->header2);
    return descriptor;
}

char *mynfs_readdir(int dir_fd){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithData *resp_proto_struct;
    char response[ResponseSize];
    req_proto_struct.command = ClientReadDir;
    req_proto_struct.header1 = htonl(dir_fd);
    req_proto_struct.header2 = 0;

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct,
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

int mynfs_closedir(int dir_fd){
    ProtoStructWithoutData req_proto_struct{};
    ProtoStructWithoutData *resp_proto_struct;
    char response[ResponseSize];
    req_proto_struct.command = ClientCloseDir;
    req_proto_struct.header1 = htonl(dir_fd);
    req_proto_struct.header2 = 0;

    send_message_and_wait_for_response((char*)global_host.c_str(), (uint8_t *) &req_proto_struct,
                                       sizeof(req_proto_struct), response, sizeof(ProtoStructWithoutData));

    resp_proto_struct = (ProtoStructWithoutData *) response;
    uint32_t error = ntohl(resp_proto_struct->header1);
    if (error != 0) {
        return -1;
    }

    return 0;
}