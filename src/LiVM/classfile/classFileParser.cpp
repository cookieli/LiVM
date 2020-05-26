//
// Created by lzx on 5/10/20.
//
#include <fstream>
#include <LiVM/classfile/classfileParser.h>
#include <iostream>
namespace LiVM {
    using namespace std;
    ClassfileParser ::ClassfileParser(const string &classFilepath) {
        fstream file (classFilepath, ios::in|ios::binary|ios::ate);
        if (file.is_open())
        {
            _size = file.tellg();
            _content = make_unique<u1[]>(_size);
            _classFileStream = make_unique<ClassFileStream>();
            file.seekg (0, ios::beg);
            auto p = _content.get();
            file.read (reinterpret_cast<char*>(p), _size);
            file.close();
            _classFileStream->init(p, _size);
        }
    }

    unique_ptr<ClassFile> ClassfileParser::parse() {
        auto classFile = make_unique<ClassFile>();
        classFile->magic = _classFileStream->get_u4();
        if(classFile->magic != 0xCAFEBABE){
            return nullptr;
        }
        classFile->minor_version = _classFileStream->get_u2();
        classFile->major_version = _classFileStream->get_u2();
        parseConstantPool(*classFile);
        classFile->access_flags    = _classFileStream->get_u2();
        classFile->this_class      = _classFileStream->get_u2();
        classFile->super_class     = _classFileStream->get_u2();
        parseInterfaces(*classFile);
        parseFields(*classFile);
        parseMethods(*classFile);
        classFile->attributes_count = _classFileStream->get_u2();
        classFile->attributes = make_unique<unique_ptr<attribute_info>[]>(classFile->attributes_count);
        for(u2 i =0; i < classFile->attributes_count; i++){
            classFile->attributes[i] = parseAttributeInfo(*classFile);
        }
        return classFile;
    }

    template<typename T>
    T ClassfileParser::getInfo(ClassFile &cf) {
        T ret;
        ret.access_flags     = _classFileStream->get_u2();
        ret.name_index       = _classFileStream->get_u2();
        ret.descriptor_index = _classFileStream->get_u2();
        ret.attributes_count = _classFileStream->get_u2();
        ret.attributes       = make_unique<unique_ptr<attribute_info>[]>(ret.attributes_count);
        for(u2 i = 0; i < ret.attributes_count; i++){
            ret.attributes[i] = parseAttributeInfo(cf);
        }
        return ret;
    }
    void ClassfileParser::parseFields(ClassFile &cf) {
        cf.fields_count = _classFileStream->get_u2();
        cf.fields = make_unique<field_info[]>(cf.fields_count);
        for(u2 i = 0; i < cf.fields_count; i++){
            cf.fields[i] = getFieldInfo(cf);
        }
    }
    field_info ClassfileParser::getFieldInfo(ClassFile& cf) {
        return getInfo<field_info>(cf);
    }
    void ClassfileParser::parseMethods(ClassFile &cf) {
        cf.methods_count = _classFileStream->get_u2();
        cf.methods = make_unique<method_info[]>(cf.methods_count);
        for(u2 i = 0; i < cf.methods_count; i++){
            cf.methods[i] = getMethodInfo(cf);
        }
    }
    method_info ClassfileParser::getMethodInfo(ClassFile &cf) {
        return getInfo<method_info>(cf);
    }
    void ClassfileParser::parseConstantPool(ClassFile& cf) {
        u2 count = cf.constant_pool_count = _classFileStream->get_u2();
        cf.constant_pool = std::move(make_unique<unique_ptr<cp_info>[]>(count));
        auto pool = std::move(cf.constant_pool);
        for(u2 i = 1; i < count; i++){
//            cout << i << endl;
            u1 tag = _classFileStream->peek_u1();
            switch (tag) {
                case CONSTANT_Utf8:
                    pool = readPoolEntry<CONSTANT_Utf8_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_Integer:
                    pool = readPoolEntry<CONSTANT_Integer_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_Float:
                    pool = readPoolEntry<CONSTANT_Float_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_Class:
                    pool = readPoolEntry<CONSTANT_Class_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_Double:
                    pool = readPoolEntry<CONSTANT_Double_info>(std::move(pool), i, *_classFileStream);
                    ++i;
                    break;
                case CONSTANT_Long:
                    pool = readPoolEntry<CONSTANT_Long_info>(std::move(pool), i, *_classFileStream);
                    ++i;
                    break;
                case CONSTANT_Fieldref:
                    pool = readPoolEntry<CONSTANT_Fieldref_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_Methodref:
                    pool = readPoolEntry<CONSTANT_Methodref_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_String:
                    pool = readPoolEntry<CONSTANT_String_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_InterfaceMethodref:
                    pool = readPoolEntry<CONSTANT_InterfaceMethodref_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_InvokeDynamic:
                    pool = readPoolEntry<CONSTANT_InvokeDynamic_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_MethodHandle:
                    pool = readPoolEntry<CONSTANT_MethodHandle_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_MethodType:
                    pool = readPoolEntry<CONSTANT_MethodType_info>(std::move(pool), i, *_classFileStream);
                    break;
                case CONSTANT_NameAndType:
                    pool = readPoolEntry<CONSTANT_NameAndType_info>(std::move(pool), i, *_classFileStream);
                    break;
                default:
                    PANIC("read unexpected Constant");
            }
        }
        cf.constant_pool = std::move(pool);
    }
    void ClassfileParser::parseInterfaces(ClassFile& cf) {
        auto count = cf.interface_count = _classFileStream->get_u2();
        cf.interfaces = make_unique<u2[]>(count);
        for(int i = 0; i < count; i++){
            cf.interfaces[i] = _classFileStream->get_u2();
        }
    }

