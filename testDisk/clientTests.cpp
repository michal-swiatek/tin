//
// Created by root on 24.01.2021.
//

#include "clientTests.h"
#include "../client/Client.h"
#include <iostream>
#include <unistd.h>
#include <unistd.h>


void opensession_closesession_correctLoginPass(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    returnedValue = client->mynfs_opensession(host, login, password);

    std::cout<<"Open session: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_closesession();
    std::cout<<"Close session: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
}

void opensession_incorrectHost(){
    char* host = "0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();
    int returnedValue = 0;
    returnedValue = client->mynfs_opensession(host, login, password);

    std::cout<<"Open session: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
}

void opensession_badLogin(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass";

    auto * client = new Client();
    int returnedValue = 0;
    returnedValue = client->mynfs_opensession(host, login, password);

    std::cout<<"Open session: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
}

void opensession_badUserName(){
    char* host = "0.0.0.0";
    char* login = "user";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    returnedValue = client->mynfs_opensession(host, login, password);

    std::cout<<"Open session: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
}

/*********************************************************************************************************************/

void closesession_correct(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_closesession();
    std::cout<<"Close session: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
}

/*********************************************************************************************************************/

void open_correctAdmin_O_RDONLY(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/2.txt", Client::O_RW);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void open_correctAdmin_O_WRONLY(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/2.txt", Client::O_WO);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void open_correctAdmin_O_RDWR(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/2.txt", Client::O_RW);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void open_correctAdmin_O_CREAT(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/admin_new.txt", Client::O_CR);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue =client->mynfs_unlink("/admin_new.txt");
    std::cout<<"Unlink created file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
    client->mynfs_closesession();
}

