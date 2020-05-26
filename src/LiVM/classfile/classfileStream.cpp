//
// Created by lzx on 5/11/20.
//
#include <LiVM/classfile/classfileStream.h>


namespace LiVM {
    using namespace std;
    void ClassFileStream::init(u1* content, size_t length){
        _front = content;
        _end   = content + length;
        set_current(content);
    }
    u4 ClassFileStream::get_u4() {
        guaranteeMore(4);
        return get_u4_fast();
    }
    u1 ClassFileStream::get_u1() {
        guaranteeMore(1);
        return get_u1_fast();
    }
    u2 ClassFileStream::get_u2() {
        guaranteeMore(2);
        return get_u2_fast();
    }
    std::unique_ptr<u1 []> ClassFileStream::getBytes(size_t length) {
        auto res = make_unique<u1[]>(length);
        for(int i = 0; i < length; i++){
            res[i] = get_u1();
        }
        return res;
    }
    ClassFileStream& ClassFileStream::operator >>(attribute_info& info){
        info.attribute_name_index = get_u2();
        info.attribute_length     = get_u4();
        return *this;
    }
    ClassFileStream& ClassFileStream::operator>>(CONSTANT_Utf8_info &info) {
        info.tag = get_u1();
        info.length = get_u2();
        info.bytes = make_unique<u1[]>(info.length);
        for(u2 i =0; i < info.length; i++){
            info.bytes[i] = get_u1();
        }
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_Integer_info &info) {
        info.tag = get_u1();
        info.bytes = get_u4();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_Float_info &info) {
        info.tag = get_u1();
        info.bytes = get_u4();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_Class_info &info) {
        info.tag = get_u1();
        info.name_index = get_u2();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_Fieldref_info &info) {
        info.tag = get_u1();
        info.class_index = get_u2();
        info.name_and_type_index = get_u2();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_Methodref_info &info) {
        info.tag = get_u1();
        info.class_index = get_u2();
        info.name_and_type_index = get_u2();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_String_info &info) {
        info.tag = get_u1();
        info.string_index = get_u2();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_Long_info &info) {
        info.tag = get_u1();
        info.high_bytes = get_u4();
        info.low_bytes  = get_u4();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_Double_info &info) {
        info.tag = get_u1();
        info.high_bytes = get_u4();
        info.low_bytes = get_u4();
        return *this;
    }
    ClassFileStream & ClassFileStream::operator>>(CONSTANT_NameAndType_info &info) {
        info.tag = get_u1();
        info.name_index = get_u2();
        info.descriptor_index = get_u2();
        return *this;
    }
    ClassFileStream& ClassFileStream::operator>>(CONSTANT_MethodHandle_info &info) {
        info.tag = get_u1();
        info.reference_kind = get_u1();
        info.reference_index = get_u2();
        return *this;
    }
    ClassFileStream& ClassFileStream::operator>>(CONSTANT_MethodType_info &info) {
        info.tag = get_u1();
        info.descriptor_index = get_u2();
        return *this;
    }
    ClassFileStream& ClassFileStream::operator>>(CONSTANT_InvokeDynamic_info &info) {
        info.tag = get_u1();
        info.bootstrap_method_attr_index = get_u2();
        info.name_and_type_index = get_u2();
        return *this;
    }
    ClassFileStream& ClassFileStream::operator>>(CONSTANT_InterfaceMethodref_info &info) {
        info.tag = get_u1();
        info.class_index = get_u2();
        info.name_and_type_index = get_u2();
        return *this;
    }
    ClassFileStream& ClassFileStream::operator>>(ConstantValue_attribute &info) {
        operator>>((attribute_info&)info);
        info.constantValue_index = get_u2();
        return *this;
    }
    void ClassFileStream::into(exception_table_t &t) {
        t.start_pc   = get_u2();
        t.end_pc     = get_u2();
        t.handler_pc = get_u2();
        t.catch_type = get_u2();
    }
   void ClassFileStream::into(attribute_info& info) {
        info.attribute_name_index = get_u2();
        info.attribute_length   = get_u4();
    }



}
