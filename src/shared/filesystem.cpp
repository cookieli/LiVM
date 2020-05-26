//
// Created by lzx on 5/25/20.
//
#include <shared/filesystem.h>
#include <sys/stat.h>
#include <unistd.h>
namespace LiVM{
    bool FileSystem::isDirectory(const String &path) {
        struct stat s{};
        if(stat(strings::toStdString(path).c_str(), &s) == 0) {
            return S_ISDIR(s.st_mode);
        }
        return false;
    }

    bool FileSystem::canRead(const String &path) {
        int r = access(strings::toStdString(path).c_str(), R_OK);
        return r == 0;
    }

    size_t FileSystem::getFileSize(const String &path) {
        struct stat s {};
        if(stat(strings::toStdString(path).c_str(), &s) == 0) {
            return (size_t) s.st_size;
        }
        return 0;
    }


}
