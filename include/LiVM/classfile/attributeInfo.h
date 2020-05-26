//
// Created by lzx on 5/8/20.
//

#ifndef LIVM_ATTRIBUTEINFO_H
#define LIVM_ATTRIBUTEINFO_H

#include "../../shared/types.h"
#include "constantPool.h"
#include <shared/hashMap.h>
#define ITEM_Top                0
#define ITEM_Integer            1
#define ITEM_Float              2
#define ITEM_Double             3
#define ITEM_Long               4
#define ITEM_Null               5
#define ITEM_UninitializedThis  6
#define ITEM_Object             7
#define ITEM_Uninitialized      8

namespace LiVM {
    struct attribute_info {
        u2 attribute_name_index;
        u4 attribute_length;
        attribute_info() = default;
        virtual ~attribute_info() = default;
    };
    struct ConstantValue_attribute: public attribute_info {
        u2 constantValue_index;
    };
    struct exception_table_t {
        u2 start_pc;
        u2 end_pc;
        u2 handler_pc;
        u2 catch_type;
    };
    struct Code_attribute: public attribute_info {
        u2 max_stack;
        u2 max_locals;
        u4 code_length;
        std::unique_ptr<u1[]> code;
        u2 exception_table_length;
        std::unique_ptr<exception_table_t[]> exception_table;
        u2 attributes_count;
        unique_arr<attribute_info> attributes;

    };
    struct vertification_type_info {
        u1 tag;
        virtual ~vertification_type_info() = default;
    };
    struct Top_variable_info: public vertification_type_info {
    };
    struct Integer_variable_info: public vertification_type_info{
    };
    struct Float_variable_info: public vertification_type_info {
    };
    struct Null_variable_info: public vertification_type_info {
    };
    struct UninitializedThis_variable_info: public vertification_type_info{
    };
    struct Object_variable_info : public vertification_type_info {
        u2 cpool_index;
    };
    struct Uninitialized_variable_info : public vertification_type_info {
        u2 offset;
    };
    struct Long_variable_info : public vertification_type_info {
    };
    struct Double_variable_info : public vertification_type_info {
    };
    struct stack_map_frame{
        u1 frame_type;
        stack_map_frame() = default;
        virtual ~stack_map_frame() = default;
    };
    struct same_frame: public stack_map_frame {
    };
    struct same_locals_1_stack_item_frame: public stack_map_frame {
        std::unique_ptr<vertification_type_info> stack;
    };
    struct same_locals_1_stack_item_frame_extended: public same_locals_1_stack_item_frame{
        u2 offset_delta;
    };
    struct chop_frame: public stack_map_frame {
        u2 offset_delta;
    };
    struct same_frame_extended : public stack_map_frame{
        u2 offset_delta;
    };
    struct append_frame: public stack_map_frame {
        u2 offset_delta;
        unique_arr<vertification_type_info> locals;
    };
    struct full_frame: public stack_map_frame {
        u2 offset_delta;
        u2 number_of_locals;
        unique_arr<vertification_type_info> locals;
        u2 number_of_stack_items;
        unique_arr<vertification_type_info> stack;
    };
    struct StackMapTable_attribute : public attribute_info{
        u2 number_of_entries;
        unique_arr<stack_map_frame> entries;
    };
    struct Exceptions_attribute : public attribute_info{
        u2 number_of_exceptions;
        std::unique_ptr<u2[]> exception_index_table;
    };
    struct InnerClasses_attribute : public attribute_info{
        u2 number_of_classes;
        struct class_t {
            u2 inner_class_info_index;
            u2 outer_class_info_index;
            u2 inner_name_index;
            u2 inner_classs_access_flags;
        };
        std::unique_ptr<class_t[]> classes;
    };
    struct EnclosingMethod_attribute : public attribute_info{
        u2 class_index;
        u2 method_index;
    };
    struct Synthetic_attribute : public attribute_info {
    };
    struct Signature_attribute : public attribute_info {
        u2 signature_index;
    };
    struct SourceFile_attribute : public attribute_info{
        u2 sourcefile_index;
    };
    struct SourceDebugExtension_attribute : public attribute_info {
        std::unique_ptr<u1[]> debug_extension;
    };
    struct LineNumberTable_attribute : public attribute_info{
        u2 line_number_table_length;
        struct line_number_table_t {
            u2 start_pc;
            u2 line_number;
        };
        std::unique_ptr<line_number_table_t[]>line_number_table;
    };
    struct LocalVariableTable_attribute : public attribute_info{
        u2 local_variable_table_length;
        struct local_variable_table_t{
            u2 start_pc;
            u2 length;
            u2 name_index;
            u2 descriptor_index;
            u2 index;
        };
        std::unique_ptr<local_variable_table_t[]>local_variable_tables;
    };
    struct LocalVariableTypeTable_attribute: public attribute_info {
        u2 local_variable_type_table_length;
        struct local_variable_type_table_t {
            u2 start_pc;
            u2 length;
            u2 name_index;
            u2 signature_index;
            u2 index;
        } ;
        std::unique_ptr<local_variable_type_table_t[]>local_variable_type_table;
    };