    template<typename T>
    static std::unique_ptr<T> readAttributeEntry(ClassFileStream& stream){
        auto ret = make_unique<T>();
        stream >> *ret;
        return ret;
    }
    std::unique_ptr<Code_attribute> ClassfileParser::parseCodeAttribute(ClassFile& cf) {
        auto ret = make_unique<Code_attribute>();
        _classFileStream->into(*ret);
        ret->max_stack = _classFileStream->get_u2();
        ret->max_locals = _classFileStream->get_u2();
        ret->code_length = _classFileStream->get_u4();
        ret->code = make_unique<u1[]>(ret->code_length);
        for(u4 i = 0; i< ret->code_length; i++){
            ret->code[i] = _classFileStream->get_u1();
        }
        ret->exception_table_length = _classFileStream->get_u2();
        ret->exception_table = make_unique<exception_table_t[]>(ret->exception_table_length);
        for(u2 i= 0; i < ret->exception_table_length; i++){
            _classFileStream->into(ret->exception_table[i]);
        }
        ret->attributes_count = _classFileStream->get_u2();
        ret->attributes = make_unique<unique_ptr<attribute_info>[]>(ret->attributes_count);
        for(u2 i = 0; i < ret->attributes_count; i++){
            ret->attributes[i]  = parseAttributeInfo(cf);
        }
        return ret;
    }
    std::unique_ptr<StackMapTable_attribute> ClassfileParser::parseStackMapTableAttribute() {
        auto ret = make_unique<StackMapTable_attribute>();
        _classFileStream->into(*ret);
        ret->number_of_entries = _classFileStream->get_u2();
        ret->entries = make_unique<unique_ptr<stack_map_frame>[]>(ret->number_of_entries);
        for(int i = 0; i < ret->number_of_entries; i++){
            ret->entries[i] = parseStackMapFrame();
        }
        return ret;
    }
    std::unique_ptr<Exceptions_attribute> ClassfileParser::parseExceptionsAttribute() {
        auto ret = make_unique<Exceptions_attribute>();
        _classFileStream->into(*ret);
        ret->number_of_exceptions = _classFileStream->get_u2();
        ret->exception_index_table = make_unique<u2[]>(ret->number_of_exceptions);
        for(int i = 0; i < ret->number_of_exceptions; i++){
            ret->exception_index_table[i] = _classFileStream->get_u2();
        }
        return ret;
    }
    std::unique_ptr<InnerClasses_attribute> ClassfileParser::parseInnerClassesAttribute() {
        auto ret = make_unique<InnerClasses_attribute>();
        _classFileStream->into(*ret);
        ret->number_of_classes = _classFileStream->get_u2();
        ret->classes =
                make_unique<InnerClasses_attribute::class_t[]>(ret->number_of_classes);
        for(int i = 0; i < ret->number_of_classes; i++){
            ret->classes[i].inner_class_info_index    = _classFileStream->get_u2();
            ret->classes[i].outer_class_info_index    = _classFileStream->get_u2();
            ret->classes[i].inner_name_index          = _classFileStream->get_u2();
            ret->classes[i].inner_classs_access_flags = _classFileStream->get_u2();
        }
        return ret;
    }
    std::unique_ptr<EnclosingMethod_attribute> ClassfileParser::parseEnclosingMethod() {
        auto ret = make_unique<EnclosingMethod_attribute>();
        _classFileStream->into(*ret);
        ret->class_index = _classFileStream->get_u2();
        ret->method_index = _classFileStream->get_u2();
        return ret;
    }
    std::unique_ptr<Synthetic_attribute> ClassfileParser::parseSyntheticAttribute() {
        auto ret = make_unique<Synthetic_attribute>();
        _classFileStream->into(*ret);
        return ret;
    }
    std::unique_ptr<Signature_attribute> ClassfileParser::parseSignatureAttribute() {
        auto ret = make_unique<Signature_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->signature_index = _classFileStream->get_u2();
        return ret;
    }
    std::unique_ptr<SourceFile_attribute> ClassfileParser::parseSourceFileAttribute() {
        auto ret = make_unique<SourceFile_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->sourcefile_index = _classFileStream->get_u2();
        return ret;
    }
    std::unique_ptr<SourceDebugExtension_attribute> ClassfileParser::parseSourceDebugExtensionAttribute() {
        auto ret = make_unique<SourceDebugExtension_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->debug_extension = make_unique<u1[]>(ret->attribute_length);
        for(int i = 0; i < ret->attribute_length; i++){
            ret->debug_extension[i] = _classFileStream->get_u1();
        }
        return ret;
    }
    std::unique_ptr<LineNumberTable_attribute> ClassfileParser::parseLineNumberTableAttribute() {
        auto ret = make_unique<LineNumberTable_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->line_number_table_length = _classFileStream->get_u2();
        ret->line_number_table =
                make_unique<LineNumberTable_attribute::line_number_table_t[]>(ret->line_number_table_length);
        for(int i = 0; i < ret->line_number_table_length; i++){
            ret->line_number_table[i].start_pc    = _classFileStream->get_u2();
            ret->line_number_table[i].line_number = _classFileStream->get_u2();
        }
        return ret;
    }
    std::unique_ptr<LocalVariableTable_attribute> ClassfileParser::parseLocalVariableTableAttribute() {
        auto ret = make_unique<LocalVariableTable_attribute>();
        _classFileStream->into((attribute_info&) *ret);
        ret->local_variable_table_length = _classFileStream->get_u2();
        ret->local_variable_tables =
                make_unique<LocalVariableTable_attribute::local_variable_table_t[]>\
                                (ret->local_variable_table_length);
        for(int i = 0; i < ret->local_variable_table_length; i++){
            ret->local_variable_tables[i].start_pc         = _classFileStream->get_u2();
            ret->local_variable_tables[i].length         = _classFileStream->get_u2();
            ret->local_variable_tables[i].name_index       = _classFileStream->get_u2();
            ret->local_variable_tables[i].descriptor_index = _classFileStream->get_u2();
            ret->local_variable_tables[i].index            =  _classFileStream->get_u2();
        }
        return ret;
    }
    std::unique_ptr<LocalVariableTypeTable_attribute> ClassfileParser::parseLocalVariableTypeTableAttribute() {
        auto ret = make_unique<LocalVariableTypeTable_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->local_variable_type_table_length = _classFileStream->get_u2();
        ret->local_variable_type_table = make_unique<LocalVariableTypeTable_attribute::\
                                                 local_variable_type_table_t[]>\
                                                 (ret->local_variable_type_table_length);
        for(int i = 0; i < ret->local_variable_type_table_length; i++){
            ret->local_variable_type_table[i].start_pc        = _classFileStream->get_u2();
            ret->local_variable_type_table[i].length          = _classFileStream->get_u2();
            ret->local_variable_type_table[i].name_index      = _classFileStream->get_u2();
            ret->local_variable_type_table[i].signature_index = _classFileStream->get_u2();
            ret->local_variable_type_table[i].index           = _classFileStream->get_u2();
        }
        return ret;
    }
    std::unique_ptr<RuntimeVisibleAnnotations_attribute> ClassfileParser::parseRuntimeVisibleAnnotationsAttribute() {
        auto ret = make_unique<RuntimeVisibleAnnotations_attribute>();
        _classFileStream->into((attribute_info&)ret);
        ret->num_annotations = _classFileStream->get_u2();
        ret->annotations = make_unique<unique_ptr<annotation>[]>(ret->num_annotations);
        for(int i = 0; i < ret->num_annotations; i++){
            ret->annotations[i] = parseAnnotation();
        }
        return ret;
    }
    std::unique_ptr<RuntimeVisibleParameterAnnotations_attribute>
            ClassfileParser::parseRuntimeVisibleParameterAnnotationsAttribute() {
        auto ret = make_unique\
                               <RuntimeVisibleParameterAnnotations_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->num_parameters = _classFileStream->get_u1();
        ret->parameter_annotations = make_unique<parameter_annotations_t[]>(ret->num_parameters);
        for(int i = 0; i < ret->num_parameters; i++){
            ret->parameter_annotations[i] = parseParameterAnnotations();
        }
        return ret;
    }
    std::unique_ptr<RuntimeInvisibleParameterAnnotations_attribute>
            ClassfileParser::parseRuntimeInvisibleParameterAnnotationsAttribute() {
        auto ret = make_unique<RuntimeInvisibleParameterAnnotations_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->num_parameters = _classFileStream->get_u1();
        for(u1 i = 0; i < ret->num_parameters; i++){
            ret->parameter_annotations[i] = parseParameterAnnotations();
        }
        return ret;
    }
    element_value ClassfileParser::parseElementValue() {
        element_value e;
        e.tag = _classFileStream->get_u1();
        switch ((char)e.tag) {
            case 'B':
            case 'C':
            case 'D':
            case 'F':
            case 'I':
            case 'J':
            case 'S':
            case 'Z':
            case 's':{
                e.value = [this]{unique_ptr<const_value_t> ret;
                    ret->const_value_index = _classFileStream->get_u2();
                    return ret;}();
                break;
            }
            case 'e':{
                e.value = [this]{unique_ptr<enum_const_value_t> ret;
                    ret->type_name_index = _classFileStream->get_u2();
                    ret->const_name_index = _classFileStream->get_u2();
                    return ret;}();
                break;
            }
            case 'c':{
                e.value = [this]{unique_ptr<class_info_index_t> ret;
                    ret->class_info_index = _classFileStream->get_u2();
                    return ret;
                }();
                break;
            }
            case '@':{
                e.value = parseAnnotation();
                break;
            }
            case '[':{
                e.value = [this]{
                    unique_ptr<array_value_t> ret;
                    ret->num_values = _classFileStream->get_u2();
                    for(int i = 0; i < ret->num_values; i++){
                        ret->values[i] = make_unique<element_value>(parseElementValue());
                    }
                    return ret;
                }();
            }
        }
        return e;
    }
    std::unique_ptr<element_value_pairs_t> ClassfileParser::parseElementValuePairs_t() {
        auto ret = make_unique<element_value_pairs_t>();
        ret->element_name_index = _classFileStream->get_u2();
        ret->value = parseElementValue();
        return ret;
    }
    std::unique_ptr<annotation> ClassfileParser::parseAnnotation() {
        auto ret = make_unique<annotation>();
        ret->type_index              = _classFileStream->get_u2();
        ret->num_element_value_pairs = _classFileStream->get_u2();
        ret->element_value_pairs = make_unique<unique_ptr<element_value_pairs_t>[]>\
                                                              (ret->num_element_value_pairs);
        for(u2 i = 0; i < ret->num_element_value_pairs; i++){
            ret->element_value_pairs[i] = parseElementValuePairs_t();
        }
        return ret;
    }
    parameter_annotations_t ClassfileParser::parseParameterAnnotations() {
        parameter_annotations_t pa;
        pa.num_annotations = _classFileStream->get_u2();
        for(u2 i = 0; i <pa.num_annotations; i++){
            pa.annotations[i] = parseAnnotation();
        }
        return pa;
    }

