#include <iostream>
#include "client/Client.h"

int main() {

    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "superhaslo1";

    auto * client = new Client();
    client->mynfs_opensession(host, login, password);

    std::cout<<mynfs_strerror(client->mynfs_error)<<'\n';

    return 0;
}