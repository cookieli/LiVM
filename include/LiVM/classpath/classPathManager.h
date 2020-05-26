//
// Created by lzx on 5/24/20.
//

#ifndef LIVM_CLASSPATHMANAGER_H
#define LIVM_CLASSPATHMANAGER_H
#include <shared/string.h>
#include <shared/libzippp.h>
#include <list>
namespace LiVM {
    enum ClassSource {
        NOT_FOUND,
        DIR,
        JAR
    };

    struct ClassSearchResult final {
        String _file;
        int _fd{};
        ClassSource _source;
        u1 *_buffer = nullptr;
        size_t bufferSize{};

        ClassSearchResult(const String &file, int fd, ClassSource source, u1 *buffer, size_t bufferSize);

        void closeResource() const;
    };

    struct ClassPathEntry final {
        ClassSource _source;
        String      _path;
        std::unique_ptr<libzippp::ZipArchive> _cookie;
    };

    class ClassPathManager final {
    private:
        std::list<ClassPathEntry> _runtimeClassPath;
    public:
        void initialize();

        static ClassPathManager *get();

        void addClassPath(const String& path);

        void addClassPaths(const String &classpath);

        ClassSearchResult searchClass(const String &className);

        void destroy();
    };
}
#endif //LIVM_CLASSPATHMANAGER_H
