//
// Created by lzx on 5/23/20.
//

#ifndef LIVM_CLASSLOADER_H
#define LIVM_CLASSLOADER_H
#pragma once

#include <memory>
#include <shared/string.h>

namespace LiVM {
    class Klass;
    class ClassLoader {
    public:
        static std::unique_ptr<Klass> requireClass(ClassLoader *classLoader, const String &className);

        static ClassLoader* getCurrentClassLoader();

        virtual Klass* loadClass(const String &className) = 0;

        virtual Klass* loadClass(u1 *classBytes, size_t classSize) = 0;

        ClassLoader() = default;

        ClassLoader(const ClassLoader &) =delete;

        ClassLoader(ClassLoader &&) noexcept  = delete;

        virtual ~ClassLoader() = default;
    };

    class BaseClassLoader : public ClassLoader {
    public:
        Klass* loadClass(const String &className) override;

        Klass* loadClass(u1 *classBytes, size_t classSize) override;
    };

    class BootstrapClassLoader : public BaseClassLoader {
    public:
        static BootstrapClassLoader *get();

        Klass* loadClass(const String &className) override;

        Klass* loadClass(u1 *classBytes, size_t classSize) override;
    };
}
#endif //LIVM_CLASSLOADER_H
