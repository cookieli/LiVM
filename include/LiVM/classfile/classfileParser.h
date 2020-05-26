//
// Created by lzx on 5/10/20.
//

#ifndef LIVM_CLASSFILEPARSER_H
#define LIVM_CLASSFILEPARSER_H

#include <string>
#include <memory>
#include "classFile.h"
#include "classfileStream.h"

namespace LiVM {
    class ClassfileParser final{
    public:
        ClassfileParser(const std::string &filepath);

        std::unique_ptr<ClassFile> parse();
        void parseConstantPool(ClassFile& cf);
        void parseInterfaces(ClassFile& cf);
        void parseFields(ClassFile& cf);
        void parseMethods(ClassFile& cf);
        method_info getMethodInfo(ClassFile& cf);
        field_info getFieldInfo(ClassFile& cf);
        std::unique_ptr<attribute_info>
                parseAttributeInfo(ClassFile& cf);
        std::unique_ptr<Code_attribute>
                parseCodeAttribute(ClassFile& cf);
        std::unique_ptr<StackMapTable_attribute>
                parseStackMapTableAttribute();
        std::unique_ptr<Exceptions_attribute>
                parseExceptionsAttribute();
        std::unique_ptr<InnerClasses_attribute>
                parseInnerClassesAttribute();
        std::unique_ptr<EnclosingMethod_attribute>
                parseEnclosingMethod();
        std::unique_ptr<Synthetic_attribute>
                parseSyntheticAttribute();
        std::unique_ptr<Signature_attribute>
                parseSignatureAttribute();
        std::unique_ptr<SourceFile_attribute>
                parseSourceFileAttribute();
        std::unique_ptr<SourceDebugExtension_attribute>
                parseSourceDebugExtensionAttribute();
        std::unique_ptr<LineNumberTable_attribute>
                parseLineNumberTableAttribute();
        std::unique_ptr<LocalVariableTable_attribute>
                parseLocalVariableTableAttribute();
        std::unique_ptr<LocalVariableTypeTable_attribute>
                parseLocalVariableTypeTableAttribute();
        std::unique_ptr<RuntimeVisibleAnnotations_attribute>
                parseRuntimeVisibleAnnotationsAttribute();
        std::unique_ptr<RuntimeVisibleParameterAnnotations_attribute>
                parseRuntimeVisibleParameterAnnotationsAttribute();
        std::unique_ptr<RuntimeInvisibleParameterAnnotations_attribute>
                parseRuntimeInvisibleParameterAnnotationsAttribute();
        template<typename T>
        std::unique_ptr<T> parseRuntimeTypeAnnotationsAttribute();
        std::unique_ptr<AnnotationDefault_attribute>
                parseAnnotationDefaultAttribute();
        std::unique_ptr<BootstrapMethods_attribute>
                parseBootstrapMethodsAttribute();
        std::unique_ptr<MethodParameters_attribute>
                parseMethodParametersAttribute();

        type_annotation parseTypeAnnotation();
        parameter_annotations_t parseParameterAnnotations();
        std::unique_ptr<annotation>
                parseAnnotation();
        std::unique_ptr<element_value_pairs_t>
                parseElementValuePairs_t();
        element_value parseElementValue();
        std::unique_ptr<stack_map_frame>
                parseStackMapFrame();
        std::unique_ptr<vertification_type_info>
                parseVertificationTypeInfo();
        std::unique_ptr<same_locals_1_stack_item_frame>
                parseSameLocals1StackItemFrame(u1 frame_type);
        std::unique_ptr<same_locals_1_stack_item_frame_extended>
                parseSameLocals1StackItemFrameExtended(u1 frame_type);
        std::unique_ptr<chop_frame>
                parseChopFrame(u1 frame_type);
        std::unique_ptr<same_frame_extended>
                parseSameFrameExtended(u1 frame_type);
        std::unique_ptr<append_frame>
                parseAppendFrame(u1 frame_type);
        std::unique_ptr<full_frame>
                parseFullFrame(u1 frame_type);
        template<typename T>
        std::unique_ptr<std::unique_ptr<cp_info>[]>
                readPoolEntry(unique_arr<cp_info> pool, int index,
                                            ClassFileStream& stream) {
                    pool[index] = std::make_unique<T>();
                    cp_info* p = pool[index].get();
                    stream >> (T&)*p;
                    return pool;
                }

    private:
        size_t _size;
        std::unique_ptr<u1[]> _content;
        std::unique_ptr<ClassFileStream> _classFileStream;
        template<typename T>
        T getInfo(ClassFile& cf);
    };
}
#endif //LIVM_CLASSFILEPARSER_H
