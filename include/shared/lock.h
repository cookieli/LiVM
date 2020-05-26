//
// Created by lzx on 5/23/20.
//

#ifndef LIVM_LOCK_H
#define LIVM_LOCK_H
#include <mutex>

namespace LiVM {
    using Lock = std::mutex;
    using RecursiveLock = std::recursive_mutex;

    using LockGuard = std::lock_guard<Lock>;
    using RecursiveLockGuard = std::lock_guard<RecursiveLock>;

}
#endif //LIVM_LOCK_H
