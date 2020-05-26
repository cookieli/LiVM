//
// Created by lzx on 5/8/20.
//

#ifndef LIVM_CLASSFILE_H
#define LIVM_CLASSFILE_H
#include <memory>
#include "../../shared/types.h"
#include "constantPool.h"
#include "attributeInfo.h"

namespace LiVM {
    struct field_info {
        u2 access_flags;
        u2 name_index;
        u2 descriptor_index;
        u2 attributes_count;
        unique_arr<attribute_info> attributes;
    };
    using method_info = field_info;
    struct ClassFile final{
    public:
        u4 magic{};
        u2 minor_version{};
        u2 major_version{};
        u2 constant_pool_count{};
        unique_arr<cp_info> constant_pool; //its length is constant_pool_count-1
        u2 access_flags{};
        u2 this_class{};
        u2 super_class{};
        u2 interface_count{};
        std::unique_ptr<u2[]> interfaces;
        u2 fields_count{};
        std::unique_ptr<field_info[]> fields;
        u2 methods_count{};
        std::unique_ptr<method_info[]> methods = nullptr;
        u2 attributes_count{};
        unique_arr<attribute_info> attributes = nullptr;
    public:
        u2 getAttributeTag(int index);
        void printConstantPool();
    };
}
#endif //LIVM_CLASSFILE_H
