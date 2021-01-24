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
    char* login = "user1";
    char* password = "pass1";
    char* file = "/delTest.txt";

    auto * client = new Client();
    client->mynfs_opensession(host, login, password);

    int desc = client->mynfs_open("/1.txt", Client::O_RW);

    char buf[6];
    buf[5] = '\0';

    client->mynfs_read(desc, buf, 5);

    std::cout << desc << ' ' << buf << '\n';

    int n;
    std::cin >> n;

//    std::cout<<"Open session: "<<mynfs_strerror(client->mynfs_error)<<'\n';

//    int desc = client->mynfs_opendir("/");

//    std::cout<<desc<<'\n';

//    std::cout<<"Open dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';


//    std::cout<<client->mynfs_readdir(desc)<<'\n';
//    std::cout<<client->mynfs_readdir(desc)<<'\n';
//    std::cout<<client->mynfs_readdir(desc)<<'\n';
//    std::cout<<client->mynfs_readdir(desc)<<'\n';
//    std::cout<<client->mynfs_readdir(desc)<<'\n';
//    std::cout<<client->mynfs_readdir(desc)<<'\n';
//    std::cout<<client->mynfs_readdir(desc)<<'\n';


//    client->mynfs_closedir(desc);

//    std::cout<<"Close dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';

//    sleep(20);

    client->mynfs_closesession();

//    std::cout<<"Close session: "<<mynfs_strerror(client->mynfs_error)<<'\n';

    return 0;
}