//
// Created by lzx on 5/24/20.
//
#include <LiVM/classpath/classPathManager.h>
#include <shared/filesystem.h>
#include <shared/libzippp.h>
namespace LiVM {
    void ClassPathManager::initialize() {
        ClassPathManager *cpm = ClassPathManager::get();
        const char *classpathEnv = getenv("CLASSPATH");
        if(classpathEnv != nullptr) {
            const String &classpath = strings::fromStdString(classpathEnv);
            cpm->addClassPaths(classpath);
        }
    }

    ClassPathManager *ClassPathManager::get() {
        static ClassPathManager classPathManager;
        return &classPathManager;
    }

    void ClassPathManager::addClassPath(const String& path) {
        if(FileSystem::isDirectory(path)){
            _runtimeClassPath.push_back({ClassSource::DIR, path, nullptr});
            return;
        }
        if (FileSystem::canRead(path)){
            auto zip = new libzippp::ZipArchive(strings::toStdString(path));
            zip->open(libzippp::ZipArchive::OpenMode::READ_ONLY);
            if(zip->isOpen()){

            }
        }
    }


}
