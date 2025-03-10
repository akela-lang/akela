#include <string.h>
#include <zinc/error_unit_test.h>

#include "zinc/unit_test.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"
#include "centipede/comp_table.h"
#include "centipede/comp_unit.h"

void test_parse_element()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test_suite {\n"
        "    properties {\n"
        "        name: String `required`\n"
        "        solo: Bool\n"
        "        mute: Bool\n"
        "    }\n"
        "    children {\n"
        "        Test\n"
        "    }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    Zinc_expect_error_count(errors, 1);
    Zinc_expect_source_error(errors, "unknown type: Test");

    /* root */
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* element */
    Cent_ast* element = Cent_ast_get(root, 0);
    Zinc_assert_ptr(element, "ptr element");
    Zinc_expect_int_equal(element->type, Cent_ast_type_element_type, "type element");
    Zinc_expect_string(&element->text, "Test_suite", "value element");

    /* properties */
    Cent_ast* prop = Cent_ast_get(element, 0);
    Zinc_assert_ptr(prop, "ptr prop");
    Zinc_expect_int_equal(prop->type, Cent_ast_type_prop, "type prop");

    /* name */
    Cent_ast* dec0 = Cent_ast_get(prop, 0);
    Zinc_assert_ptr(dec0, "ptr dec0");
    Zinc_expect_int_equal(dec0->type, Cent_ast_type_prop_dec, "type dec0");

    Cent_ast* name0 = Cent_ast_get(dec0, 0);
    Zinc_assert_ptr(name0, "ptr name0");
    Zinc_expect_int_equal(name0->type, Cent_ast_type_id, "type name0");
    Zinc_expect_string(&name0->text, "name", "value name0");

    Cent_ast* type0 = Cent_ast_get(dec0, 1);
    Zinc_assert_ptr(type0, "ptr type0");
    Zinc_expect_int_equal(type0->type, Cent_ast_type_id, "type type0");
    Zinc_expect_string(&type0->text, "String", "value type0");

    Cent_ast* mod0 = Cent_ast_get(dec0, 2);
    Zinc_assert_ptr(mod0, "ptr mod0");
    Zinc_expect_int_equal(mod0->type, Cent_ast_type_modifier, "type mod0");
    Zinc_expect_string(&mod0->text, "required", "value mod0");

    /* solo */
    Cent_ast* dec1 = Cent_ast_get(prop, 1);
    Zinc_assert_ptr(dec1, "ptr dec1");
    Zinc_expect_int_equal(dec1->type, Cent_ast_type_prop_dec, "type dec1");

    Cent_ast* name1 = Cent_ast_get(dec1, 0);
    Zinc_assert_ptr(name1, "ptr name1");
    Zinc_expect_int_equal(name1->type, Cent_ast_type_id, "type name1");
    Zinc_expect_string(&name1->text, "solo", "value name1");

    Cent_ast* type1 = Cent_ast_get(dec1, 1);
    Zinc_assert_ptr(type1, "ptr type1");
    Zinc_expect_int_equal(type1->type, Cent_ast_type_id, "type type1");
    Zinc_expect_string(&type1->text, "Bool", "value type1");

    /* mute */
    Cent_ast* dec2 = Cent_ast_get(prop, 2);
    Zinc_assert_ptr(dec2, "ptr dec2");
    Zinc_expect_int_equal(dec2->type, Cent_ast_type_prop_dec, "type dec2");

    Cent_ast* name2 = Cent_ast_get(dec2, 0);
    Zinc_assert_ptr(name2, "ptr name2");
    Zinc_expect_int_equal(name2->type, Cent_ast_type_id, "type name2");
    Zinc_expect_string(&name2->text, "mute", "value name2");

    Cent_ast* type2 = Cent_ast_get(dec2, 1);
    Zinc_assert_ptr(type2, "ptr type2");
    Zinc_expect_int_equal(type2->type, Cent_ast_type_id, "type type2");
    Zinc_expect_string(&type2->text, "Bool", "value type2");

    /* children */
    Cent_ast* children = Cent_ast_get(element, 1);
    Zinc_assert_ptr(children, "ptr children");
    Zinc_expect_int_equal(children->type, Cent_ast_type_children, "type children");

    /* Test */
    Cent_ast* test = Cent_ast_get(children, 0);
    Zinc_assert_ptr(test, "ptr test");
    Zinc_expect_int_equal(test->type, Cent_ast_type_id, "type test");
    Zinc_expect_string(&test->text, "Test", "value test");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_enumerate()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
    "enum Symbol_type {\n"
    "    Variable\n"
    "    Type\n"
    "    Info\n"
    "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    /* root */
    Zinc_expect_no_errors(errors);
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* enumerate */
    Cent_ast* enumerate = Cent_ast_get(root, 0);
    Zinc_assert_ptr(enumerate, "ptr enumerate");
    Zinc_expect_int_equal(enumerate->type, Cent_ast_type_enum_type, "type enumerate");
    Zinc_expect_string(&enumerate->text, "Symbol_type", "value enumerate");

    /* Variable */
    Cent_ast* value0 = Cent_ast_get(enumerate, 0);
    Zinc_assert_ptr(value0, "ptr value0");
    Zinc_expect_int_equal(value0->type, Cent_ast_type_id, "type value0");
    Zinc_expect_string(&value0->text, "Variable", "value value0");

    /* Type */
    Cent_ast* value1 = Cent_ast_get(enumerate, 1);
    Zinc_assert_ptr(value1, "ptr value1");
    Zinc_expect_int_equal(value1->type, Cent_ast_type_id, "type value1");
    Zinc_expect_string(&value1->text, "Type", "value value1");

    /* Info */
    Cent_ast* value2 = Cent_ast_get(enumerate, 2);
    Zinc_assert_ptr(value2, "ptr value2");
    Zinc_expect_int_equal(value2->type, Cent_ast_type_id, "type value2");
    Zinc_expect_string(&value2->text, "Info", "value value2");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_top_level_assignment()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Type_def_type {\n"
        "    Integer\n"
        "    Float\n"
        "    String\n"
        "    Boolean\n"
        "}\n"
        "# built-in element defs\n"
        "const i32 = Type_def {\n"
        "    .type = Type_def_type::Integer\n"
        "    .name = \"i32\"\n"
        "    .bit_count = 32\n"
        "    .is_signed = true\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    /* root */
    Zinc_expect_no_errors(errors);
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* enum type */
    Cent_ast* enum_type = Cent_ast_get(root, 0);
    Zinc_assert_ptr(enum_type, "ptr enum_type");
    Zinc_expect_int_equal(enum_type->type, Cent_ast_type_enum_type, "type enum_type");
    Zinc_expect_string(&enum_type->text, "Type_def_type", "text enum_type");

    /* const */
    Cent_ast* const_ = Cent_ast_get(root, 1);
    Zinc_assert_ptr(const_, "ptr const_");
    Zinc_expect_int_equal(const_->type, Cent_ast_type_const, "type const_");

    /* name */
    Cent_ast* name = Cent_ast_get(const_, 0);
    Zinc_assert_ptr(name, "ptr name");
    Zinc_expect_int_equal(name->type, Cent_ast_type_id, "type name");
    Zinc_expect_string(&name->text, "i32", "value name");

    /* object */
    Cent_ast* object = Cent_ast_get(const_, 1);
    Zinc_assert_ptr(object, "ptr object");
    Zinc_expect_int_equal(object->type, Cent_ast_type_expr_object, "type object");

    /* type prop set */
    Cent_ast* type_prop_set = Cent_ast_get(object, 0);
    Zinc_assert_ptr(type_prop_set, "ptr type_prop_set");
    Zinc_expect_int_equal(type_prop_set->type, Cent_ast_type_prop_set, "type type_prop_set");

    /* type */
    Cent_ast* type = Cent_ast_get(type_prop_set, 0);
    Zinc_assert_ptr(type, "ptr type");
    Zinc_expect_int_equal(type->type, Cent_ast_type_id, "type type");
    Zinc_expect_string(&type->text, "type", "value type");

    /* enum namespace */
    Cent_ast* ns = Cent_ast_get(type_prop_set, 1);
    Zinc_assert_ptr(ns, "ptr ns");
    Zinc_expect_int_equal(ns->type, Cent_ast_type_namespace, "type ns");

    /* enum id */
    Cent_ast* enum_id = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(enum_id, "ptr enum_id");
    Zinc_expect_int_equal(enum_id->type, Cent_ast_type_id, "type enum_id");
    Zinc_expect_string(&enum_id->text, "Type_def_type", "text enum_id");

    /* enum value */
    Cent_ast* enum_value_id = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(enum_value_id, "ptr enum_value_id");
    Zinc_expect_int_equal(enum_value_id->type, Cent_ast_type_id, "type enum_value_id");
    Zinc_expect_string(&enum_value_id->text, "Integer", "text enum_value_id");

    /* name prop set */
    Cent_ast* name_prop_set = Cent_ast_get(object, 1);
    Zinc_assert_ptr(name_prop_set, "ptr name_prop_set");
    Zinc_expect_int_equal(name_prop_set->type, Cent_ast_type_prop_set, "type name_prop_set");

    /* name */
    Cent_ast* name1 = Cent_ast_get(name_prop_set, 0);
    Zinc_assert_ptr(name1, "ptr type");
    Zinc_expect_int_equal(name1->type, Cent_ast_type_id, "type name1");
    Zinc_expect_string(&name1->text, "name", "text name");

    /* name value */
    Cent_ast* name_value = Cent_ast_get(name_prop_set, 1);
    Zinc_assert_ptr(name_value, "ptr name_value");
    Zinc_expect_int_equal(name_value->type, Cent_ast_type_expr_string, "type name_value");
    Zinc_expect_int_equal(name_value->value_type, Cent_value_type_string, "value_type name_value");
    Zinc_expect_string(&name_value->data.string, "i32", "string name_value");

    /* bit_count prop set */
    Cent_ast* bit_count_prop_set = Cent_ast_get(object, 2);
    Zinc_assert_ptr(bit_count_prop_set, "ptr bit_count_prop_set");
    Zinc_expect_int_equal(bit_count_prop_set->type, Cent_ast_type_prop_set, "type bit_count_prop_set");

    /* bit_count */
    Cent_ast* bit_count = Cent_ast_get(bit_count_prop_set, 0);
    Zinc_assert_ptr(bit_count, "ptr bit_count");
    Zinc_expect_int_equal(bit_count->type, Cent_ast_type_id, "type bit_count");
    Zinc_expect_string(&bit_count->text, "bit_count", "text bit_count");

    /* bit_count value */
    Cent_ast* bit_count_value = Cent_ast_get(bit_count_prop_set, 1);
    Zinc_assert_ptr(bit_count_value, "ptr bit_count_value");
    Zinc_expect_int_equal(bit_count_value->type, Cent_ast_type_expr_natural, "type bit_count_value");
    Zinc_expect_int_equal(bit_count_value->value_type, Cent_value_type_natural, "value_type bit_count_value");
    Zinc_expect_uint64_t_equal(bit_count_value->data.natural, 32, "integer bit_count_value");

    /* is_signed prop set */
    Cent_ast* is_signed_prop_set = Cent_ast_get(object, 3);
    Zinc_assert_ptr(is_signed_prop_set, "ptr is_signed_prop_set");
    Zinc_expect_int_equal(is_signed_prop_set->type, Cent_ast_type_prop_set, "type is_signed_prop_set");

    /* is_signed */
    Cent_ast* is_signed = Cent_ast_get(is_signed_prop_set, 0);
    Zinc_assert_ptr(is_signed, "ptr is_signed");
    Zinc_expect_int_equal(is_signed->type, Cent_ast_type_id, "type is_signed");
    Zinc_expect_string(&is_signed->text, "is_signed", "text is_signed");

    /* is_signed value */
    Cent_ast* is_signed_value = Cent_ast_get(is_signed_prop_set, 1);
    Zinc_assert_ptr(is_signed_value, "ptr is_signed_value");
    Zinc_expect_int_equal(is_signed_value->type, Cent_ast_type_expr_boolean, "type is_signed_value");
    Zinc_expect_int_equal(is_signed_value->value_type, Cent_value_type_boolean, "value_type is_signed_value");
    Zinc_expect_long_long_equal(is_signed_value->data.boolean, true, "boolean is_signed_value");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_error_unhandled_token()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "1 }");

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "unhandled token: right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_error_expected_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "element 1");

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_error_expected_left_curly_brace()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected left-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_error_name_already_exits()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "}\n"
        "element Test {\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "name already exists: Test");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_error_properties_expected_right_curly_brace()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "    properties {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_error_property_expected_colon()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "    properties {\n"
        "        count Integer\n"
        "    }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected colon");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_error_property_expected_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "    properties {\n"
        "        count:\n"
        "    }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_error_children_expected_right_curly_brace()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "    children {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_enumerate_error_expected_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum {\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_enumerate_error_expected_right_curly_brace()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Ast_type {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_value_error_enum_expected_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Test_type {\n"
        "}\n"
        "Test_type::1\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_value_error_object_expected_rcb()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_value_error_object_property_expected_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
        "   . = 1\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_value_error_object_property_expected_equal()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
        "   .a 1\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected equal");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_property_unknown_type()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "   properties {\n"
        "        a: Abc\n"
        "   }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "unknown type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_property_type_not_element()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const Abc = 1;\n"
        "element Test {\n"
        "   properties {\n"
        "        a: Abc\n"
        "   }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "type is not an element or enum type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_child_unknown_type()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "   children {\n"
        "        Abc\n"
        "   }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "unknown type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_child_type_not_an_element_type()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const Abc = 1;\n"
        "element Test {\n"
        "   children {\n"
        "        Abc\n"
        "   }\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "type is not an element type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_enum_error_duplicate_enum_value()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Symbol_type {\n"
        "   Element\n"
        "   Enumerate\n"
        "   Info\n"
        "   Variable\n"
        "   Info\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "duplicate enum value: Symbol_type::Info");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_object_method_call()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = Foo {};\n"
        "Bar {\n"
        "    .@child_of(a)\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    Zinc_expect_no_errors(errors);

    /* root */
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* const */
    Cent_ast* const_ = Cent_ast_get(root, 0);
    Zinc_assert_ptr(const_, "ptr const_");
    Zinc_expect_int_equal(const_->type, Cent_ast_type_const, "type const_");

    /* Bar object */
    Cent_ast* bar = Cent_ast_get(root, 1);
    Zinc_assert_ptr(bar, "ptr bar");
    Zinc_expect_int_equal(bar->type, Cent_ast_type_expr_object, "type bar");

    /* method child of */
    Cent_ast* method = Cent_ast_get(bar, 0);
    Zinc_assert_ptr(method, "ptr method");
    Zinc_expect_int_equal(method->type, Cent_ast_type_method_child_of, "type method");

    /* variable a */
    Cent_ast* value = Cent_ast_get(method, 0);
    Zinc_assert_ptr(value, "ptr value");
    Zinc_expect_int_equal(value->type, Cent_ast_type_expr_variable, "type value");
    Zinc_expect_string(&value->text, "a", "text id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_object_method_call2()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = Foo {};\n"
        "Bar {\n"
        "    .@property_of(a, \"b\")\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    Zinc_expect_no_errors(errors);

    /* root */
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* const */
    Cent_ast* const_ = Cent_ast_get(root, 0);
    Zinc_assert_ptr(const_, "ptr assign");
    Zinc_expect_int_equal(const_->type, Cent_ast_type_const, "type const_");

    /* Bar object */
    Cent_ast* bar = Cent_ast_get(root, 1);
    Zinc_assert_ptr(bar, "ptr bar");
    Zinc_expect_int_equal(bar->type, Cent_ast_type_expr_object, "type bar");

    /* method child of */
    Cent_ast* method = Cent_ast_get(bar, 0);
    Zinc_assert_ptr(method, "ptr method");
    Zinc_expect_int_equal(method->type, Cent_ast_type_method_property_of, "type method");

    /* argument 1 a */
    Cent_ast* value = Cent_ast_get(method, 0);
    Zinc_assert_ptr(value, "ptr value");
    Zinc_expect_int_equal(value->type, Cent_ast_type_expr_variable, "type value");
    Zinc_expect_string(&value->text, "a", "text id");

    /* argument 2 string b */
    Cent_ast* name = Cent_ast_get(method, 1);
    Zinc_assert_ptr(name, "ptr name");
    Zinc_expect_int_equal(name->type, Cent_ast_type_expr_string, "type name");
    Zinc_expect_string(&name->data.string, "b", "text name");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_function_call()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Foo {\n"
        "    .b = @file_name()\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    Zinc_expect_no_errors(errors);

    /* root */
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* Foo object */
    Cent_ast* foo = Cent_ast_get(root, 0);
    Zinc_assert_ptr(foo, "ptr foo");
    Zinc_expect_int_equal(foo->type, Cent_ast_type_expr_object, "type foo");
    Zinc_expect_string(&foo->text, "Foo", "text foo");

    /* property set b */
    Cent_ast* prop_set1 = Cent_ast_get(foo, 0);
    Zinc_assert_ptr(prop_set1, "ptr prop_set1");
    Zinc_expect_int_equal(prop_set1->type, Cent_ast_type_prop_set, "type prop_set1");

    /* b id */
    Cent_ast* b = Cent_ast_get(prop_set1, 0);
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Cent_ast_type_id, "type b");
    Zinc_expect_string(&b->text, "b", "text b");

    /* file_name() */
    Cent_ast* file_name_call = Cent_ast_get(prop_set1, 1);
    Zinc_assert_ptr(file_name_call, "ptr file_name_call");
    Zinc_expect_int_equal(
        file_name_call->type,
        Cent_ast_type_expr_function_file_name,
        "type file_name_call");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_enum_duplicate_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Foo {\n"
        "    One\n"
        "    Two\n"
        "    Two\n"
        "    Three\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "duplicate enum value: Foo::Two");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 4, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 5, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_enum_error_could_not_find_enum()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Foo {\n"
        "    One\n"
        "    Two\n"
        "    Three\n"
        "}\n"
        "Bar::Two\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "id is not a variable: Bar");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 6, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_enum_error_could_not_find_enum_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Foo {\n"
        "    One\n"
        "    Two\n"
        "    Three\n"
        "}\n"
        "Foo::Four\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    struct Zinc_error* e = Zinc_expect_source_error(errors, "invalid enum id: Four");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.line, 6, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 6, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_include()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use types;\n"
        "Groceries {\n"
        "    types::Grocery_item::Milk\n"
        "}\n"
    );

    Cent_comp_table_add_module_str(ct, "types.cent",
        "enum Grocery_item {\n"
        "    Milk\n"
        "    Cereal\n"
        "    Steak\n"
        "    Potatoes\n"
        "    Carrots\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    Zinc_expect_no_errors(errors);

    /* root */
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* use */
    Cent_ast* use = Cent_ast_get(root, 0);
    Zinc_assert_ptr(use, "ptr use");
    Zinc_expect_int_equal(use->type, Cent_ast_type_use, "type use");

    Cent_ast* module = Cent_ast_get(use, 0);
    Zinc_assert_ptr(module, "ptr module");
    Zinc_expect_int_equal(module->type, Cent_ast_type_id, "type module");
    Zinc_expect_string(&module->text, "types", "text module");

    /* Groceries object */
    Cent_ast* object = Cent_ast_get(root, 1);
    Zinc_assert_ptr(object, "ptr object");
    Zinc_expect_int_equal(object->type, Cent_ast_type_expr_object, "type object");
    Zinc_expect_string(&object->text, "Groceries", "text object");

    /* namespace */
    Cent_ast* ns = Cent_ast_get(object, 0);
    Zinc_assert_ptr(ns, "ptr ns");
    Zinc_expect_int_equal(ns->type, Cent_ast_type_namespace, "type ns");

    Cent_ast* types = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(types, "ptr types");
    Zinc_expect_int_equal(types->type, Cent_ast_type_id, "type types");
    Zinc_expect_string(&types->text, "types", "text types");

    Cent_ast* grocery_item = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(grocery_item, "ptr grocery_item");
    Zinc_expect_int_equal(grocery_item->type, Cent_ast_type_id, "type grocery_item");
    Zinc_expect_string(&grocery_item->text, "Grocery_item", "text grocery_item");

    Cent_ast* milk = Cent_ast_get(ns, 2);
    Zinc_assert_ptr(milk, "ptr milk");
    Zinc_expect_int_equal(milk->type, Cent_ast_type_id, "type milk");
    Zinc_expect_string(&milk->text, "Milk", "text milk");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_include_multiple_namespace()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use lib::types;\n"
        "Groceries {\n"
        "    lib::types::Grocery_item::Milk\n"
        "}\n"
    );

    Cent_comp_table_add_module_str(ct, "lib/types.cent",
        "enum Grocery_item {\n"
        "    Milk\n"
        "    Cereal\n"
        "    Steak\n"
        "    Potatoes\n"
        "    Carrots\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    Zinc_expect_no_errors(errors);

    /* root */
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* use */
    Cent_ast* use = Cent_ast_get(root, 0);
    Zinc_assert_ptr(use, "ptr use");
    Zinc_expect_int_equal(use->type, Cent_ast_type_use, "type use");

    Cent_ast* lib_mod = Cent_ast_get(use, 0);
    Zinc_assert_ptr(lib_mod, "ptr lib_mod");
    Zinc_expect_int_equal(lib_mod->type, Cent_ast_type_id, "type lib_mod");
    Zinc_expect_string(&lib_mod->text, "lib", "text lib_mod");

    Cent_ast* types_mod = Cent_ast_get(use, 1);
    Zinc_assert_ptr(types_mod, "ptr types_mod");
    Zinc_expect_int_equal(types_mod->type, Cent_ast_type_id, "type types_mod");
    Zinc_expect_string(&types_mod->text, "types", "text types_mod");

    /* Groceries object */
    Cent_ast* object = Cent_ast_get(root, 1);
    Zinc_assert_ptr(object, "ptr object");
    Zinc_expect_int_equal(object->type, Cent_ast_type_expr_object, "type object");
    Zinc_expect_string(&object->text, "Groceries", "text object");

    /* namespace */
    Cent_ast* ns = Cent_ast_get(object, 0);
    Zinc_assert_ptr(ns, "ptr ns");
    Zinc_expect_int_equal(ns->type, Cent_ast_type_namespace, "type ns");

    Cent_ast* lib = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(lib, "ptr lib");
    Zinc_expect_int_equal(lib->type, Cent_ast_type_id, "type lib");
    Zinc_expect_string(&lib->text, "lib", "text lib");

    Cent_ast* types = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(types, "ptr types");
    Zinc_expect_int_equal(types->type, Cent_ast_type_id, "type types");
    Zinc_expect_string(&types->text, "types", "text types");

    Cent_ast* grocery_item = Cent_ast_get(ns, 2);
    Zinc_assert_ptr(grocery_item, "ptr grocery_item");
    Zinc_expect_int_equal(grocery_item->type, Cent_ast_type_id, "type grocery_item");
    Zinc_expect_string(&grocery_item->text, "Grocery_item", "text grocery_item");

    Cent_ast* milk = Cent_ast_get(ns, 3);
    Zinc_assert_ptr(milk, "ptr milk");
    Zinc_expect_int_equal(milk->type, Cent_ast_type_id, "type milk");
    Zinc_expect_string(&milk->text, "Milk", "text milk");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_include_value()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use data;\n"
        "data::a\n"
    );

    Cent_comp_table_add_module_str(ct, "data.cent",
        "const a = 12597;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;

    Zinc_expect_no_errors(errors);

    /* root */
    Zinc_assert_ptr(root, "ptr pr.root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type pr.root");

    /* use */
    Cent_ast* use = Cent_ast_get(root, 0);
    Zinc_assert_ptr(use, "ptr use");
    Zinc_expect_int_equal(use->type, Cent_ast_type_use, "type use");

    Cent_ast* data_mod = Cent_ast_get(use, 0);
    Zinc_assert_ptr(data_mod, "ptr data_mod");
    Zinc_expect_int_equal(data_mod->type, Cent_ast_type_id, "type data_mod");
    Zinc_expect_string(&data_mod->text, "data", "text data_mod");

    /* namespace */
    Cent_ast* ns = Cent_ast_get(root, 1);
    Zinc_assert_ptr(ns, "ptr ns");
    Zinc_expect_int_equal(ns->type, Cent_ast_type_namespace, "type ns");

    Cent_ast* data = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(data, "ptr data");
    Zinc_expect_int_equal(data->type, Cent_ast_type_id, "type data");
    Zinc_expect_string(&data->text, "data", "text data");

    Cent_ast* a = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Cent_ast_type_id, "type a");
    Zinc_expect_string(&a->text, "a", "text a");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_include_value_error()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use data;\n"
        "data::b\n"
    );

    Cent_comp_table_add_module_str(ct, "data.cent",
        "const a = 12597;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "not a valid id: b");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_include_glob()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use data::*;\n"
        "a\n"
    );

    Cent_comp_table_add_module_str(ct, "data.cent",
        "const a = 12597;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_no_errors(errors);

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_include_error_expected_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use 1::2;\n"
        "a\n"
    );

    Cent_comp_table_add_module_str(ct, "1/2.cent",
        "const a = 12597;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_namespace_error_expected_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use lib::data;\n"
        "1::a\n"
    );

    Cent_comp_table_add_module_str(ct, "lib/data.cent",
        "const a = 12597;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_namespace_error_expected_id2()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use lib::data;\n"
        "data::1\n"
    );

    Cent_comp_table_add_module_str(ct, "lib/data.cent",
        "const a = 12597;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_const()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 45;\n"
        "a\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;
    
    Zinc_expect_no_errors(errors);

    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type root");

    Cent_ast* const_ = Cent_ast_get(root, 0);
    Zinc_assert_ptr(const_, "ptr const_");
    Zinc_expect_int_equal(const_->type, Cent_ast_type_const, "type const_");

    Cent_ast* id = Cent_ast_get(const_, 0);
    Zinc_assert_ptr(id, "ptr id");
    Zinc_expect_int_equal(id->type, Cent_ast_type_id, "type id");
    Zinc_expect_string(&id->text, "a", "text id");

    Cent_ast* number = Cent_ast_get(const_, 1);
    Zinc_assert_ptr(number, "ptr number");
    Zinc_expect_int_equal(number->type, Cent_ast_type_expr_natural, "type number");
    Zinc_expect_uint64_t_equal(number->data.natural, 45, "integer number");

    Cent_ast* variable = Cent_ast_get(root, 1);
    Zinc_assert_ptr(variable, "ptr variable");
    Zinc_expect_int_equal(variable->type, Cent_ast_type_expr_variable, "type variable");
    Zinc_expect_string(&variable->text, "a", "text variable");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_const_error_shadow_type()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "}\n"
        "const Foo = 45;\n"
        "Foo\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "shadowing of type: Foo");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_const_error_shadow_module()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use base;\n"
        "const base = 45;\n"
        "base\n"
    );

    Cent_comp_table_add_module_str(ct, "base.cent", "");

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "shadowing of module: base");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_const_error_shadow_local()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 1;\n"
        "const a = 2;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "shadowing of local variable: a");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_object_const()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Foo {\n"
        "    const bar = Bar {};\n"
        "    bar\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;
    
    Zinc_expect_no_errors(errors);

    /* root */
    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type root");

    /* line 1 */
    Cent_ast* foo = Cent_ast_get(root, 0);
    Zinc_assert_ptr(foo, "ptr foo");
    Zinc_expect_int_equal(foo->type, Cent_ast_type_expr_object, "type foo");

    /* line 2 */
    Cent_ast* foo_const = Cent_ast_get(foo, 0);
    Zinc_assert_ptr(foo_const, "ptr foo_const");
    Zinc_expect_int_equal(foo_const->type, Cent_ast_type_const, "type foo_const");

    Cent_ast* bar = Cent_ast_get(foo_const, 0);
    Zinc_assert_ptr(bar, "ptr bar");
    Zinc_expect_int_equal(bar->type, Cent_ast_type_id, "type bar");

    Cent_ast* bar_object = Cent_ast_get(foo_const, 1);
    Zinc_assert_ptr(bar_object, "ptr bar_object");
    Zinc_expect_int_equal(bar_object->type, Cent_ast_type_expr_object, "type bar_object");
    Zinc_expect_string(&bar_object->text, "Bar", "text bar_object");

    /* line 3 */
    Cent_ast* bar_variable = Cent_ast_get(foo, 1);
    Zinc_assert_ptr(bar_variable, "ptr bar_variable");
    Zinc_expect_int_equal(bar_variable->type, Cent_ast_type_expr_variable, "type bar_variable");
    Zinc_expect_string(&bar_variable->text, "bar", "text bar_variable");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_bad_id()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 1;\n"
        "b\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "unknown variable: b");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_module_id_error()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
                     "const math = 1;\n"
                     "use math;\n"
                     "math::Pi\n"
    );

    Cent_comp_table_add_module_str(ct, "math.cent",
                             "const Pi = 3.14;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);

    Zinc_expect_source_error(errors, "module identifier collides with existing identifier: math");
    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_bad_id_child_of()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Foo {\n"
        "    const bar = Bar {\n"
        "        .@child_of(x)\n"
        "    }\n"
        "    bar\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "unknown variable: x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_bad_id_property_of()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Foo {\n"
        "    const bar = Bar {\n"
        "        .@property_of(x, \"a\")\n"
        "    }\n"
        "    bar\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "unknown variable: x");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_follow_on()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Ast {\n"
        "    properties {\n"
        "        type: Ast_type\n"
        "        value: String\n"
        "    }\n"
        "    children {\n"
        "        Ast\n"
        "    }\n"
        "}\n"
        "enum Ast_type {\n"
        "    Number\n"
        "    Add\n"
        "    Subtract\n"
        "    Multiply\n"
        "    Divide\n"
        "}\n"
        "Ast {.type=Ast_type::Add}\n"
        ".Ast {.type=Ast_type::Number;.value=\"10\"}\n"
        ".Ast {.type=Ast_type::Number;.value=\"5\"}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Zinc_expect_no_errors(errors);

    /* root */
    Cent_ast* root = ct->primary->pr.root;
    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type root");

    /* element type */
    Cent_ast* element0 = Cent_ast_get(root, 0);
    Zinc_assert_ptr(element0, "ptr element0");
    Zinc_expect_int_equal(element0->type, Cent_ast_type_element_type, "type element0");

    /* enum */
    Cent_ast* enum0 = Cent_ast_get(root, 1);
    Zinc_assert_ptr(enum0, "ptr enum0");
    Zinc_expect_int_equal(enum0->type, Cent_ast_type_enum_type, "type enum0");
    
    /* add */
    Cent_ast* add = Cent_ast_get(root, 2);
    Zinc_assert_ptr(add, "ptr add");
    Zinc_expect_int_equal(add->type, Cent_ast_type_expr_object, "type add");

    /* type */
    Cent_ast* add_prop0 = Cent_ast_get(add, 0);
    Zinc_assert_ptr(add_prop0, "ptr add_prop0");
    Zinc_expect_int_equal(add_prop0->type, Cent_ast_type_prop_set, "type add_prop0");

    Cent_ast* add_prop0_name = Cent_ast_get(add_prop0, 0);
    Zinc_assert_ptr(add_prop0_name, "ptr add_prop0_name");
    Zinc_expect_string(&add_prop0_name->text, "type", "value add_prop0_name");

    Cent_ast* namespace0 = Cent_ast_get(add_prop0, 1);
    Zinc_assert_ptr(namespace0, "ptr namespace0");
    Zinc_expect_int_equal(namespace0->type, Cent_ast_type_namespace, "type namespace0");
    
    Cent_ast* ns0_id0 = Cent_ast_get(namespace0, 0);
    Zinc_assert_ptr(ns0_id0, "ptr add_id0");
    Zinc_expect_int_equal(ns0_id0->type, Cent_ast_type_id, "type ns0_id0");
    Zinc_expect_string(&ns0_id0->text, "Ast_type", "text ns0_id0");

    Cent_ast* ns0_id1 = Cent_ast_get(namespace0, 1);
    Zinc_assert_ptr(ns0_id1, "ptr add_id1");
    Zinc_expect_int_equal(ns0_id1->type, Cent_ast_type_id, "type ns0_id1");
    Zinc_expect_string(&ns0_id1->text, "Add", "text ns0_id1");

    /* number 0 node */
    Cent_ast* node0 = Cent_ast_get(root, 3);
    Zinc_assert_ptr(node0, "ptr node0");
    Zinc_expect_int_equal(node0->type, Cent_ast_type_follow_on, "type node0");
    
    /* number 0 level */
    Cent_ast* level0 = Cent_ast_get(node0, 0);
    Zinc_assert_ptr(level0, "ptr level0");
    Zinc_expect_int_equal(level0->type, Cent_ast_type_expr_natural, "type level0");
    Zinc_expect_uint64_t_equal(level0->data.natural, 1, "integer level0");

    /* number 0 object */
    Cent_ast* number0 = Cent_ast_get(node0, 1);
    Zinc_assert_ptr(number0, "ptr number0");
    Zinc_expect_int_equal(number0->type, Cent_ast_type_expr_object, "type number0");

    /* number 0 prop 0 */
    Cent_ast* n0_prop0 = Cent_ast_get(number0, 0);
    Zinc_assert_ptr(n0_prop0, "ptr n0_type");
    Zinc_expect_int_equal(n0_prop0->type, Cent_ast_type_prop_set, "type n0_prop0");

    /* number 0 prop 0 id */
    Cent_ast* n0_prop0_id = Cent_ast_get(n0_prop0, 0);
    Zinc_assert_ptr(n0_prop0_id, "ptr n0_prop0_id");
    Zinc_expect_int_equal(n0_prop0_id->type, Cent_ast_type_id, "type n0_prop0_id");
    Zinc_expect_string(&n0_prop0_id->text, "type", "text n0_prop0_id");

    /* number 0 prop 0 ns */
    Cent_ast* n0_prop0_ns = Cent_ast_get(n0_prop0, 1);
    Zinc_assert_ptr(n0_prop0_ns, "ptr n0_prop0_ns");
    Zinc_expect_int_equal(n0_prop0_ns->type, Cent_ast_type_namespace, "type n0_prop0_ns");

    /* number 0 prop 0 ns id 0 */
    Cent_ast* n0_prop0_ns_id0 = Cent_ast_get(n0_prop0_ns, 0);
    Zinc_assert_ptr(n0_prop0_ns_id0, "ptr n0_prop0_ns_id0");
    Zinc_expect_int_equal(n0_prop0_ns_id0->type, Cent_ast_type_id, "type n0_prop0_ns_id0");
    Zinc_expect_string(&n0_prop0_ns_id0->text, "Ast_type", "type n0_prop0_ns_id0");

    /* number 0 prop 0 ns id 1 */
    Cent_ast* n0_prop0_ns_id1 = Cent_ast_get(n0_prop0_ns, 1);
    Zinc_assert_ptr(n0_prop0_ns_id1, "ptr n0_prop0_ns_id1");
    Zinc_expect_int_equal(n0_prop0_ns_id1->type, Cent_ast_type_id, "type n0_prop0_ns_id1");
    Zinc_expect_string(&n0_prop0_ns_id1->text, "Number", "type n0_prop0_ns_id1");

    /* number 0 prop 1 */
    Cent_ast* n0_prop1 = Cent_ast_get(number0, 1);
    Zinc_assert_ptr(n0_prop1, "ptr n0_prop1");
    Zinc_expect_int_equal(n0_prop1->type, Cent_ast_type_prop_set, "type n0_prop1");

    /* number 0 prop 1 id */
    Cent_ast* n0_prop1_id = Cent_ast_get(n0_prop1, 0);
    Zinc_assert_ptr(n0_prop1_id, "ptr n0_prop1_id");
    Zinc_expect_int_equal(n0_prop1_id->type, Cent_ast_type_id, "type n0_prop1_id");
    Zinc_expect_string(&n0_prop1_id->text, "value", "text n0_prop1_id");
 
    /* number 0 prop 1 string */
    Cent_ast* n0_prop1_string = Cent_ast_get(n0_prop1, 1);
    Zinc_assert_ptr(n0_prop1_string, "ptr n0_prop0_string");
    Zinc_expect_int_equal(n0_prop1_string->type, Cent_ast_type_expr_string, "type n0_prop1_string");
    Zinc_expect_string(&n0_prop1_string->data.string, "10", "value n0_prop1_string");

    /* number 1 node */
    Cent_ast* node1 = Cent_ast_get(root, 4);
    Zinc_assert_ptr(node1, "ptr node1");
    Zinc_expect_int_equal(node1->type, Cent_ast_type_follow_on, "type node1");

    /* number 1 level */
    Cent_ast* level1 = Cent_ast_get(node1, 0);
    Zinc_assert_ptr(level1, "ptr level1");
    Zinc_expect_int_equal(level1->type, Cent_ast_type_expr_natural, "type level1");
    Zinc_expect_uint64_t_equal(level1->data.natural, 1, "integer level1");

    /* number 1 object */
    Cent_ast* number1 = Cent_ast_get(node1, 1);
    Zinc_assert_ptr(number1, "ptr number1");
    Zinc_expect_int_equal(number1->type, Cent_ast_type_expr_object, "type number1");
 
    /* number 1 prop 0 */
    Cent_ast* n1_prop0 = Cent_ast_get(number1, 0);
    Zinc_assert_ptr(n1_prop0, "ptr n1_type");
    Zinc_expect_int_equal(n1_prop0->type, Cent_ast_type_prop_set, "type n1_prop0");

    /* number 1 prop 0 id */
    Cent_ast* n1_prop0_id = Cent_ast_get(n1_prop0, 0);
    Zinc_assert_ptr(n1_prop0_id, "ptr n1_prop0_id");
    Zinc_expect_int_equal(n1_prop0_id->type, Cent_ast_type_id, "type n1_prop0_id");
    Zinc_expect_string(&n1_prop0_id->text, "type", "text n1_prop0_id");

    /* number 1 prop 0 ns */
    Cent_ast* n1_prop0_ns = Cent_ast_get(n1_prop0, 1);
    Zinc_assert_ptr(n1_prop0_ns, "ptr n1_prop0_ns");
    Zinc_expect_int_equal(n1_prop0_ns->type, Cent_ast_type_namespace, "type n1_prop0_ns");

    /* number 1 prop 0 ns id 0 */
    Cent_ast* n1_prop0_ns_id0 = Cent_ast_get(n1_prop0_ns, 0);
    Zinc_assert_ptr(n1_prop0_ns_id0, "ptr n1_prop0_ns_id0");
    Zinc_expect_int_equal(n1_prop0_ns_id0->type, Cent_ast_type_id, "type n1_prop0_ns_id0");
    Zinc_expect_string(&n1_prop0_ns_id0->text, "Ast_type", "type n1_prop0_ns_id0");

    /* number 1 prop 0 ns id 1 */
    Cent_ast* n1_prop0_ns_id1 = Cent_ast_get(n1_prop0_ns, 1);
    Zinc_assert_ptr(n1_prop0_ns_id1, "ptr n1_prop0_ns_id1");
    Zinc_expect_int_equal(n1_prop0_ns_id1->type, Cent_ast_type_id, "type n1_prop0_ns_id1");
    Zinc_expect_string(&n1_prop0_ns_id1->text, "Number", "type n1_prop0_ns_id1");

    /* number 1 prop 1 */
    Cent_ast* n1_prop1 = Cent_ast_get(number1, 1);
    Zinc_assert_ptr(n1_prop1, "ptr n1_prop1");
    Zinc_expect_int_equal(n1_prop1->type, Cent_ast_type_prop_set, "type n1_prop1");

    /* number 1 prop 1 id */
    Cent_ast* n1_prop1_id = Cent_ast_get(n1_prop1, 0);
    Zinc_assert_ptr(n1_prop1_id, "ptr n1_prop1_id");
    Zinc_expect_int_equal(n1_prop1_id->type, Cent_ast_type_id, "type n1_prop1_id");
    Zinc_expect_string(&n1_prop1_id->text, "value", "text n1_prop1_id");
 
    /* number 1 prop 1 string */
    Cent_ast* n1_prop1_string = Cent_ast_get(n1_prop1, 1);
    Zinc_assert_ptr(n1_prop1_string, "ptr n1_prop0_string");
    Zinc_expect_int_equal(n1_prop1_string->type, Cent_ast_type_expr_string, "type n1_prop1_string");
    Zinc_expect_string(&n1_prop1_string->data.string, "5", "value n1_prop1_string");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_follow_on_error_no_previous_object()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        ".Ast {.type=Ast_type::Add}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "follow-on has no previous object");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_follow_on_error_follow_on_is_to_non_object()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Ast {\n"
        "}\n"
        ".Ast {.type=Ast_type::Add}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "follow-on is to non-object");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_follow_on_error_follow_on_increased_level_greater_than_one()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Ast {\n"
        "}\n"
        "Ast {}\n"
        "..Ast {}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "follow-on increased level greater than one");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_follow_on_error_follow_on_increased_level_greater_than_one2()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Ast {\n"
        "}\n"
        "Ast{}\n"
        ".Ast {}\n"
        "...Ast {}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Zinc_expect_has_errors(errors);
    Zinc_expect_source_error(errors, "follow-on increased level greater than one");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_element_tagged()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Vehicle(Vehicle_type) {\n"
        "}\n"
        "enum Vehicle_type {\n"
        "    Car\n"
        "    Truck\n"
        "    Van\n"
        "    Bike\n"
        "}\n"
        "Vehicle::Car {}\n"
        "Vehicle::Bike {}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Zinc_expect_no_errors(errors);

    /* root */
    Cent_ast* root = ct->primary->pr.root;
    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type root");

    /* element type */
    Cent_ast* element = Cent_ast_get(root, 0);
    Zinc_assert_ptr(element, "ptr element");
    Zinc_expect_int_equal(element->type, Cent_ast_type_element_type, "type element");
    Zinc_expect_string(&element->text, "Vehicle", "text element");

    /* enum type */
    Cent_ast* enum_ = Cent_ast_get(root, 1);
    Zinc_assert_ptr(enum_, "ptr enum_");
    Zinc_expect_int_equal(enum_->type, Cent_ast_type_enum_type, "type enum_");
    Zinc_expect_string(&enum_->text, "Vehicle_type", "text enum_");

    /* Car */
    Cent_ast* item0 = Cent_ast_get(enum_, 0);
    Zinc_assert_ptr(item0, "ptr item0");
    Zinc_expect_int_equal(item0->type, Cent_ast_type_id, "type item0");
    Zinc_expect_string(&item0->text, "Car", "text item0");

    /* Truck */
    Cent_ast* item1 = Cent_ast_get(enum_, 1);
    Zinc_assert_ptr(item1, "ptr item1");
    Zinc_expect_int_equal(item1->type, Cent_ast_type_id, "type item1");
    Zinc_expect_string(&item1->text, "Truck", "text item1");
    
    /* Van */
    Cent_ast* item2 = Cent_ast_get(enum_, 2);
    Zinc_assert_ptr(item2, "ptr item2");
    Zinc_expect_int_equal(item2->type, Cent_ast_type_id, "type item2");
    Zinc_expect_string(&item2->text, "Van", "text item2");

    /* Bike */
    Cent_ast* item3 = Cent_ast_get(enum_, 3);
    Zinc_assert_ptr(item3, "ptr item3");
    Zinc_expect_int_equal(item3->type, Cent_ast_type_id, "type item3");
    Zinc_expect_string(&item3->text, "Bike", "text item3");

    /* object car */
    Cent_ast* car = Cent_ast_get(root, 2);
    Zinc_assert_ptr(car, "ptr car");
    Zinc_expect_int_equal(car->type, Cent_ast_type_expr_object, "type car");
    Zinc_expect_string(&car->text, "Vehicle", "text car");

    /* car variant set */
    Cent_ast* car_var_set = Cent_ast_get(car, 0);
    Zinc_assert_ptr(car_var_set, "ptr car_var_set");
    Zinc_expect_int_equal(car_var_set->type, Cent_ast_type_variant_set, "type car_var_set");

    /* car @tag */
    Cent_ast* car_tag = Cent_ast_get(car, 1);
    Zinc_assert_ptr(car_tag, "ptr car_tag");
    Zinc_expect_int_equal(car_tag->type, Cent_ast_type_prop_set, "type car_tag");
    
    /* car @tag name */
    Cent_ast* car_tag_name = Cent_ast_get(car_tag, 0);
    Zinc_assert_ptr(car_tag_name, "ptr car_tag_name");
    Zinc_expect_int_equal(car_tag_name->type, Cent_ast_type_id, "type, car_tag_name");
    Zinc_expect_string(&car_tag_name->text, "@tag", "text car_tag_name");

    /* car @tag namespace */
    Cent_ast* car_tag_ns = Cent_ast_get(car_tag, 1);
    Zinc_assert_ptr(car_tag_ns, "ptr car_tag_ns");
    Zinc_expect_int_equal(car_tag_ns->type, Cent_ast_type_namespace, "type, car_tag_ns");
    Zinc_expect_int_equal(car_tag_ns->value_type, Cent_value_type_enum, "value_type car_tag_ns");
    Zinc_expect_size_t_equal(car_tag_ns->data.enumeration.number, 0, "enumeration number car_tag_ns");

    /* car @tag namespace id1 */
    Cent_ast* car_tag_ns_id1 = Cent_ast_get(car_tag_ns, 0);
    Zinc_assert_ptr(car_tag_ns_id1, "ptr car_tag_ns_id1");
    Zinc_expect_int_equal(car_tag_ns_id1->type, Cent_ast_type_id, "type car_tag_ns_id1");
    Zinc_expect_string(&car_tag_ns_id1->text, "Vehicle_type", "text car_tag_ns_id1");

    /* car @tag namespace id2 */
    Cent_ast* car_tag_ns_id2 = Cent_ast_get(car_tag_ns, 1);
    Zinc_assert_ptr(car_tag_ns_id2, "ptr car_tag_ns_id2");
    Zinc_expect_int_equal(car_tag_ns_id2->type, Cent_ast_type_id, "type car_tag_ns_id2");
    Zinc_expect_string(&car_tag_ns_id2->text, "Car", "text car_tag_ns_id2");


    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse_variant()
{
    Zinc_test_name(__func__);

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Monster(MonsterKind) {\n"
        "    properties {\n"
        "        hp: Integer\n"
        "        attack: Integer\n"
        "        defense: Integer\n"
        "    }\n"
        "}\n"
        "enum MonsterKind {\n"
        "    Bat\n"
        "    Rat\n"
        "    Orc\n"
        "    Goblin\n"
        "    Skeleton\n"
        "    Human\n"
        "    Dog\n"
        "}\n"
        "variant Monster::Bat {\n"
        "    properties {\n"
        "        siphon: Integer\n"
        "    }\n"
        "}\n"
        "Monsters {\n"
        "    Monster::Bat {"
        "       .hp = 10\n"
        "       .attack = 2\n"
        "       .defense = 1\n"
        "       .siphon = 1\n"
        "    }\n"
        "    Monster::Human {"
        "        .hp = 20\n"
        "        .attack = 1\n"
        "        .defense = 1\n"
        "        .name = \"Tom\"\n"
        "    }\n"
        "}\n"
    );
    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;
    Zinc_expect_no_errors(errors);

    /* root */
    Cent_ast* root = ct->primary->pr.root;
    Zinc_assert_ptr(root, "ptr root");
    Zinc_expect_int_equal(root->type, Cent_ast_type_stmts, "type root");

    /* element type */
    Cent_ast* element = Cent_ast_get(root, 0);
    Zinc_assert_ptr(element, "ptr element");
    Zinc_expect_int_equal(element->type, Cent_ast_type_element_type, "type element");
    Zinc_expect_string(&element->text, "Monster", "text element");

    /* enum type */
    Cent_ast* enum_ = Cent_ast_get(root, 1);
    Zinc_assert_ptr(enum_, "ptr enum_");
    Zinc_expect_int_equal(enum_->type, Cent_ast_type_enum_type, "type enum_");
    Zinc_expect_string(&enum_->text, "MonsterKind", "text enum_");

    /* variant */
    Cent_ast* variant = Cent_ast_get(root, 2);
    Zinc_assert_ptr(variant, "ptr variant");
    Zinc_expect_int_equal(variant->type, Cent_ast_type_variant, "type variant");

    /* ns */
    Cent_ast* vns = Cent_ast_get(variant, 0);
    Zinc_assert_ptr(vns, "ptr vns");
    Zinc_expect_int_equal(vns->type, Cent_ast_type_namespace, "type vns");

    /* el */
    Cent_ast* el = Cent_ast_get(vns, 0);
    Zinc_assert_ptr(el, "ptr el");
    Zinc_expect_int_equal(el->type, Cent_ast_type_id, "type el");
    Zinc_expect_string(&el->text, "Monster", "text el");

    /* kind */
    Cent_ast* kind = Cent_ast_get(vns, 1);
    Zinc_assert_ptr(kind, "ptr kind");
    Zinc_expect_int_equal(kind->type, Cent_ast_type_id, "type kind");
    Zinc_expect_string(&kind->text, "Bat", "text kind");

    /* variant properties */
    Cent_ast* vnt_properties = Cent_ast_get(variant, 1);
    Zinc_assert_ptr(vnt_properties, "ptr vnt_properties");
    Zinc_expect_int_equal(vnt_properties->type, Cent_ast_type_prop, "type vnt_properties");

    Cent_ast* siphon_prop = Cent_ast_get(vnt_properties, 0);
    Zinc_assert_ptr(siphon_prop, "ptr siphon_prop");
    Zinc_expect_int_equal(siphon_prop->type, Cent_ast_type_prop_dec, "type siphon_prop");

    Cent_ast* siphon_id = Cent_ast_get(siphon_prop, 0);
    Zinc_assert_ptr(siphon_id, "ptr siphon");
    Zinc_expect_int_equal(siphon_id->type, Cent_ast_type_id, "type siphon");
    Zinc_expect_string(&siphon_id->text, "siphon", "text siphon");

    Cent_ast* siphon_type = Cent_ast_get(siphon_prop, 1);
    Zinc_assert_ptr(siphon_type, "ptr siphon_type");
    Zinc_expect_int_equal(siphon_type->type, Cent_ast_type_id, "type siphon_type");
    Zinc_expect_string(&siphon_type->text, "Integer", "text siphon_type");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void test_parse()
{
    test_parse_element();
    test_parse_enumerate();
    test_parse_top_level_assignment();
    test_parse_error_unhandled_token();
    test_parse_element_error_expected_id();
    test_parse_element_error_expected_left_curly_brace();
    test_parse_element_error_name_already_exits();
    test_parse_element_error_properties_expected_right_curly_brace();
    test_parse_element_error_property_expected_colon();
    test_parse_element_error_property_expected_id();
    test_parse_element_error_children_expected_right_curly_brace();
    test_parse_enumerate_error_expected_id();
    test_parse_enumerate_error_expected_right_curly_brace();
    test_parse_value_error_enum_expected_id();
    test_parse_value_error_object_expected_rcb();
    test_parse_value_error_object_property_expected_id();
    test_parse_value_error_object_property_expected_equal();
    test_parse_element_property_unknown_type();
    test_parse_element_property_type_not_element();
    test_parse_element_child_unknown_type();
    test_parse_element_child_type_not_an_element_type();
    test_parse_enum_error_duplicate_enum_value();
    test_parse_object_method_call();
    test_parse_object_method_call2();
    test_parse_function_call();
    test_parse_enum_duplicate_id();
    test_parse_enum_error_could_not_find_enum();
    test_parse_enum_error_could_not_find_enum_id();

    test_parse_include();
    test_parse_include_multiple_namespace();

    test_parse_include_value();
    test_parse_include_value_error();

    test_parse_include_glob();
    test_parse_include_error_expected_id();

    test_parse_namespace_error_expected_id();
    test_parse_namespace_error_expected_id2();

    test_parse_const();
    test_parse_const_error_shadow_type();
    test_parse_const_error_shadow_module();
    test_parse_const_error_shadow_local();

    test_parse_object_const();

    test_parse_module_id_error();

    test_parse_bad_id();
    test_parse_bad_id_child_of();
    test_parse_bad_id_property_of();

    test_parse_follow_on();
    test_parse_follow_on_error_no_previous_object();
    test_parse_follow_on_error_follow_on_is_to_non_object();
    test_parse_follow_on_error_follow_on_increased_level_greater_than_one();
    test_parse_follow_on_error_follow_on_increased_level_greater_than_one2();

    test_parse_element_tagged();

    test_parse_variant();
}
