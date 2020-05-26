//
// Created by lzx on 5/8/20.
//

#ifndef LIVM_CONSTANTPOOL_H
#define LIVM_CONSTANTPOOL_H

#include "../../shared/types.h"
#include <memory>
#include <shared/string.h>
//Constant pool tags */

#define CONSTANT_Utf8                    1
#define CONSTANT_Integer                 3
#define CONSTANT_Float                   4
#define CONSTANT_Long                    5
#define CONSTANT_Double                  6
#define CONSTANT_Class                   7
#define CONSTANT_String                  8
#define CONSTANT_Fieldref                9
#define CONSTANT_Methodref              10
#define CONSTANT_InterfaceMethodref     11
#define CONSTANT_NameAndType            12
#define CONSTANT_MethodHandle           15
#define CONSTANT_MethodType             16
#define CONSTANT_InvokeDynamic          18

#define ATTRIBUTE_ConstantValue 0
#define ATTRIBUTE_Code 1
#define ATTRIBUTE_StackMapTable 2
#define ATTRIBUTE_Exceptions 3
#define ATTRIBUTE_InnerClasses 4
#define ATTRIBUTE_EnclosingMethod 5
#define ATTRIBUTE_Synthetic 6
#define ATTRIBUTE_Signature 7
#define ATTRIBUTE_SourceFile 8
#define ATTRIBUTE_SourceDebugExtension 9
#define ATTRIBUTE_LineNumberTable 10
#define ATTRIBUTE_LocalVariableTable 11
#define ATTRIBUTE_LocalVariableTypeTable 12
#define ATTRIBUTE_Deprecated 13
#define ATTRIBUTE_RuntimeVisibleAnnotations 14
#define ATTRIBUTE_RuntimeInvisibleAnnotations 15
#define ATTRIBUTE_RuntimeVisibleParameterAnnotations 16
#define ATTRIBUTE_RuntimeInvisibleParameterAnnotations 17
#define ATTRIBUTE_RuntimeVisibleTypeAnnotations 18
#define ATTRIBUTE_RuntimeInvisibleTypeAnnotations 19
#define ATTRIBUTE_AnnotationDefault 20
#define ATTRIBUTE_BootstrapMethods 21
#define ATTRIBUTE_MethodParameters 22
#define ATTRIBUTE_INVALID 99

#define REF_getField                 1
#define REF_getStatic                2
#define REF_putField                 3
#define REF_putStatic                4
#define REF_invokeVirtual            5
#define REF_invokeStatic             6
#define REF_invokeSpecial            7
#define REF_newInvokeSpecial         8
#define REF_invokeInterface          9

/* JVM Specification §4.4.4 */
#define _FLOAT_POSITIVE_INFINITY    0x7f800000
#define _FLOAT_NEGATIVE_INFINITY    0xff800000
#define FLOAT_IS_POSITIVE_INFINITY(x)    ((x) == _FLOAT_POSITIVE_INFINITY)
#define FLOAT_IS_NEGATIVE_INFINITY(x)    ((x) == _FLOAT_NEGATIVE_INFINITY)
#define FLOAT_NAN                   0x7f800001
#define FLOAT_IS_NAN(x) (((x) >= 0x7f800001 && (x) <= 0x7fffffff) \
                            || ((x) >= 0xff800001 && (x) <= 0xffffffff))

#define _DOUBLE_POSITIVE_INFINITY   0x7ff0000000000000L
#define _DOUBLE_NEGATIVE_INFINITY   0xfff0000000000000L
#define DOUBLE_IS_POSITIVE_INFINITY(x)    ((x) == _DOUBLE_POSITIVE_INFINITY)
#define DOUBLE_IS_NEGATIVE_INFINITY(x)    ((x) == _DOUBLE_NEGATIVE_INFINITY)
#define DOUBLE_NAN                  0x7ff0000000000001L
#define DOUBLE_IS_NAN(x) (((x) >= 0x7ff0000000000001L && (x) <= 0x7fffffffffffffffL) \
                            || ((x) >= 0xfff0000000000001L && (x) <= 0xffffffffffffffffL))
namespace LiVM {
    struct cp_info{
        u1 tag;
        cp_info() = default;
        ~cp_info() = default;
    };
    struct CONSTANT_Class_info: public cp_info {
        u2 name_index;
    };

    struct CONSTANT_Fieldref_info: public cp_info {
        u2 class_index;
        u2 name_and_type_index;
    };
    struct CONSTANT_Methodref_info: public cp_info {
        u2 class_index;
        u2 name_and_type_index;
    };
    struct CONSTANT_String_info: public cp_info{
        u2 string_index;
    };
    struct CONSTANT_Integer_info: public cp_info{
        u4 bytes;
        int getConstant() const{
            return static_cast<int>(bytes);
        }
    };
    struct CONSTANT_Float_info: public cp_info {
        u4 bytes;
        float getConstant() const;
    };
    struct CONSTANT_Long_info: public cp_info {
        u4 high_bytes;
        u4 low_bytes;
        long getConstant() const {
            return ((long) high_bytes << 32) + low_bytes;
        }
    };
    struct CONSTANT_Double_info: public cp_info {
        u4 high_bytes;
        u4 low_bytes;
        double getConstant() const;
    };
    struct CONSTANT_NameAndType_info: public cp_info {
        u2 name_index;
        u2 descriptor_index;
    };
    struct CONSTANT_Utf8_info: public cp_info {
    public:
        u2 length;
        std::unique_ptr<u1[]> bytes;
        CONSTANT_Utf8_info();
        String getConstant() ;

    private:
        bool _cached;
        String _cached_string;

    };
    struct CONSTANT_MethodHandle_info: public cp_info{
        u1 reference_kind;
        u2 reference_index;
    };
    struct CONSTANT_MethodType_info: public cp_info {
        u2 descriptor_index;
    };
    struct CONSTANT_InvokeDynamic_info: public cp_info {
        u2 bootstrap_method_attr_index;
        u2 name_and_type_index;
    };
    struct CONSTANT_InterfaceMethodref_info: public cp_info {
        u2 class_index;
        u2 name_and_type_index;
    };



}
#endif //LIVM_CONSTANTPOOL_H
