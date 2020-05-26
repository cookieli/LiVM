//
// Created by lzx on 5/10/20.
//
#include <LiVM/classfile/classFile.h>

namespace LiVM {
    u2 ClassFile::getAttributeTag(int index) {
        static HashMap<String, u2> ATTRIBUTE_MAPPING{
                {L"ConstantValue",                        ATTRIBUTE_ConstantValue},
                {L"Code",                                 ATTRIBUTE_Code},
                {L"StackMapTable",                        ATTRIBUTE_StackMapTable},
                {L"Exceptions",                           ATTRIBUTE_Exceptions},
                {L"InnerClasses",                         ATTRIBUTE_InnerClasses},
                {L"EnclosingMethod",                      ATTRIBUTE_EnclosingMethod},
                {L"Synthetic",                            ATTRIBUTE_Synthetic},
                {L"Signature",                            ATTRIBUTE_Signature},
                {L"SourceFile",                           ATTRIBUTE_SourceFile},
                {L"SourceDebugExtension",                 ATTRIBUTE_SourceDebugExtension},
                {L"LineNumberTable",                      ATTRIBUTE_LineNumberTable},
                {L"LocalVariableTable",                   ATTRIBUTE_LocalVariableTable},
                {L"LocalVariableTypeTable",               ATTRIBUTE_LocalVariableTypeTable},
                {L"Deprecated",                           ATTRIBUTE_Deprecated},
                {L"RuntimeVisibleAnnotations",            ATTRIBUTE_RuntimeVisibleAnnotations},
                {L"RuntimeInvisibleAnnotations",          ATTRIBUTE_RuntimeInvisibleAnnotations},
                {L"RuntimeVisibleParameterAnnotations",   ATTRIBUTE_RuntimeVisibleParameterAnnotations},
                {L"RuntimeInvisibleParameterAnnotations", ATTRIBUTE_RuntimeInvisibleParameterAnnotations},
                {L"RuntimeVisibleTypeAnnotations",        ATTRIBUTE_RuntimeVisibleTypeAnnotations},
                {L"RuntimeInvisibleTypeAnnotations",      ATTRIBUTE_RuntimeInvisibleTypeAnnotations},
                {L"AnnotationDefault",                    ATTRIBUTE_AnnotationDefault},
                {L"BootstrapMethods",                     ATTRIBUTE_BootstrapMethods},
                {L"MethodParameters",                     ATTRIBUTE_MethodParameters},
        };
        cp_info *info = this->constant_pool[index].get();
        if(info->tag == CONSTANT_Utf8){
            auto *utf8_info = static_cast<CONSTANT_Utf8_info *>(info);
            String name = utf8_info->getConstant();
            auto iter = ATTRIBUTE_MAPPING.find(name);
            if(iter != ATTRIBUTE_MAPPING.end()){
                return iter->second;
            }
            return ATTRIBUTE_INVALID;
        }
    }
    void ClassFile::printConstantPool() {
        printf("\n");
        for(u2 i = 1; i < constant_pool_count; i++){
            switch (constant_pool[i]->tag) {
                case CONSTANT_Class:{
                    const CONSTANT_Class_info &target = (CONSTANT_Class_info&)*constant_pool[i];
                    printf("    #%4d = Class %15s #%d\n", i, "", target.name_index);
                    break;
                }
                case CONSTANT_String:{
                    const CONSTANT_String_info &target = (CONSTANT_String_info&)*constant_pool[i];
                    printf("    #%4d = Class %15s #%d\n", i, "", target.string_index);
                    break;
                }
                case CONSTANT_Fieldref:{
                    const CONSTANT_Fieldref_info& target = (CONSTANT_Fieldref_info&)*constant_pool[i];
                    printf("    #%4d = Fieldref %12s #%d.#%d\n", i, "", target.class_index,
                           target.name_and_type_index);
                    break;
                }
                case CONSTANT_Methodref: {
                    const CONSTANT_Methodref_info& target = (CONSTANT_Methodref_info &)*constant_pool[i];
                    printf("    #%4d = Methodref %11s #%d.#%d\n", i, "", target.class_index,
                           target.name_and_type_index);
                    break;
                }
                case CONSTANT_InterfaceMethodref:{
                    const CONSTANT_InterfaceMethodref_info& target = \
                                                             (CONSTANT_InterfaceMethodref_info &)*constant_pool[i];
                    printf("    #%4d = InterfaceMethodref %2s #%d.#%d\n", i, "", target.class_index,
                           target.name_and_type_index);
                    break;
                }
                case CONSTANT_Integer:{
                    const CONSTANT_Integer_info& target = (CONSTANT_Integer_info &) *constant_pool[i];
                    printf("    #%4d = Integer %13s %di\n", i, "", target.getConstant());
                    break;
                }
                case CONSTANT_Float: {
                    const CONSTANT_Float_info& target = (CONSTANT_Float_info &) *constant_pool[i];
                    float result = target.getConstant();
                    if (FLOAT_IS_POSITIVE_INFINITY(result)) {
                        printf("    #%4d = Float %15s Inf\n", i, "");

                    } else if (FLOAT_IS_NEGATIVE_INFINITY(result)) {
                        printf("    #%4d = Float %15s -Inf\n", i, "");

                    } else if (FLOAT_IS_NAN(result)) {
                        printf("    #%4d = Float %15s NaN\n", i, "");

                    } else {
                        printf("    #%4d = Float %15s %f\n", i, "", result);
                    }
                    break;
                }
                case CONSTANT_Long:{
                    const CONSTANT_Long_info& target = (CONSTANT_Long_info &) *constant_pool[i];
                    printf("    #%4d = Long %16s %ldl\n", i, "", target.getConstant());
                    ++i;
                    break;
                }
                case CONSTANT_Double:{
                    const CONSTANT_Double_info& target = (CONSTANT_Double_info &) *constant_pool[i];
                    double result = target.getConstant();
                    if (DOUBLE_IS_POSITIVE_INFINITY(result))
                        printf("    #%4d = Double %14s Inf\n", i, "");
                    else if (DOUBLE_IS_NEGATIVE_INFINITY(result))
                        printf("    #%4d = Double %14s -Inf\n", i, "");
                    else if (DOUBLE_IS_NAN(result)) {
                        printf("    #%4d = Double %14s NaN\n", i, "");
                    } else {
                        printf("    #%4d = Double %14s %lf\n", i, "", result);
                    }
                    ++i;
                    break;
                }
                case CONSTANT_NameAndType:{
                    const CONSTANT_NameAndType_info& target = (CONSTANT_NameAndType_info &) *constant_pool[i];
                    printf("    #%4d = NameAndType %9s #%d.#%d\n", i, "", target.name_index,
                           target.descriptor_index);
                    break;
                }
                case CONSTANT_Utf8:{
                    CONSTANT_Utf8_info& target = (CONSTANT_Utf8_info &) *constant_pool[i];
                    printf("    #%4d = Utf8 %16s ", i, "");
                    printf("%s\n", strings::toStdString(target.getConstant()).c_str());
                    break;
                }
                case CONSTANT_MethodHandle:{
                    CONSTANT_MethodHandle_info& target = (CONSTANT_MethodHandle_info &)*constant_pool[i];
                    const char *ref_kind;
                    switch (target.reference_kind) {
                        case REF_getField:
                            ref_kind = "[getField]";
                            break;
                        case REF_getStatic:
                            ref_kind = "[getStatic]";
                            break;
                        case REF_putField:
                            ref_kind = "[putField]";
                            break;
                        case REF_putStatic:
                            ref_kind = "[putStatic]";
                            break;
                        case REF_invokeVirtual:
                            ref_kind = "[invokeVirtual]";
                            break;
                        case REF_invokeStatic:
                            ref_kind = "[invokeStatic]";
                            break;
                        case REF_invokeSpecial:
                            ref_kind = "[invokeSpecial]";
                            break;
                        case REF_newInvokeSpecial:
                            ref_kind = "[newInvokeSpecial]";
                            break;
                        case REF_invokeInterface:
                            ref_kind = "[invokeInterface]";
                            break;
                        default:
                            printf("Unknown reference kind: %u\n", target.reference_kind);
                            return;
                    }
                    printf("    #%4d = MethodHandle %8s %s:#%d\n", i, "", ref_kind, target.reference_index);
                    break;
                }
                case CONSTANT_MethodType:{
                    CONSTANT_MethodType_info& target = (CONSTANT_MethodType_info&)*constant_pool[i];
                    printf("    #%4d = MethodType %10s #%d\n", i, "", target.descriptor_index);
                    break;
                }
                case CONSTANT_InvokeDynamic:{
                    CONSTANT_InvokeDynamic_info& target = (CONSTANT_InvokeDynamic_info&)*constant_pool[i];
                    printf("    #%4d = InvokeDynamic %7s #%d:#%d\n", i, "", target.bootstrap_method_attr_index,
                           target.name_and_type_index);
                    break;
                }
                default: {
                    printf("Unknown constant entry: %u", constant_pool[i]->tag);
                    return;
                }
            }
        }
    }
}
