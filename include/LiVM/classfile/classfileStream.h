//
// Created by lzx on 5/9/20.
//

#ifndef LIVM_CLASSFILESTREAM_H
#define LIVM_CLASSFILESTREAM_H

#include <shared/types.h>
#include <string>
#include <cassert>
#include "LiVM/LiVM.h"
#include "constantPool.h"
#include "attributeInfo.h"
#include <type_traits>
#define READ_U2(v, p)  v = ((p)[0]<<8)|(p)[1];
#define READ_U4(v, p)  v = ((p)[0]<<24)|((p)[1]<<16)|((p)[2]<<8)|(p)[3];
#define READ_U8(v, p)  v = ((u8)(p)[0]<<56)|((u8)(p)[1]<<48)|((u8)(p)[2]<<40) \
                            |((u8)(p)[3]<<32)|((u8)(p)[4]<<24)|((u8)(p)[5]<<16) \
                            |((u8)(p)[6]<<8)|(u8)(p)[7];
typedef LiVM::u1* cfstream_iterator ;
namespace LiVM {
    class ClassFileStream final {
    private:
        cfstream_iterator _front;
        cfstream_iterator _end;
        cfstream_iterator _current;
        std::string _source;
        bool _need_verify;
    public:
        ClassFileStream(): _front(nullptr), _end(nullptr), _current(nullptr), _source(){}
        void init(u1* content, size_t length);
        void set_current(cfstream_iterator pos){
            _current = pos;
        }
        void guaranteeMore(size_t s){
            auto remaining = (size_t) (_end - _current);
            if(s > remaining){
                PANIC("Unexpected EOF");
            }

        };
        u1 peek_u1(){
            return *_current;
        };
        u1 get_u1();
        u1 get_u1_fast(){
            return *_current++;
        }
        u4 get_u4();
        u4 get_u4_fast(){
            u4 res;
            READ_U4(res, _current);
            _current += 4;
            return res;
        }
        u2 peek_u2() const{
            u2 res;
            READ_U2(res, _current);
            return res;
        }
        u2 get_u2();
        u2 get_u2_fast(){
            u2 res;
            READ_U2(res, _current);
            _current += 2;
            return res;
        }

        std::unique_ptr<u1[]> getBytes(size_t length);
        void into(exception_table_t &);

        ClassFileStream& operator >>(CONSTANT_Utf8_info& info);
        ClassFileStream& operator >>(CONSTANT_Integer_info& info);
        ClassFileStream& operator >>(CONSTANT_Float_info& info);
        ClassFileStream& operator >>(CONSTANT_Class_info& info);
        ClassFileStream& operator >>(CONSTANT_Fieldref_info& info);
        ClassFileStream& operator >>(CONSTANT_Methodref_info& info);
        ClassFileStream& operator >>(CONSTANT_String_info& info);
        ClassFileStream& operator >>(CONSTANT_Long_info& info);
        ClassFileStream& operator >>(CONSTANT_Double_info& info);
        ClassFileStream& operator >>(CONSTANT_NameAndType_info& info);
        ClassFileStream& operator >>(CONSTANT_MethodHandle_info& info);
        ClassFileStream& operator >>(CONSTANT_MethodType_info& info);
        ClassFileStream& operator >>(CONSTANT_InvokeDynamic_info& info);
        ClassFileStream& operator >>(CONSTANT_InterfaceMethodref_info& info);
        ClassFileStream& operator >>(attribute_info& info);
        ClassFileStream& operator >>(ConstantValue_attribute& info);
        void into(attribute_info&);

    };
}
#endif //LIVM_CLASSFILESTREAM_H
