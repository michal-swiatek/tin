#ifndef TIN_FUNCTIONS_H
#define TIN_FUNCTIONS_H

//int mynfs_open(char *host, char* path, int oflag, int mode );
int mynfs_open( char* path, int oflag, int mode );
int mynfs_read( int fd, char *buf, int size);
int mynfs_write(int fd, const char *buf, int size);
int mynfs_lseek(int fd, int offset, int whence);
int mynfs_close(int fd);
//int mynfs_unlink( char *host, char* path);
int mynfs_unlink( char* path);
int mynfs_fstat(int fd, struct stat *buf);

//int mynfs_opendir( char *host, char *path);
int mynfs_opendir( char *path);
char *mynfs_readdir(int dir_fd);
int mynfs_closedir(int dir_fd);

int mynfs_opensession(char *host, char *login, char *passwd);
int mynfs_closesession(char *host);

#endif //TIN_FUNCTIONS_H
