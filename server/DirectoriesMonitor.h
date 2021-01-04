#ifndef TIN_DIRECTORIESMONITOR_H
#define TIN_DIRECTORIESMONITOR_H

#include <set>
#include <mutex>

class DirectoriesMonitor {
public:
    void add(const std::string &directory, const std::string &user) {
        std::lock_guard lockGuard(m);
        if(directories.find(std::make_pair(directory, user)) == directories.end()){
            directories.insert(std::make_pair(directory, user));
        }else{
            // TODO: directory juz otwarte
            throw std::exception();
        }
    }

    void remove(const std::string &directory, const std::string &user) {
        std::lock_guard lockGuard(m);
        directories.erase(std::make_pair(directory, user));
    }

private:
    std::mutex m;
    std::set<std::pair<std::string, std::string>> directories;
};

#endif //TIN_DIRECTORIESMONITOR_H
