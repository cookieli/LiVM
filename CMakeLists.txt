cmake_minimum_required(VERSION 3.16)
project(LiVM)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
set(SOURCE_FILES
        include/LiVM/classfile/attributeInfo.h
        include/LiVM/classfile/classFile.h
        include/LiVM/classfile/classfileParser.h
        include/LiVM/classfile/classfileStream.h
        include/LiVM/classfile/constantPool.h
        include/LiVM/classpath/classLoader.h
        include/LiVM/classpath/classPathManager.h
        include/LiVM/classpath/system.h
        include/LiVM/LiVM.h
        include/shared/filesystem.h
        include/shared/hashMap.h
        include/shared/libzippp.h
        include/shared/lock.h
        include/shared/string.h
        include/shared/types.h
        include/sparsepp/spp.h
        include/sparsepp/spp_config.h
        include/sparsepp/spp_dlalloc.h
        include/sparsepp/spp_memory.h
        include/sparsepp/spp_smartptr.h
        include/sparsepp/spp_stdint.h
        include/sparsepp/spp_timer.h
        include/sparsepp/spp_traits.h
        include/sparsepp/spp_utils.h
        src/bin/javap.cpp
        src/bin/main.cpp
        src/LiVM/classfile/attributeInfo.cpp
        src/LiVM/classfile/classFile.cpp
        src/LiVM/classfile/classFileParser.cpp
        src/LiVM/classfile/classfileStream.cpp
        src/LiVM/classfile/constantPool.cpp
        src/LiVM/classpath/baseClassLoader.cpp
        src/LiVM/classpath/classLoader.cpp
        src/LiVM/classpath/classPathManager.cpp
        src/LiVM/classpath/LiVM.cpp
        src/LiVM/classpath/system.cpp
        src/shared/filesystem.cpp
        src/shared/libzippp.cpp
        src/shared/string.cpp)
add_executable(KivaVM ${SOURCE_FILES} src/bin/main.cpp)
add_executable(javap ${SOURCE_FILES} src/bin/javap.cpp)