void open_correctUser_O_RDONLY(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void open_correctUser_O_WRONLY(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_CR);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void open_correctUser_O_RDWR(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}


void open_correctUser_O_CREAT(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/user_new.txt", Client::O_CR);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_unlink("/user_new.txt");
    std::cout<<"Unlink created file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}


void open_fileNotExists(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/nazwaaaakjhd.txt", Client::O_RW);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void open_fileNotPermited(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/2.txt", Client::O_RW);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

/*********************************************************************************************************************/

void read_correct(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[3];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 3);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void read_incorrectOpenMode(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[3];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_WO);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 3);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void read_incorrectDescriptor(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[3];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(-1, answer, 3);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

//próba odczytania z pustego pliku
//mynfs_sterror: NO_ERROR
//returned value: liczba przeczytanych znaków (0)
void read_emptyFile(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[3];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/pusty.txt", Client::O_CR);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 3);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

/*********************************************************************************************************************/
//testy do funkcji int mynfs_write();

void write_correct(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    char buf[6] = "tekst";
    returnedValue = client->mynfs_write(descriptor, buf, 6);
    std::cout<<"Write to file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Written text: "<<buf<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}


void write_incorrectOpenMode(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RO);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    char buf[6] = "tekst";
    returnedValue = client->mynfs_write(descriptor, buf, 6);
    std::cout<<"Write to file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Written text: "<<buf<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

//zapis do pliku z podaniem błędnego deskryptora
//mynfs_sterror: INVALID_DESCRIPTOR
//returned value: -1
void write_incorrectDescriptor(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    char buf[6] = "tekst";
    returnedValue = client->mynfs_write(-8, buf, 6);
    std::cout<<"Write to file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Written text: "<<buf<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

/*********************************************************************************************************************/

void lseek_returnToBeginningOfTheFile(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[1];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 1);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_lseek(descriptor, 0, Client::S_SET);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 1);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

//przesunięcie kursora na drugi znak w pliku i odczytanie drugiego znaku tekstu znajdującego się w pliku
void lseek_skipFirstSignInFile(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[1];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_lseek(descriptor, 1, Client::S_SET);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 1);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void lseek_skipSecondSignInFile(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[1];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 1);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_lseek(descriptor, 1, Client::S_CUR);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 1);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

//wywołanie lseek z błędnym deskryptorem
void lseek_incorectDescriptor(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';


    returnedValue = client->mynfs_lseek(-8, 0, Client::S_SET);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';


    client->mynfs_closesession();
}

void lseek_endFile(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char answer[1];
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_lseek(descriptor, 1, Client::S_END);
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 1);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

/*********************************************************************************************************************/

//poprawne zamknięcie otwartego pliku
void close_correct(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_close(descriptor);
    descriptor = returnedValue;
    std::cout<<"Close file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void close_incorrectDescriptor(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_close(-5);
    std::cout<<"Close file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void close_SecondUserTryClose(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    char* host2 = "0.0.0.0";
    char* login2 = "user1";
    char* password2 = "pass1";

    auto * client2 = new Client();

    client2->mynfs_opensession(host2, login2, password2);
    std::cout<<"Client2 opensession: "<<mynfs_strerror(client2->mynfs_error)<<'\n';

    returnedValue = client2->mynfs_close(descriptor);
    std::cout<<"Close file: "<<mynfs_strerror(client2->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
    client2->mynfs_closesession();
}
/*********************************************************************************************************************/
void unlink_noPermissionToFile(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_unlink("/2.txt");
    std::cout<<"Unlink created file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void unlink_fileNotExists(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_unlink("/nieistniejaca_nazwa.txt");
    std::cout<<"Unlink created file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

void unlink_otherUserHadOpenFile(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    char* host2 = "0.0.0.0";
    char* login2 = "user1";
    char* password2 = "pass1";

    auto * clientAdmin = new Client();

    clientAdmin->mynfs_opensession(host2, login2, password2);
    std::cout<<"Client2 opensession: "<<mynfs_strerror(clientAdmin->mynfs_error)<<'\n';

    returnedValue = clientAdmin->mynfs_unlink("/1.txt");
    std::cout<<"Unlink file: "<<mynfs_strerror(clientAdmin->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
    clientAdmin->mynfs_closesession();
}

/*********************************************************************************************************************/

void fstat_correct(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();
    FileStat *buf = new FileStat();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_open("/1.txt", Client::O_RW);
    descriptor = returnedValue;
    std::cout<<"Open file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_fstat(descriptor,buf );
    std::cout<<"Get fstat file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned FileStat: "<<'\n';
    std::cout<<"   Size: "<<buf->size<<'\n';
    std::cout<<"   LastAccess: "<<buf->lastAccess<<'\n';
    std::cout<<"   lastModification: "<<buf->lastModification<<'\n';
    std::cout<<"   lastStatusChange: "<<buf->lastStatusChange<<'\n';
    std::cout<<"   flag: "<<buf->flag<<'\n';
    std::cout<<"   filePath: "<<buf->filePath<<'\n';
    std::cout<<"   owner: "<<buf->owner<<'\n';

    client->mynfs_closesession();
}

void fstat_incorrectDescriptor(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();
    FileStat *buf = new FileStat();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_fstat(-8,buf );
    std::cout<<"Get fstat file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
}
/*********************************************************************************************************************/

void opendir_correct(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char *buf;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_opendir("/");
    std::cout<<"Open dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
    client->mynfs_closesession();
}

//próba otwarcia katalogu przez drugiego użytkownika, gdy został on już otwarty przez innego użytkownika
void opendir_secondUserTry(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_opendir("/");
    std::cout<<"Open dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    char* host2 = "0.0.0.0";
    char* login2 = "user1";
    char* password2 = "pass1";

    auto * clientAdmin = new Client();

    clientAdmin->mynfs_opensession(host2, login2, password2);

    returnedValue = clientAdmin->mynfs_opendir("/");
    std::cout<<"Open dir: "<<mynfs_strerror(clientAdmin->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
    clientAdmin->mynfs_closesession();
}

//próba otwarcia katalogu z podaniem nieistaniejącej ścieżki
void opendir_pathNotExists(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char *buf;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_opendir("/ksalfl");
    std::cout<<"Open dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
    client->mynfs_closesession();
}

/*********************************************************************************************************************/

void readdir_correct(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char *buf;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_opendir("/");
    std::cout<<"Open dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
    descriptor = returnedValue;
    buf = client->mynfs_readdir(descriptor);
    std::cout<<"Read dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<buf<<'\n';

    client->mynfs_closesession();
}
//niepoprawne odczytanie katalogu - błędny deskryptor
void readdir_incorrectDescriptor(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char *buf;
    client->mynfs_opensession(host, login, password);

    buf = client->mynfs_readdir(-8);
    std::cout<<"Read dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';

    client->mynfs_closesession();
}
//odczytanie katalogu otwartego przez dwóch użytkowników w tym samym momencie
void readdit_twoUsers(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();
    char *buf;

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_opendir("/");
    descriptor = returnedValue;
    std::cout<<"Open dir client1: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    char* host2 = "0.0.0.0";
    char* login2 = "user1";
    char* password2 = "pass1";

    auto * clientAdmin = new Client();

    clientAdmin->mynfs_opensession(host2, login2, password2);

    returnedValue = clientAdmin->mynfs_opendir("/");
    std::cout<<"Open dir client2: "<<mynfs_strerror(clientAdmin->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    buf = client->mynfs_readdir(descriptor);
    std::cout<<"Read dir client1: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<buf<<'\n';

    buf = clientAdmin->mynfs_readdir(returnedValue);
    std::cout<<"Read dir client2: "<<mynfs_strerror(clientAdmin->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<buf<<'\n';

    client->mynfs_closesession();
    clientAdmin->mynfs_closesession();
}

void readdir_correctReadMultipleTimes(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    char *buf;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_opendir("/");
    std::cout<<"Open dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
    descriptor = returnedValue;
    for(int i =0; i<100;i++) {
        buf = client->mynfs_readdir(descriptor);
        std::cout << "Read dir: " << mynfs_strerror(client->mynfs_error) << '\n';
        if( buf != nullptr )
            std::cout << "Function returned value: " << buf << '\n';
        else break;
    }

    client->mynfs_closesession();
}
/*********************************************************************************************************************/
void closedir_correct(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_opendir("/");
    std::cout<<"Open dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
    descriptor = returnedValue;
    returnedValue = client->mynfs_closedir(descriptor);
    std::cout<<"Close dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}
//próba zamknięcia katalogu z podaniem błędnego deskryptora
void closedir_incorrectDescriptor(){
    char* host = "0.0.0.0";
    char* login = "user1";
    char* password = "pass1";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);

    returnedValue = client->mynfs_closedir(-8);
    std::cout<<"Close dir: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

/*********************************************************************************************************************/
//stworz nowy plik i wpisz cos do niego, odczytaj co tam wpisales, zamknij, usun i proboj odczytac ponownie i ma byc bład ze juz nie ma pliku
void user_createWriteReadCloseUnlinkfile_andTryToRead(){
    char* host = "0.0.0.0";
    char* login = "user2";
    char* password = "pass2";

    auto * client = new Client();

    int returnedValue = 0;
    int descriptor;
    client->mynfs_opensession(host, login, password);


    returnedValue = client->mynfs_open("/user_new.txt", Client::O_CR);
    descriptor = returnedValue;
    std::cout<<"Open and create file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';
    char buf[10] = "cokolwiek";
    char answer[10];
    returnedValue = client->mynfs_write(descriptor, buf, 10);
    std::cout<<"Write to file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Written text: "<<buf<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_lseek(descriptor,0, Client::S_SET);
    std::cout<<"Lseek to beginning of the file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_read(descriptor, answer, 10);
    std::cout<<"Read from file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Read text:"<<answer<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_close(descriptor);
    std::cout<<"Close file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_unlink("/user_new.txt");
    std::cout<<"Unlink created file: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    returnedValue = client->mynfs_open("/user_new.txt", Client::O_RO);
    std::cout<<"Open file again: "<<mynfs_strerror(client->mynfs_error)<<'\n';
    std::cout<<"Function returned value: "<<returnedValue<<'\n';

    client->mynfs_closesession();
}

