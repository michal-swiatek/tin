#ifndef TIN_FILESMONITOR_H
#define TIN_FILESMONITOR_H

#include <unordered_map>
#include <mutex>

class FilesMonitor
{
public:
    void add(const std::string& file, const std::string& user)
    {
        // TODO: co jak juz jest plik
        std::lock_guard lockGuard(m);
        files.insert({file, user});
    }

    std::string& find(const std::string& file)
    {
        std::lock_guard lockGuard(m);
        return files.at(file);
    }

    void remove(const std::string& file)
    {
        std::lock_guard lockGuard(m);
        files.erase(file);
    }

private:
    std::mutex m;
    std::unordered_map<std::string, std::string> files;
};

#endif //TIN_FILESMONITOR_H
