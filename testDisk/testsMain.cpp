//
// Created by root on 24.01.2021.
//

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "../client/Client.h"
#include <unistd.h>
#include "clientTests.h"

int main() {

    std::cout<<"\n Test: opensession_closesession_correctLoginPass()\n";
    opensession_closesession_correctLoginPass();
    std::cout<<"\n Test: opensession_badLogin()\n";
    opensession_badLogin();
    std::cout<<"\n Test: opensession_badUserName()\n";
    opensession_badUserName();
    std::cout<<"\n Test: opensession_incorrectHost()\n";
    opensession_incorrectHost();

    std::cout<<"\n Test: closesession_correct()\n";
    closesession_correct();

    std::cout<<"\n Test: open_correctAdmin_O_RDONLY()\n";
    open_correctAdmin_O_RDONLY();
    std::cout<<"\n Test: open_correctAdmin_O_WRONLY()\n";
    open_correctAdmin_O_WRONLY();
    std::cout<<"\n Test: open_correctAdmin_O_RDWR()\n";
    open_correctAdmin_O_RDWR();
    std::cout<<"\n Test: open_correctAdmin_O_CREAT()\n";
    open_correctAdmin_O_CREAT();

    std::cout<<"\n Test: open_correctUser_O_RDONLY()\n";
    open_correctUser_O_RDONLY();
    std::cout<<"\n Test: open_correctUser_O_WRONLY()\n";
    open_correctUser_O_WRONLY();
    std::cout<<"\n Test: open_correctUser_O_RDWR()\n";
    open_correctUser_O_RDWR();
    std::cout<<"\n Test: open_correctUser_O_CREAT()\n";
    open_correctUser_O_CREAT();

    std::cout<<"\n Test: open_fileNotExists()\n";
    open_fileNotExists();
    std::cout<<"\n Test: open_fileNotPermited()\n";
    open_fileNotPermited();

    std::cout<<"\n Test: read_correct()\n";
    read_correct();
    std::cout<<"\n Test: read_incorrectOpenMode()\n";
    read_incorrectOpenMode();
    std::cout<<"\n Test: read_incorrectDescriptor()\n";
    read_incorrectDescriptor();
    std::cout<<"\n Test: read_emptyFile()\n";
    read_emptyFile();

    std::cout<<"\n Test: write_correct()\n";
    write_correct();
    std::cout<<"\n Test: write_incorrectOpenMode()\n";
    write_incorrectOpenMode();
    std::cout<<"\n Test: write_incorrectDescriptor()\n";
    write_incorrectDescriptor();

    std::cout<<"\n Test: lseek_returnToBeginningOfTheFile()\n";
    lseek_returnToBeginningOfTheFile();
    std::cout<<"\n Test: lseek_skipFirstLetterInFile()\n";
    lseek_skipFirstSignInFile();
    std::cout<<"\n Test: lseek_skipSecondSignInFile()\n";
    lseek_skipSecondSignInFile();
    std::cout<<"\n Test: lseek_incorectDescriptor()\n";
    lseek_incorectDescriptor();
    std::cout<<"\n Test: lseek_endFile()\n";
    lseek_endFile();

    std::cout<<"\n Test: close_correct()\n";
    close_correct();
    std::cout<<"\n Test: close_incorrectDescriptor()\n";
    close_incorrectDescriptor();
    std::cout<<"\n Test: close_SecondUserTryClose()\n";
    close_SecondUserTryClose();

    std::cout<<"\n Test: unlink_noPermissionToFile()\n";
    unlink_noPermissionToFile();
    std::cout<<"\n Test: unlink_fileNotExists()\n";
    unlink_fileNotExists();
    std::cout<<"\n Test: unlink_otherUserHadOpenFile()\n";
    unlink_otherUserHadOpenFile();

    std::cout<<"\n Test: fstat_correct()\n";
    fstat_correct();
    std::cout<<"\n Test: fstat_incorrectDescriptor()\n";
    fstat_incorrectDescriptor();

    std::cout<<"\n Test: opendir_correct()\n";
    opendir_correct();
    std::cout<<"\n Test: opendir_secondUserTry()\n";
    opendir_secondUserTry();
    std::cout<<"\n Test: opendir_pathNotExists()\n";
    opendir_pathNotExists();

    std::cout<<"\n Test: readdir_correct()\n";
    readdir_correct();
    std::cout<<"\n Test: readdir_incorrectDescriptor()\n";
    readdir_incorrectDescriptor();
    std::cout<<"\n Test: readdit_twoUsers()\n";
    readdit_twoUsers();
    std::cout<<"\n Test: readdir_correctReadMultipleTimes()\n";
    readdir_correctReadMultipleTimes();

    std::cout<<"\n Test: closedir_correct()\n";
    closedir_correct();
    std::cout<<"\n Test: closedir_incorrectDescriptor()\n";
    closedir_incorrectDescriptor();

    std::cout<<"\n Test: user_createWriteReadCloseUnlinkfile_andTryToRead()\n";
    user_createWriteReadCloseUnlinkfile_andTryToRead();

    return 0;
}