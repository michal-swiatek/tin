//
// Created by root on 24.01.2021.
//

#ifndef TIN_TESTS_H
#define TIN_TESTS_H


/*********************************************************************************************************************/

//testy do funkcji mynfs_opensession(char *host, char *login, char *passwd)

//poprawne otwarcie i zamknięcie sesji - został podany poprawny host, login i password
//mynfs_sterror: NO_ERROR
//returned value: 0
void opensession_closesession_correctLoginPass();

//niepoprawne otwarcie sesji - został podany niepoprawny host
//mynfs_sterror: BAD_LOGIN
//returned value: -1
void opensession_incorrectHost();

//niepoprawne otwarcie sesji - zostało podane niepoprawne hasło
//mynfs_sterror: BAD_LOGIN
//returned value: -1
void opensession_badLogin();

//niepoprawne otwarcie sesji - zostało podane niepoprawna nazwa użytkownika
//mynfs_sterror: COULD_NOT_CREATE_SOCKET
//returned value: -1
void opensession_badUserName();

/*********************************************************************************************************************/
//testy do funkcji int mynfs_closesession();

//poprawne zamknięcie otwartej sesji użytkownika
//mynfs_sterror:   NO_ERROR
//returned value: 0
void closesession_correct();

//testy do funkcji int mynfs_open(char* path, int oflag );

//poprawne otwarcie  pliku przez administratora z flagą O_RDONLY
//mynfs_sterror:   NO_ERROR
//returned value: descriptor
void open_correctAdmin_O_RDONLY();

//poprawne otwarcie  pliku przez administratora z flagą O_WRONLY
//mynfs_sterror:   NO_ERROR
//returned value: descriptor
void open_correctAdmin_O_WRONLY();

//poprawne otwarcie  pliku przez administratora z flagą O_RDWR
//mynfs_sterror:   NO_ERROR
//returned value: descriptor
void open_correctAdmin_O_RDWR();

//poprawne otwarcie pliku przez administratora z flagą O_CREAT
//a następnie usunięcie utworzonego przez niego pliku
//mynfs_open(): mynfs_sterror:   NO_ERROR;  returned value: descriptor
//mynfs_unlink(): mynfs_sterror:   NO_ERROR;  returned value: descriptor
void open_correctAdmin_O_CREAT();

//poprawne otwarcie poprawne pliku przez użytkownika z flagą O_RDONLY
//mynfs_sterror:   NO_ERROR
//returned value: descriptor
void open_correctUser_O_RDONLY();

//poprawne otwarcie poprawne pliku przez użytkownika z flagą O_WRONLY
//mynfs_sterror:   NO_ERROR
//returned value: descriptor
void open_correctUser_O_WRONLY();

//poprawne otwarcie poprawne pliku przez użytkownika z flagą O_RDWR
//mynfs_sterror:   NO_ERROR
//returned value: descriptor
void open_correctUser_O_RDWR();

//poprawne otwarcie pliku przez użytkownika z flagą O_CREAT
//a następnie usunięcie utworzonego przez niego pliku
//mynfs_open(): mynfs_sterror:   NO_ERROR;  returned value: descriptor
//mynfs_unlink(): mynfs_sterror:   NO_ERROR;  returned value: descriptor
void open_correctUser_O_CREAT();


//niepoprawne otwarcie pliku - próba otwarcia nieistniejącego pliku
//mynfs_sterror: FILE_NOT_EXISTS
//returned value: -1
void open_fileNotExists();

//niepoprawne otwarcie pliku - próba otwarcia pliku przez użytkownika, do którego nie ma praw dostępu
//mynfs_sterror: INVALID_FLAG_VALUE
//returned value: -1
void open_fileNotPermited();

/*********************************************************************************************************************/

//testy do funkcji int mynfs_read();

//poprawne przeczytanie z pliku
//mynfs_sterror: NO_ERROR
//returned value: liczba przeczytanych znaków (3)
void read_correct();

//TODO: ustawia tutaj błąd na OTHER_ERROR
//próba przeczytani z pliku nie otwartego w trybie do odczytu
//mynfs_sterror:
//returned value: -1
void read_incorrectOpenMode();
//próba odczytanie pliku z podaniem błędnego deskryptora
//mynfs_sterror: INVALID_DESCRIPTOR
//returned value: -1
void read_incorrectDescriptor();

//próba odczytania z pustego pliku - utworzenie nowego pliku, a następnie próba odczytania z niego
//mynfs_sterror: NO_ERROR
//returned value: liczba przeczytanych znaków (0)
void read_emptyFile();

