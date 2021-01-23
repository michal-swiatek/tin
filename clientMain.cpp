#include <iostream>
#include <fcntl.h>
#include "client/Client.h"

int main() {

    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "superhaslo1";

    auto * client = new Client();
    client->mynfs_opensession(host, login, password);

    std::cout<<"Open session: "<<mynfs_strerror(client->mynfs_error)<<'\n';


    int desc = client->mynfs_open("/23.txt", Client::O_RW);

    std::cout<<desc<<'\n';

    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';





    client->mynfs_close(desc);

    std::cout<<"Close file: "<<mynfs_strerror(client->mynfs_error)<<'\n';


    client->mynfs_closesession();

    std::cout<<"Close session: "<<mynfs_strerror(client->mynfs_error)<<'\n';

    return 0;
}