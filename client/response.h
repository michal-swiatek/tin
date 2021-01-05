#ifndef TIN_RESPONSE_H
#define TIN_RESPONSE_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8080

int send_message_and_wait_for_response(char *server_ip, uint8_t *msg, uint32_t msg_len, char *recvBuff,
                                       uint32_t expected_response_size, uint16_t port=DEFAULT_PORT);

#endif //TIN_RESPONSE_H
