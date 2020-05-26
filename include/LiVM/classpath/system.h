//
// Created by lzx on 5/23/20.
//

#ifndef LIVM_SYSTEM_H
#define LIVM_SYSTEM_H

#include <shared/hashMap.h>
#include <shared/string.h>
#include <shared/lock.h>

namespace LiVM {
    class Klass;

    class SystemDictionary final {
    private:
        HashMap<String, Klass*> _classes;
        Lock _lock;
    public:
        static SystemDictionary *get();

        Klass *find(const String &name);

        void put(const String &name, Klass *);

        inline const HashMap<String, Klass*> &getLoadedClasses() const {
            return _classes;
        }
    };
}
#endif //LIVM_SYSTEM_H
