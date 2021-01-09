#ifndef TIN_FILESTAT_H
#define TIN_FILESTAT_H

struct FileStat{
    off_t     size;    /* total size, in bytes */
    blksize_t blksize; /* blocksize for file system I/O */
    blkcnt_t  blocks;  /* number of 512B blocks allocated */
    time_t    atime;   /* time of last access */
    time_t    mtime;   /* time of last modification */
    time_t    ctime;   /* time of last status change */
    int flag;
};

#endif //TIN_FILESTAT_H
