//
// Created by lzx on 5/23/20.
//
#include <LiVM/classpath/system.h>

namespace LiVM {
    SystemDictionary *SystemDictionary::get() {
        static SystemDictionary dictionary;
        return &dictionary;
    }
    Klass * SystemDictionary::find(const String &name) {
        LockGuard lockGuard(this->_lock);
        const auto &iter = this->_classes.find(name);
        return iter != this->_classes.end() ? iter->second: nullptr;
    }
    void SystemDictionary::put(const String &name, Klass* klass) {
        LockGuard lockGuard(this->_lock);
        this->_classes.insert(std::make_pair(name, klass));
    }
}

