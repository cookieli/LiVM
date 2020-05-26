//
// Created by lzx on 5/25/20.
//

#ifndef LIVM_FILESYSTEM_H
#define LIVM_FILESYSTEM_H

#include "string.h"

namespace LiVM {
    class FileSystem final {
    public:
        static bool isDirectory(const String &path);

        static bool canRead(const String &path);

        static size_t getFileSize(const String &path);
    };
}
#endif //LIVM_FILESYSTEM_H
