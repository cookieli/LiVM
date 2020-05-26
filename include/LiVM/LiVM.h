//
// Created by lzx on 5/11/20.
//

#ifndef LIVM_LIVM_H
#define LIVM_LIVM_H
#include <shared/string.h>
#define PANIC(fmt, ...) \
    do { \
       (void) fprintf(stderr, "\n\n\n*** (panic) *** [%s:%d]: " fmt "\n\n\n", \
                __FILE__, __LINE__, ##__VA_ARGS__); \
       exit(1); \
    } while (false)

namespace LiVM {
    struct Global {
        static String SLASH;
        static String DOT;
        static String UNDERLINE;
        static String PATH_SEPARATOR;
        static String PATH_DELIMITER;
        static String CLASS_EXTENSION;
    };
}
#endif //LIVM_LIVM_H