/*********************************************************************************************************************/
//testy do funkcji int mynfs_write();

//poprawny zapis tekstu do pliku
//mynfs_sterror: NO_ERROR
//returned value: liczba zapisanych znaków (3)
void write_correct();
//niepoprawna próba zapisu do pliku otwartego w trybie read only
//mynfs_sterror:FILE_READ_ONLY
//returned value: -1
void write_incorrectOpenMode();

//zapis do pliku z podaniem błędnego deskryptora
//mynfs_sterror: INVALID_DESCRIPTOR
//returned value: -1
void write_incorrectDescriptor();

/*********************************************************************************************************************/
//testy do funkcji int mynfs_lseek(int fd, int offset, int whence);

//odczyt z pliku pierwszego znaku tekstu, następnie zmiana kursora na początek pliku i odczytanie jeszcze raz tego samego znaku
void lseek_returnToBeginningOfTheFile();
//przesunięcie kursora na drugi znak w pliku i odczytanie drugiego znaku tekstu znajdującego się w pliku
void lseek_skipFirstSignInFile();
//odczytanie pierwszego znaku z pliku, następnie przesunięcie o 1 kursora od bieżącej pozycji i odczytanie trzeciego znaku z tekstu
void lseek_skipSecondSignInFile();
//wywołanie lseek z błędnym deskryptorem
void lseek_incorectDescriptor();
//przesuniecie kursora na koniec pliku i próba odczytu
void lseek_endFile();

/*********************************************************************************************************************/
//testy do funkcji int mynfs_close(int fd);

//poprawne zamknięcie otwartego pliku
void close_correct();
//niepoprawne zamknięcie pliku z błędnym deskryptorem
void close_incorrectDescriptor();
//próba zamknięcia pliku przez użytkownika nie mającego go otwartego, gdy jest on otwarty przez innego użytkownika
//mynfs_sterror: INVALID_DESCRIPTOR
void close_SecondUserTryClose();

/*********************************************************************************************************************/
//testy do funkcji int mynfs_unlink(char* path);

//stworzenie nowego pliku, a następnie poprawne usunięcie go jest jednym z testów funkcji open

//niepoprawna próba usunięcia pliku, przez użytkownika, który nie ma do pliku praw dostępu
void unlink_noPermissionToFile();
//usunięcie nieistniejącego pliku
void unlink_fileNotExists();
//próba usunięcia pliku, który jest otwarty przez drugiego użytkownika
void unlink_otherUserHadOpenFile();

/*********************************************************************************************************************/
// testy do funkcji int mynfs_fstat(int fd, struct stat *buf);

//poprawne odczytanie atrybutów danego pliku
void fstat_correct();

//próba odczytania fstat przy podaniu błędnego deskryptora
void fstat_incorrectDescriptor();
/*********************************************************************************************************************/
//testy do funkcji int mynfs_opendir(char *path)

//poprawne otwarcie katalogu
void opendir_correct();

//poprawne otwarcie katalogu przez drugiego użytkownika, gdy został on już otwarty przez innego użytkownika
void opendir_secondUserTry();

//próba otwarcia katalogu z podaniem nieistaniejącej ścieżki
void opendir_pathNotExists();
/*********************************************************************************************************************/
//testy do funkcji char *mynfs_readdir(int dir_fd);
//poprawne odczytanie katalogu
void readdir_correct();
//niepoprawne odczytanie katalogu - błędny deskryptor
void readdir_incorrectDescriptor();
//odczytanie katalogu otwartego przez dwóch użytkowników w tym samym momencie
void readdit_twoUsers();
//odczytanie kilkukrotne tego samego katalogu, aż do osiągnięcia końca folderu
void readdir_correctReadMultipleTimes();
/*********************************************************************************************************************/
//testy do funkcji int mynfs_closedir(int dir_fd);

//poprawne zamknięcie katalogu
void closedir_correct();
//próba zamknięcia katalogu z podaniem błędnego deskryptora
void closedir_incorrectDescriptor();
/*********************************************************************************************************************/
//TODO: ogólnie coś nie działa z tym testem - nie chce odczytać tekstu Uwaga - trzeba jeszcze lseek przestawić
//stworz nowy plik i wpisz cos do niego, odczytaj co tam wpisales, zamknij, usun i proboj odczytac ponownie i ma byc bład ze juz nie ma pliku
void user_createWriteReadCloseUnlinkfile_andTryToRead();
#endif //TIN_TESTS_H
