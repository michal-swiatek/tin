#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "client/Client.h"
#include <unistd.h>

int main() {

//    int d = ::open("./disk/2.txt", O_RDWR);
//
//    int size = 5;
//
//    char* buff = new char[size];
//
//
//
//    ::read(d, );




    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();
    client->mynfs_opensession(host, login, password);

    std::cout<<"Open session: "<<mynfs_strerror(client->mynfs_error)<<'\n';


//    int desc = client->mynfs_open("/2.txt", Client::O_RW);
//
//    std::cout<<desc<<'\n';
//
//    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
//
//    char letters[5] = "dupa";
//    auto ret = client->mynfs_write(desc, letters, 5);
//
//    std::cout << "Write: "<< ret << '\n';
//
//    std::cout<<"Write file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
//
//    char recMess[2];
//    auto retREad = client->mynfs_read(desc, recMess, 2);
//
//    std::cout << retREad << ' ' << recMess << '\n';
//
//    std::cout<<"Read file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
//
//
//
//
//    int xd = client->mynfs_lseek(desc, 0, Client::S_SET);
//
//    std::cout << xd << '\n';
//
//    std::cout<<"Offset: "<<mynfs_strerror(client->mynfs_error)<<'\n';
//
//
//
//
//
//    retREad = client->mynfs_read(desc, recMess, 2);
//
//    std::cout << retREad << ' ' << recMess << '\n';
//
//    std::cout<<"Read file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
//
//    client->mynfs_close(desc);
//
//    std::cout<<"Close file: "<<mynfs_strerror(client->mynfs_error)<<'\n';

    sleep(20);

    client->mynfs_closesession();

    std::cout<<"Close session: "<<mynfs_strerror(client->mynfs_error)<<'\n';

    return 0;
}