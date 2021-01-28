#ifndef TIN_FILESTAT_H
#define TIN_FILESTAT_H

#include <sys/types.h>

static const int MAX_PATH_LEN = 64;
static const int MAX_USERNAME_LEN = 64;

struct FileStat {
    off_t size;               /* total size, in bytes */
    time_t lastAccess;         /* time of last access */
    time_t lastModification;   /* time of last modification */
    time_t lastStatusChange;   /* time of last status change */
    int flag;
    char filePath[MAX_PATH_LEN];
    char owner[MAX_USERNAME_LEN];
};

#endif //TIN_FILESTAT_H
