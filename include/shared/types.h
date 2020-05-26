//
// Created by lzx on 5/8/20.
//

#ifndef LIVM_TYPES_H
#define LIVM_TYPES_H
#include <memory>
namespace LiVM {
    typedef unsigned char u1;
    typedef unsigned short u2;
    typedef unsigned int u4;
    template<typename T>
    using unique_arr = std::unique_ptr<std::unique_ptr<T>[]>;
}

#endif //LIVM_TYPES_H
