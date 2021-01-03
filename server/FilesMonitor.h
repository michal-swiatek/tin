//
// Created by micho6 on 03.01.2021.
//

#ifndef TIN_FILESMONITOR_H
#define TIN_FILESMONITOR_H

#include <set>
#include <mutex>

class FilesMonitor
{
public:
    void add(int fd)
    {
        std::lock_guard lockGuard(m);
        files.insert(files.end(), fd);
    }

    void remove(int fd)
    {
        std::lock_guard lockGuard(m);
        files.erase(fd);
    }

private:
    std::mutex m;
    std::set<int> files;
};

#endif //TIN_FILESMONITOR_H