    struct Deprecated_attribute: public attribute_info{
    };
    struct value_t {
        virtual ~value_t() = default;
    };
    struct const_value_t: public value_t {
        u2 const_value_index;
    };
    struct enum_const_value_t: public value_t {
        u2 type_name_index;
        u2 const_name_index;
    };
    struct class_info_index_t: public value_t {
        u2 class_info_index;
    };

    struct  element_value {
        u1 tag;
        std::unique_ptr<value_t> value;
    };
    struct element_value_pairs_t {
        u2 element_name_index;
        element_value value;
    };
    struct annotation: public value_t {
        u2 type_index;
        u2 num_element_value_pairs;
        unique_arr<element_value_pairs_t> element_value_pairs;
    };
    struct array_value_t: public value_t {
        u2 num_values;
        unique_arr<element_value> values;
    };
    struct RuntimeVisibleAnnotations_attribute: public attribute_info{
        u2 num_annotations;
        unique_arr<annotation> annotations;
    };
    struct parameter_annotations_t{
        u2 num_annotations;
        unique_arr<annotation> annotations;
    };
    struct RuntimeVisibleParameterAnnotations_attribute: public attribute_info {
        u1 num_parameters;
        std::unique_ptr<parameter_annotations_t[]> parameter_annotations;
    };
    struct RuntimeInvisibleParameterAnnotations_attribute: public attribute_info {
        u1 num_parameters;
        std::unique_ptr<parameter_annotations_t[]> parameter_annotations;
    };
    struct target_info_t {
    };
    struct type_parameter_target: public target_info_t {
        u1 type_parameter_index;
    };
    struct supertype_target: public target_info_t {
        u2 supertype_index;
    };
    struct type_parameter_bound_target: public target_info_t {
        u1 type_parameter_index;
        u1 bound_index;
    };
    struct empty_target: public target_info_t {
    };
    struct formal_parameter_target: public target_info_t {
        u1 formal_parameter_index;
    };
    struct throws_target: public target_info_t {
        u2 throws_type_index;
    };
    struct localvar_target: public target_info_t {
        u2 table_length;
        struct table_t {
            u2 start_pc;
            u2 length;
            u2 index;
        };
        std::unique_ptr<table_t[]> table;
    };
    struct catch_target: public target_info_t {
        u2 exception_table_index;
    };
    struct offset_target: public target_info_t {
        u2 offset;
    };
    struct type_argument_target: public target_info_t {
        u2 offset;
        u1 type_argument_index;
    };
    struct type_path {
        u1 path_length;
        struct path_t {
            u1 type_path_kind;
            u1 type_argument_index;
        } ;
        std::unique_ptr<path_t[]> path;
    };
    struct type_annotation {
        u1 target_type;
        std::unique_ptr<target_info_t> target_info;
        type_path target_path;
        u2 type_index;
        u2 num_element_value_pairs;
        unique_arr<element_value_pairs_t> element_value_pairs;
    };
    struct RuntimeVisibleTypeAnnotations_attribute: public attribute_info {
        u2 num_annotations;
        std::unique_ptr<type_annotation[]> annotations;
    };
    using RuntimeInvisibleTypeAnnotations_attribute = RuntimeVisibleTypeAnnotations_attribute;

    struct AnnotationDefault_attribute: public attribute_info {
        element_value  default_value;
    };
    struct BootstrapMethods_attribute: public attribute_info {
        u2 num_bootstrap_methods;
        struct bootstrap_method_t{
            u2 bootstrap_method_ref;
            u2 num_bootstrap_arguments;
            std::unique_ptr<u2[]>bootstrap_arguments;
        } ;
        std::unique_ptr<bootstrap_method_t[]> bootstrap_methods;
    };
    struct MethodParameters_attribute : public attribute_info {
        u1 parameters_count;
        struct parameter_t {
            u2 name_index;
            u2 access_flags;
        };
        std::unique_ptr<parameter_t[]> parameters;
    };


}
#endif //LIVM_ATTRIBUTEINFO_H