    template<typename  T>
    std::unique_ptr<T> ClassfileParser::parseRuntimeTypeAnnotationsAttribute() {
        auto ret = make_unique<T>();
        _classFileStream->into((attribute_info&)*ret);
        ret->num_annotations = _classFileStream->get_u2();
        ret->annotations =make_unique<type_annotation[]>(ret->num_annotations);
        for(u2 i = 0; i < ret->num_annotations; i++){
            ret->annotations[i] = parseTypeAnnotation();
        }
        return ret;
    }
    std::unique_ptr<AnnotationDefault_attribute> ClassfileParser::parseAnnotationDefaultAttribute() {
        auto ret = make_unique<AnnotationDefault_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->default_value = parseElementValue();
        return ret;
    }
    std::unique_ptr<BootstrapMethods_attribute> ClassfileParser::parseBootstrapMethodsAttribute() {
        auto ret = make_unique<BootstrapMethods_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->num_bootstrap_methods = _classFileStream->get_u2();
        ret->bootstrap_methods = make_unique<BootstrapMethods_attribute::bootstrap_method_t[]>\
                                                                         (ret->num_bootstrap_methods);
        for(u2 i =0; i < ret->num_bootstrap_methods; i++){
            ret->bootstrap_methods[i] = [this]{
                BootstrapMethods_attribute::bootstrap_method_t ret;
                ret.bootstrap_method_ref = _classFileStream->get_u2();
                ret.num_bootstrap_arguments = _classFileStream->get_u2();
                ret.bootstrap_arguments = make_unique<u2[]>(ret.num_bootstrap_arguments);
                for(u2 i = 0; i < ret.num_bootstrap_arguments; i++){
                    ret.bootstrap_arguments[i] = _classFileStream->get_u2();
                }
                return ret;
            }();
        }
        return ret;
    }
    std::unique_ptr<MethodParameters_attribute> ClassfileParser::parseMethodParametersAttribute() {
        auto ret = make_unique<MethodParameters_attribute>();
        _classFileStream->into((attribute_info&)*ret);
        ret->parameters_count = _classFileStream->get_u1();
        ret->parameters = make_unique<MethodParameters_attribute::parameter_t[]>(ret->parameters_count);
        for(u1 i = 0; i < ret->parameters_count; i++){
            ret->parameters[i].name_index   = _classFileStream->get_u2();
            ret->parameters[i].access_flags = _classFileStream->get_u2();
        }
        return ret;
    }
    type_annotation ClassfileParser::parseTypeAnnotation() {
        type_annotation ret;
        ret.target_type = _classFileStream->get_u1();
        switch (ret.target_type) {
            case 0x00:
            case 0x01:
                ret.target_info = [this]{
                    auto ret = make_unique<type_parameter_target>();
                    ret->type_parameter_index = _classFileStream->get_u1();
                    return ret;
                     }();
                break;
            case 0x10:
                ret.target_info = [this]{
                    auto ret = make_unique<supertype_target>();
                    ret->supertype_index = _classFileStream->get_u2();
                    return ret;
                }();
                break;
            case 0x11:
            case 0x12:
                ret.target_info = [this]{
                    auto ret = make_unique<type_parameter_bound_target>();
                    ret->type_parameter_index = _classFileStream->get_u1();
                    ret->bound_index          = _classFileStream->get_u1();
                    return ret;
                }();
                break;
            case 0x13:
            case 0x14:
            case 0x15:
                ret.target_info = [this]{
                    return make_unique<empty_target>();
                }();
                break;
            case 0x16:
                ret.target_info = [this]{
                    auto ret = make_unique<formal_parameter_target>();
                    ret->formal_parameter_index = _classFileStream->get_u1();
                    return ret;
                }();
                break;
            case 0x17:
                ret.target_info = [this]{
                    auto ret = make_unique<throws_target>();
                    ret->throws_type_index = _classFileStream->get_u2();
                    return ret;
                }();
                break;
            case 0x40:
            case 0x41:
                ret.target_info = [this]{
                    auto ret = make_unique<localvar_target>();
                    ret->table_length = _classFileStream->get_u2();
                    ret->table = make_unique<localvar_target::table_t[]>(ret->table_length);
                    for(int i = 0; i < ret->table_length; i++){
                        ret->table[i].start_pc = _classFileStream->get_u2();
                        ret->table[i].length   = _classFileStream->get_u2();
                        ret->table[i].index    = _classFileStream->get_u2();
                    }
                    return ret;
                }();
                break;
            case 0x42:
                ret.target_info = [this]{
                    auto ret = make_unique<catch_target>();
                    ret->exception_table_index = _classFileStream->get_u2();
                    return ret;
                }();
                break;
            case 0x43:
            case 0x44:
            case 0x45:
            case 0x46:
                ret.target_info = [this]{
                    auto ret = make_unique<offset_target>();
                    ret->offset = _classFileStream->get_u2();
                    return ret;
                }();
                break;
            case 0x47:
            case 0x48:
            case 0x49:
            case 0x4A:
            case 0x4B:
                ret.target_info = [this]{
                    auto ret = make_unique<type_argument_target>();
                    ret->offset = _classFileStream->get_u2();
                    ret->type_argument_index = _classFileStream->get_u1();
                    return ret;
                }();
                break;
            default:
                assert(false);
        }
        ret.target_path = [this]{
            type_path ret;
            ret.path_length = _classFileStream->get_u1();
            ret.path = make_unique<type_path::path_t[]>(ret.path_length);
            for(int i = 0; i < ret.path_length; i++){
                ret.path[i].type_path_kind      = _classFileStream->get_u1();
                ret.path[i].type_argument_index = _classFileStream->get_u1();
            }
            return ret;
        }();
        ret.type_index              = _classFileStream->get_u2();
        ret.num_element_value_pairs = _classFileStream->get_u2();
        ret.element_value_pairs     =
                make_unique<unique_ptr<element_value_pairs_t>[]>(ret.num_element_value_pairs);
        for(u2 i = 0; i < ret.num_element_value_pairs; i++){
            ret.element_value_pairs[i] = parseElementValuePairs_t();
        }
        return ret;

    }

