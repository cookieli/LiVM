//
// Created by lzx on 5/23/20.
//
#include <LiVM/classpath/classLoader.h>
#include <shared/lock.h>
#include <LiVM/classpath/system.h>
namespace LiVM {
    static RecursiveLock &bootstrapLock() {
        static RecursiveLock lock;
        return lock;
    }

    BootstrapClassLoader* BootstrapClassLoader::get() {
        static BootstrapClassLoader classLoader;
        return &classLoader;
    }

    Klass* BootstrapClassLoader::loadClass(const String &className) {
        RecursiveLockGuard guard(bootstrapLock());
        auto iter = SystemDictionary::get()->find(className);
        if(iter != nullptr){
            return iter;
        }
        Klass *klass = BaseClassLoader::loadClass(className);
        if(klass != nullptr){
//            SystemDictionary::get()->put(className, std::unique_ptr<Klass>(klass));
        }
        return klass;
    }
}

