#ifndef TIN_FILESMONITOR_H
#define TIN_FILESMONITOR_H

#include <unordered_map>
#include <mutex>

class FilesMonitor {
public:
    void add(const std::string &file, const std::string &user) {
        std::lock_guard lockGuard(m);
        try{
            files.at(file);
            // TODO: plik juz otwarty
            throw std::exception();
        }catch (std::out_of_range& e){
            files.insert({file, user});
        }
    }

    void remove(const std::string &file) {
        std::lock_guard lockGuard(m);
        files.erase(file);
    }

private:
    std::mutex m;
    std::unordered_map<std::string, std::string> files;
};

#endif //TIN_FILESMONITOR_H