    std::unique_ptr<vertification_type_info> ClassfileParser::parseVertificationTypeInfo() {
        u1 tag = _classFileStream->get_u1();
        switch (tag) {
            case ITEM_Object: {
                auto ret = make_unique<Object_variable_info>();
                ret->tag = tag;
                ret->cpool_index = _classFileStream->get_u2();
                return ret;
            }
            case ITEM_Uninitialized: {
                auto ret = make_unique<Uninitialized_variable_info>();
                ret->tag = tag;
                ret->offset = _classFileStream->get_u2();
                return ret;
            }
            default:
                auto ret = make_unique<vertification_type_info>();
                ret->tag = tag;
                return ret;
        }
    }
    std::unique_ptr<same_frame> parseSameFrame(u1 frame_type){
        auto ret = make_unique<same_frame>();
        ret->frame_type = frame_type;
        return ret;
    }
    std::unique_ptr<same_locals_1_stack_item_frame> ClassfileParser::parseSameLocals1StackItemFrame(u1 frame_type){
        auto ret = make_unique<same_locals_1_stack_item_frame>();
        ret->frame_type = frame_type;
        ret->stack = parseVertificationTypeInfo();
        return ret;
    }
    std::unique_ptr<same_locals_1_stack_item_frame_extended>
            ClassfileParser::parseSameLocals1StackItemFrameExtended(u1 frame_type) {
        auto ret = make_unique<same_locals_1_stack_item_frame_extended>();
        ret->frame_type = frame_type;
        ret->offset_delta = _classFileStream->get_u2();
        ret->stack = parseVertificationTypeInfo();
        return ret;
    }
    std::unique_ptr<chop_frame> ClassfileParser::parseChopFrame(u1 frame_type) {
        auto ret = make_unique<chop_frame>();
        ret->frame_type = frame_type;
        ret->offset_delta = _classFileStream->get_u2();
        return ret;
    }
    std::unique_ptr<same_frame_extended> ClassfileParser::parseSameFrameExtended(u1 frame_type) {
        auto ret = make_unique<same_frame_extended>();
        ret->frame_type = frame_type;
        ret->offset_delta = _classFileStream->get_u2();
        return ret;
    }
    std::unique_ptr<append_frame> ClassfileParser::parseAppendFrame(u1 frame_type) {
        auto ret = make_unique<append_frame>();
        ret->frame_type = frame_type;
        ret->offset_delta = _classFileStream->get_u2();
        ret->locals = make_unique<unique_ptr<vertification_type_info>[]>(frame_type - 251);
        for(u2 i = 0; i < frame_type - 251; i++){
            ret->locals[i] = parseVertificationTypeInfo();
        }
        return ret;
    }
    std::unique_ptr<full_frame> ClassfileParser::parseFullFrame(u1 frame_type) {
        auto ret = make_unique<full_frame>();
        ret->frame_type = frame_type;
        ret->offset_delta = _classFileStream->get_u2();
        ret->number_of_locals = _classFileStream->get_u2();
        ret->locals = make_unique<unique_ptr<vertification_type_info>[]>(ret->number_of_locals);
        for(int i = 0; i < ret->number_of_locals; i++){
            ret->locals[i] = parseVertificationTypeInfo();
        }
        ret->number_of_stack_items = _classFileStream->get_u2();
        ret->stack = make_unique<unique_ptr<vertification_type_info>[]>(ret->number_of_stack_items);
        for(int i = 0; i < ret->number_of_stack_items; i++){
            ret->stack[i] = parseVertificationTypeInfo();
        }
        return ret;
    }
    std::unique_ptr<stack_map_frame>
            ClassfileParser::parseStackMapFrame() {
        u1 frame_type = _classFileStream->get_u1();
        if(frame_type >= 0 && frame_type <= 63){//same frame
            return parseSameFrame(frame_type);
        } else if(frame_type >= 64 && frame_type <= 127){//same_locals_1_stack_item_frame
            return parseSameLocals1StackItemFrame(frame_type);
        } else if(frame_type == 247){//same_locals_1_stack_item_frame_extended
            return parseSameLocals1StackItemFrameExtended(frame_type);
        } else if(frame_type >= 248 && frame_type <=250){
           return parseChopFrame(frame_type);
        } else if(frame_type == 251){
            return parseSameFrameExtended(frame_type);
        } else if(frame_type >= 252 && frame_type <= 254){
            return parseAppendFrame(frame_type);
        } else if(frame_type == 255){
            return parseFullFrame(frame_type);
        } else {
            PANIC("Unexpected frame type");
        }
    }
    unique_ptr <attribute_info> ClassfileParser::parseAttributeInfo(ClassFile& cf) {
        u2 attribute_name_index = _classFileStream->peek_u2();
//        u2 attribute_length = _classFileStream->get_u2();
        u2 tag = cf.getAttributeTag(attribute_name_index);
        switch (tag) {
            case ATTRIBUTE_ConstantValue:                      return readAttributeEntry<ConstantValue_attribute>\
                                                                                   (*_classFileStream);
            case ATTRIBUTE_Code:                               return parseCodeAttribute(cf);
            case ATTRIBUTE_StackMapTable:                      return parseStackMapTableAttribute();
            case ATTRIBUTE_Exceptions:                         return parseExceptionsAttribute();
            case ATTRIBUTE_InnerClasses:                       return parseInnerClassesAttribute();
            case ATTRIBUTE_EnclosingMethod:                    return parseEnclosingMethod();
            case ATTRIBUTE_Synthetic:                          return parseSyntheticAttribute();
            case ATTRIBUTE_Signature:                          return parseSignatureAttribute();
            case ATTRIBUTE_SourceFile:                         return parseSourceFileAttribute();
            case ATTRIBUTE_SourceDebugExtension:               return parseSourceDebugExtensionAttribute();
            case ATTRIBUTE_LineNumberTable:                    return parseLineNumberTableAttribute();
            case ATTRIBUTE_LocalVariableTable:                 return parseLocalVariableTableAttribute();
            case ATTRIBUTE_LocalVariableTypeTable:             return parseLocalVariableTypeTableAttribute();
            case ATTRIBUTE_RuntimeVisibleAnnotations:          return parseRuntimeVisibleAnnotationsAttribute();
            case ATTRIBUTE_RuntimeVisibleParameterAnnotations:
                return parseRuntimeVisibleParameterAnnotationsAttribute();
            case ATTRIBUTE_RuntimeInvisibleParameterAnnotations:{
                return parseRuntimeInvisibleParameterAnnotationsAttribute();
            }
            case ATTRIBUTE_RuntimeVisibleTypeAnnotations:{
                return parseRuntimeTypeAnnotationsAttribute<RuntimeVisibleTypeAnnotations_attribute>();
            }
            case ATTRIBUTE_RuntimeInvisibleTypeAnnotations:{
                return parseRuntimeTypeAnnotationsAttribute<RuntimeInvisibleTypeAnnotations_attribute>();
            }
            case ATTRIBUTE_AnnotationDefault:{
                return parseAnnotationDefaultAttribute();
            }
            case ATTRIBUTE_BootstrapMethods:{
                return parseBootstrapMethodsAttribute();
            }
            case ATTRIBUTE_MethodParameters:{
                return parseMethodParametersAttribute();
            }
        }
    }

}
