//
// Created by lzx on 5/14/20.
//

#ifndef LIVM_STRING_H
#define LIVM_STRING_H

#include <string>

#include <vector>
#include "types.h"

namespace LiVM {
    namespace strings {
        using String = std::wstring;

        String fromBytes(u1 *bytes, size_t length);

        std::string toStdString(const String &str);

        String fromStdString(const std::string &str);

        String replaceAll(const String &string, const String &oldValue, const String &newValue);

        std::vector<String> split(const String &string, const String &delimiter);
    }
    using String = strings::String ;
}
#endif //LIVM_STRING_H
