#include <string.h>
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"
#include "centipede/comp_table.h"
#include "centipede/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CentUnit_parse_element(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_error_count(test, errors, 1);
    Zinc_expect_source_error(test, errors, "unknown type: Test");

    /* root */
    Zinc_assert_ptr(test, root, "ptr pr.root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type pr.root");

    /* element */
    Cent_ast* element = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, element, "ptr element");
    Zinc_expect_int_equal(test, element->type, Cent_ast_type_element_type, "type element");
    Zinc_expect_string(test, &element->text, "Test_suite", "value element");

    /* properties */
    Cent_ast* prop = Cent_ast_get(element, 0);
    Zinc_assert_ptr(test, prop, "ptr prop");
    Zinc_expect_int_equal(test, prop->type, Cent_ast_type_prop, "type prop");

    /* name */
    Cent_ast* dec0 = Cent_ast_get(prop, 0);
    Zinc_assert_ptr(test, dec0, "ptr dec0");
    Zinc_expect_int_equal(test, dec0->type, Cent_ast_type_prop_dec, "type dec0");

    Cent_ast* name0 = Cent_ast_get(dec0, 0);
    Zinc_assert_ptr(test, name0, "ptr name0");
    Zinc_expect_int_equal(test, name0->type, Cent_ast_type_id, "type name0");
    Zinc_expect_string(test, &name0->text, "name", "value name0");

    Cent_ast* type0 = Cent_ast_get(dec0, 1);
    Zinc_assert_ptr(test, type0, "ptr type0");
    Zinc_expect_int_equal(test, type0->type, Cent_ast_type_id, "type type0");
    Zinc_expect_string(test, &type0->text, "String", "value type0");

    Cent_ast* mod0 = Cent_ast_get(dec0, 2);
    Zinc_assert_ptr(test, mod0, "ptr mod0");
    Zinc_expect_int_equal(test, mod0->type, Cent_ast_type_modifier, "type mod0");
    Zinc_expect_string(test, &mod0->text, "required", "value mod0");

    /* solo */
    Cent_ast* dec1 = Cent_ast_get(prop, 1);
    Zinc_assert_ptr(test, dec1, "ptr dec1");
    Zinc_expect_int_equal(test, dec1->type, Cent_ast_type_prop_dec, "type dec1");

    Cent_ast* name1 = Cent_ast_get(dec1, 0);
    Zinc_assert_ptr(test, name1, "ptr name1");
    Zinc_expect_int_equal(test, name1->type, Cent_ast_type_id, "type name1");
    Zinc_expect_string(test, &name1->text, "solo", "value name1");

    Cent_ast* type1 = Cent_ast_get(dec1, 1);
    Zinc_assert_ptr(test, type1, "ptr type1");
    Zinc_expect_int_equal(test, type1->type, Cent_ast_type_id, "type type1");
    Zinc_expect_string(test, &type1->text, "Bool", "value type1");

    /* mute */
    Cent_ast* dec2 = Cent_ast_get(prop, 2);
    Zinc_assert_ptr(test, dec2, "ptr dec2");
    Zinc_expect_int_equal(test, dec2->type, Cent_ast_type_prop_dec, "type dec2");

    Cent_ast* name2 = Cent_ast_get(dec2, 0);
    Zinc_assert_ptr(test, name2, "ptr name2");
    Zinc_expect_int_equal(test, name2->type, Cent_ast_type_id, "type name2");
    Zinc_expect_string(test, &name2->text, "mute", "value name2");

    Cent_ast* type2 = Cent_ast_get(dec2, 1);
    Zinc_assert_ptr(test, type2, "ptr type2");
    Zinc_expect_int_equal(test, type2->type, Cent_ast_type_id, "type type2");
    Zinc_expect_string(test, &type2->text, "Bool", "value type2");

    /* children */
    Cent_ast* children = Cent_ast_get(element, 1);
    Zinc_assert_ptr(test, children, "ptr children");
    Zinc_expect_int_equal(test, children->type, Cent_ast_type_children, "type children");

    /* Test */
    Cent_ast* test_node = Cent_ast_get(children, 0);
    Zinc_assert_ptr(test, test_node, "ptr test");
    Zinc_expect_int_equal(test, test_node->type, Cent_ast_type_id, "type test");
    Zinc_expect_string(test, &test_node->text, "Test", "value test");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_enumerate(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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
    Zinc_expect_no_errors(test, errors);
    Zinc_assert_ptr(test, root, "ptr pr.root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type pr.root");

    /* enumerate */
    Cent_ast* enumerate = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, enumerate, "ptr enumerate");
    Zinc_expect_int_equal(test, enumerate->type, Cent_ast_type_enum_type, "type enumerate");
    Zinc_expect_string(test, &enumerate->text, "Symbol_type", "value enumerate");

    /* Variable */
    Cent_ast* value0 = Cent_ast_get(enumerate, 0);
    Zinc_assert_ptr(test, value0, "ptr value0");
    Zinc_expect_int_equal(test, value0->type, Cent_ast_type_id, "type value0");
    Zinc_expect_string(test, &value0->text, "Variable", "value value0");

    /* Type */
    Cent_ast* value1 = Cent_ast_get(enumerate, 1);
    Zinc_assert_ptr(test, value1, "ptr value1");
    Zinc_expect_int_equal(test, value1->type, Cent_ast_type_id, "type value1");
    Zinc_expect_string(test, &value1->text, "Type", "value value1");

    /* Info */
    Cent_ast* value2 = Cent_ast_get(enumerate, 2);
    Zinc_assert_ptr(test, value2, "ptr value2");
    Zinc_expect_int_equal(test, value2->type, Cent_ast_type_id, "type value2");
    Zinc_expect_string(test, &value2->text, "Info", "value value2");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_top_level_assignment(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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
    Zinc_expect_no_errors(test, errors);
    Zinc_assert_ptr(test, root, "ptr pr.root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type pr.root");

    /* enum type */
    Cent_ast* enum_type = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, enum_type, "ptr enum_type");
    Zinc_expect_int_equal(test, enum_type->type, Cent_ast_type_enum_type, "type enum_type");
    Zinc_expect_string(test, &enum_type->text, "Type_def_type", "text enum_type");

    /* const */
    Cent_ast* const_ = Cent_ast_get(root, 1);
    Zinc_assert_ptr(test, const_, "ptr const_");
    Zinc_expect_int_equal(test, const_->type, Cent_ast_type_const, "type const_");

    /* name */
    Cent_ast* name = Cent_ast_get(const_, 0);
    Zinc_assert_ptr(test, name, "ptr name");
    Zinc_expect_int_equal(test, name->type, Cent_ast_type_id, "type name");
    Zinc_expect_string(test, &name->text, "i32", "value name");

    /* object */
    Cent_ast* object = Cent_ast_get(const_, 1);
    Zinc_assert_ptr(test, object, "ptr object");
    Zinc_expect_int_equal(test, object->type, Cent_ast_type_expr_object, "type object");

    /* type prop set */
    Cent_ast* type_prop_set = Cent_ast_get(object, 0);
    Zinc_assert_ptr(test, type_prop_set, "ptr type_prop_set");
    Zinc_expect_int_equal(test, type_prop_set->type, Cent_ast_type_prop_set, "type type_prop_set");

    /* type */
    Cent_ast* type = Cent_ast_get(type_prop_set, 0);
    Zinc_assert_ptr(test, type, "ptr type");
    Zinc_expect_int_equal(test, type->type, Cent_ast_type_id, "type type");
    Zinc_expect_string(test, &type->text, "type", "value type");

    /* enum namespace */
    Cent_ast* ns = Cent_ast_get(type_prop_set, 1);
    Zinc_assert_ptr(test, ns, "ptr ns");
    Zinc_expect_int_equal(test, ns->type, Cent_ast_type_namespace, "type ns");

    /* enum id */
    Cent_ast* enum_id = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(test, enum_id, "ptr enum_id");
    Zinc_expect_int_equal(test, enum_id->type, Cent_ast_type_id, "type enum_id");
    Zinc_expect_string(test, &enum_id->text, "Type_def_type", "text enum_id");

    /* enum value */
    Cent_ast* enum_value_id = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(test, enum_value_id, "ptr enum_value_id");
    Zinc_expect_int_equal(test, enum_value_id->type, Cent_ast_type_id, "type enum_value_id");
    Zinc_expect_string(test, &enum_value_id->text, "Integer", "text enum_value_id");

    /* name prop set */
    Cent_ast* name_prop_set = Cent_ast_get(object, 1);
    Zinc_assert_ptr(test, name_prop_set, "ptr name_prop_set");
    Zinc_expect_int_equal(test, name_prop_set->type, Cent_ast_type_prop_set, "type name_prop_set");

    /* name */
    Cent_ast* name1 = Cent_ast_get(name_prop_set, 0);
    Zinc_assert_ptr(test, name1, "ptr type");
    Zinc_expect_int_equal(test, name1->type, Cent_ast_type_id, "type name1");
    Zinc_expect_string(test, &name1->text, "name", "text name");

    /* name value */
    Cent_ast* name_value = Cent_ast_get(name_prop_set, 1);
    Zinc_assert_ptr(test, name_value, "ptr name_value");
    Zinc_expect_int_equal(test, name_value->type, Cent_ast_type_expr_string, "type name_value");
    Zinc_expect_int_equal(test, name_value->value_type, Cent_value_type_string, "value_type name_value");
    Zinc_expect_string(test, &name_value->data.string, "i32", "string name_value");

    /* bit_count prop set */
    Cent_ast* bit_count_prop_set = Cent_ast_get(object, 2);
    Zinc_assert_ptr(test, bit_count_prop_set, "ptr bit_count_prop_set");
    Zinc_expect_int_equal(test, bit_count_prop_set->type, Cent_ast_type_prop_set, "type bit_count_prop_set");

    /* bit_count */
    Cent_ast* bit_count = Cent_ast_get(bit_count_prop_set, 0);
    Zinc_assert_ptr(test, bit_count, "ptr bit_count");
    Zinc_expect_int_equal(test, bit_count->type, Cent_ast_type_id, "type bit_count");
    Zinc_expect_string(test, &bit_count->text, "bit_count", "text bit_count");

    /* bit_count value */
    Cent_ast* bit_count_value = Cent_ast_get(bit_count_prop_set, 1);
    Zinc_assert_ptr(test, bit_count_value, "ptr bit_count_value");
    Zinc_expect_int_equal(test, bit_count_value->type, Cent_ast_type_expr_natural, "type bit_count_value");
    Zinc_expect_int_equal(test, bit_count_value->value_type, Cent_value_type_natural, "value_type bit_count_value");
    Zinc_expect_uint64_t_equal(test, bit_count_value->data.natural, 32, "integer bit_count_value");

    /* is_signed prop set */
    Cent_ast* is_signed_prop_set = Cent_ast_get(object, 3);
    Zinc_assert_ptr(test, is_signed_prop_set, "ptr is_signed_prop_set");
    Zinc_expect_int_equal(test, is_signed_prop_set->type, Cent_ast_type_prop_set, "type is_signed_prop_set");

    /* is_signed */
    Cent_ast* is_signed = Cent_ast_get(is_signed_prop_set, 0);
    Zinc_assert_ptr(test, is_signed, "ptr is_signed");
    Zinc_expect_int_equal(test, is_signed->type, Cent_ast_type_id, "type is_signed");
    Zinc_expect_string(test, &is_signed->text, "is_signed", "text is_signed");

    /* is_signed value */
    Cent_ast* is_signed_value = Cent_ast_get(is_signed_prop_set, 1);
    Zinc_assert_ptr(test, is_signed_value, "ptr is_signed_value");
    Zinc_expect_int_equal(test, is_signed_value->type, Cent_ast_type_expr_boolean, "type is_signed_value");
    Zinc_expect_int_equal(test, is_signed_value->value_type, Cent_value_type_boolean, "value_type is_signed_value");
    Zinc_expect_long_long_equal(test, is_signed_value->data.boolean, true, "boolean is_signed_value");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_error_unhandled_token(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "1 }");

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "unhandled token: right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_error_expected_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct, "element 1");

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_error_expected_left_curly_brace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected left-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_error_name_already_exits(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "}\n"
        "element Test {\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "name already exists: Test");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_error_properties_expected_right_curly_brace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "    properties {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_error_property_expected_colon(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected colon");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_error_property_expected_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_error_children_expected_right_curly_brace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Test {\n"
        "    children {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_enumerate_error_expected_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum {\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_enumerate_error_expected_right_curly_brace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Ast_type {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_value_error_enum_expected_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "enum Test_type {\n"
        "}\n"
        "Test_type::1\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_value_error_object_expected_rcb(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
    );

    Cent_comp_unit_parse(ct->primary);
    Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected right-curly-brace");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_value_error_object_property_expected_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
        "   . = 1\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_value_error_object_property_expected_equal(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "Test {\n"
        "   .a 1\n"
        "}\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected equal");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_property_unknown_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "unknown type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_property_type_not_element(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "type is not an element or enum type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_child_unknown_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "unknown type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_child_type_not_an_element_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "type is not an element type: Abc");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_enum_error_duplicate_enum_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "duplicate enum value: Symbol_type::Info");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_enum_duplicate_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    struct Zinc_error* e = Zinc_expect_source_error(test, errors, "duplicate enum value: Foo::Two");
    Zinc_assert_ptr(test, e, "ptr e");
    Zinc_expect_size_t_equal(test, e->loc.line, 4, "line e");
    Zinc_expect_size_t_equal(test, e->loc.col, 5, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_enum_error_could_not_find_enum(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    struct Zinc_error* e = Zinc_expect_source_error(test, errors, "id is not a variable: Bar");
    Zinc_assert_ptr(test, e, "ptr e");
    Zinc_expect_size_t_equal(test, e->loc.line, 6, "line e");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_enum_error_could_not_find_enum_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    struct Zinc_error* e = Zinc_expect_source_error(test, errors, "invalid enum id: Four");
    Zinc_assert_ptr(test, e, "ptr e");
    Zinc_expect_size_t_equal(test, e->loc.line, 6, "line e");
    Zinc_expect_size_t_equal(test, e->loc.col, 6, "col e");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_include(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_no_errors(test, errors);

    /* root */
    Zinc_assert_ptr(test, root, "ptr pr.root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type pr.root");

    /* use */
    Cent_ast* use = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, use, "ptr use");
    Zinc_expect_int_equal(test, use->type, Cent_ast_type_use, "type use");

    Cent_ast* module = Cent_ast_get(use, 0);
    Zinc_assert_ptr(test, module, "ptr module");
    Zinc_expect_int_equal(test, module->type, Cent_ast_type_id, "type module");
    Zinc_expect_string(test, &module->text, "types", "text module");

    /* Groceries object */
    Cent_ast* object = Cent_ast_get(root, 1);
    Zinc_assert_ptr(test, object, "ptr object");
    Zinc_expect_int_equal(test, object->type, Cent_ast_type_expr_object, "type object");
    Zinc_expect_string(test, &object->text, "Groceries", "text object");

    /* namespace */
    Cent_ast* ns = Cent_ast_get(object, 0);
    Zinc_assert_ptr(test, ns, "ptr ns");
    Zinc_expect_int_equal(test, ns->type, Cent_ast_type_namespace, "type ns");

    Cent_ast* types = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(test, types, "ptr types");
    Zinc_expect_int_equal(test, types->type, Cent_ast_type_id, "type types");
    Zinc_expect_string(test, &types->text, "types", "text types");

    Cent_ast* grocery_item = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(test, grocery_item, "ptr grocery_item");
    Zinc_expect_int_equal(test, grocery_item->type, Cent_ast_type_id, "type grocery_item");
    Zinc_expect_string(test, &grocery_item->text, "Grocery_item", "text grocery_item");

    Cent_ast* milk = Cent_ast_get(ns, 2);
    Zinc_assert_ptr(test, milk, "ptr milk");
    Zinc_expect_int_equal(test, milk->type, Cent_ast_type_id, "type milk");
    Zinc_expect_string(test, &milk->text, "Milk", "text milk");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_include_multiple_namespace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_no_errors(test, errors);

    /* root */
    Zinc_assert_ptr(test, root, "ptr pr.root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type pr.root");

    /* use */
    Cent_ast* use = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, use, "ptr use");
    Zinc_expect_int_equal(test, use->type, Cent_ast_type_use, "type use");

    Cent_ast* lib_mod = Cent_ast_get(use, 0);
    Zinc_assert_ptr(test, lib_mod, "ptr lib_mod");
    Zinc_expect_int_equal(test, lib_mod->type, Cent_ast_type_id, "type lib_mod");
    Zinc_expect_string(test, &lib_mod->text, "lib", "text lib_mod");

    Cent_ast* types_mod = Cent_ast_get(use, 1);
    Zinc_assert_ptr(test, types_mod, "ptr types_mod");
    Zinc_expect_int_equal(test, types_mod->type, Cent_ast_type_id, "type types_mod");
    Zinc_expect_string(test, &types_mod->text, "types", "text types_mod");

    /* Groceries object */
    Cent_ast* object = Cent_ast_get(root, 1);
    Zinc_assert_ptr(test, object, "ptr object");
    Zinc_expect_int_equal(test, object->type, Cent_ast_type_expr_object, "type object");
    Zinc_expect_string(test, &object->text, "Groceries", "text object");

    /* namespace */
    Cent_ast* ns = Cent_ast_get(object, 0);
    Zinc_assert_ptr(test, ns, "ptr ns");
    Zinc_expect_int_equal(test, ns->type, Cent_ast_type_namespace, "type ns");

    Cent_ast* lib = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(test, lib, "ptr lib");
    Zinc_expect_int_equal(test, lib->type, Cent_ast_type_id, "type lib");
    Zinc_expect_string(test, &lib->text, "lib", "text lib");

    Cent_ast* types = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(test, types, "ptr types");
    Zinc_expect_int_equal(test, types->type, Cent_ast_type_id, "type types");
    Zinc_expect_string(test, &types->text, "types", "text types");

    Cent_ast* grocery_item = Cent_ast_get(ns, 2);
    Zinc_assert_ptr(test, grocery_item, "ptr grocery_item");
    Zinc_expect_int_equal(test, grocery_item->type, Cent_ast_type_id, "type grocery_item");
    Zinc_expect_string(test, &grocery_item->text, "Grocery_item", "text grocery_item");

    Cent_ast* milk = Cent_ast_get(ns, 3);
    Zinc_assert_ptr(test, milk, "ptr milk");
    Zinc_expect_int_equal(test, milk->type, Cent_ast_type_id, "type milk");
    Zinc_expect_string(test, &milk->text, "Milk", "text milk");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_include_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_no_errors(test, errors);

    /* root */
    Zinc_assert_ptr(test, root, "ptr pr.root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type pr.root");

    /* use */
    Cent_ast* use = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, use, "ptr use");
    Zinc_expect_int_equal(test, use->type, Cent_ast_type_use, "type use");

    Cent_ast* data_mod = Cent_ast_get(use, 0);
    Zinc_assert_ptr(test, data_mod, "ptr data_mod");
    Zinc_expect_int_equal(test, data_mod->type, Cent_ast_type_id, "type data_mod");
    Zinc_expect_string(test, &data_mod->text, "data", "text data_mod");

    /* namespace */
    Cent_ast* ns = Cent_ast_get(root, 1);
    Zinc_assert_ptr(test, ns, "ptr ns");
    Zinc_expect_int_equal(test, ns->type, Cent_ast_type_namespace, "type ns");

    Cent_ast* data = Cent_ast_get(ns, 0);
    Zinc_assert_ptr(test, data, "ptr data");
    Zinc_expect_int_equal(test, data->type, Cent_ast_type_id, "type data");
    Zinc_expect_string(test, &data->text, "data", "text data");

    Cent_ast* a = Cent_ast_get(ns, 1);
    Zinc_assert_ptr(test, a, "ptr a");
    Zinc_expect_int_equal(test, a->type, Cent_ast_type_id, "type a");
    Zinc_expect_string(test, &a->text, "a", "text a");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_include_value_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "not a valid id: b");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_include_glob(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_no_errors(test, errors);

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_include_error_expected_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_namespace_error_expected_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_namespace_error_expected_id2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "expected id");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_const(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 45;\n"
        "a\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;
    Cent_ast* root = ct->primary->pr.root;
    
    Zinc_expect_no_errors(test, errors);

    Zinc_assert_ptr(test, root, "ptr root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type root");

    Cent_ast* const_ = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, const_, "ptr const_");
    Zinc_expect_int_equal(test, const_->type, Cent_ast_type_const, "type const_");

    Cent_ast* id = Cent_ast_get(const_, 0);
    Zinc_assert_ptr(test, id, "ptr id");
    Zinc_expect_int_equal(test, id->type, Cent_ast_type_id, "type id");
    Zinc_expect_string(test, &id->text, "a", "text id");

    Cent_ast* number = Cent_ast_get(const_, 1);
    Zinc_assert_ptr(test, number, "ptr number");
    Zinc_expect_int_equal(test, number->type, Cent_ast_type_expr_natural, "type number");
    Zinc_expect_uint64_t_equal(test, number->data.natural, 45, "integer number");

    Cent_ast* variable = Cent_ast_get(root, 1);
    Zinc_assert_ptr(test, variable, "ptr variable");
    Zinc_expect_int_equal(test, variable->type, Cent_ast_type_expr_variable, "type variable");
    Zinc_expect_string(test, &variable->text, "a", "text variable");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_const_error_shadow_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "element Foo {\n"
        "}\n"
        "const Foo = 45;\n"
        "Foo\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "shadowing of type: Foo");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_const_error_shadow_module(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "use base;\n"
        "const base = 45;\n"
        "base\n"
    );

    Cent_comp_table_add_module_str(ct, "base.cent", "");

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "shadowing of module: base");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_const_error_shadow_local(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 1;\n"
        "const a = 2;\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "shadowing of local variable: a");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_bad_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_comp_table* ct = NULL;
    Cent_comp_table_create_str(&ct,
        "const a = 1;\n"
        "b\n"
    );

    Cent_comp_unit_parse(ct->primary);
    struct Zinc_error_list* errors = &ct->primary->errors;

    Zinc_expect_has_errors(test, errors);
    Zinc_expect_source_error(test, errors, "unknown variable: b");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_module_id_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_has_errors(test, errors);

    Zinc_expect_source_error(test, errors, "module identifier collides with existing identifier: math");
    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_element_tagged(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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
    Zinc_expect_no_errors(test, errors);

    /* root */
    Cent_ast* root = ct->primary->pr.root;
    Zinc_assert_ptr(test, root, "ptr root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type root");

    /* element type */
    Cent_ast* element = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, element, "ptr element");
    Zinc_expect_int_equal(test, element->type, Cent_ast_type_element_type, "type element");
    Zinc_expect_string(test, &element->text, "Vehicle", "text element");

    /* enum type */
    Cent_ast* enum_ = Cent_ast_get(root, 1);
    Zinc_assert_ptr(test, enum_, "ptr enum_");
    Zinc_expect_int_equal(test, enum_->type, Cent_ast_type_enum_type, "type enum_");
    Zinc_expect_string(test, &enum_->text, "Vehicle_type", "text enum_");

    /* Car */
    Cent_ast* item0 = Cent_ast_get(enum_, 0);
    Zinc_assert_ptr(test, item0, "ptr item0");
    Zinc_expect_int_equal(test, item0->type, Cent_ast_type_id, "type item0");
    Zinc_expect_string(test, &item0->text, "Car", "text item0");

    /* Truck */
    Cent_ast* item1 = Cent_ast_get(enum_, 1);
    Zinc_assert_ptr(test, item1, "ptr item1");
    Zinc_expect_int_equal(test, item1->type, Cent_ast_type_id, "type item1");
    Zinc_expect_string(test, &item1->text, "Truck", "text item1");
    
    /* Van */
    Cent_ast* item2 = Cent_ast_get(enum_, 2);
    Zinc_assert_ptr(test, item2, "ptr item2");
    Zinc_expect_int_equal(test, item2->type, Cent_ast_type_id, "type item2");
    Zinc_expect_string(test, &item2->text, "Van", "text item2");

    /* Bike */
    Cent_ast* item3 = Cent_ast_get(enum_, 3);
    Zinc_assert_ptr(test, item3, "ptr item3");
    Zinc_expect_int_equal(test, item3->type, Cent_ast_type_id, "type item3");
    Zinc_expect_string(test, &item3->text, "Bike", "text item3");

    /* object car */
    Cent_ast* car = Cent_ast_get(root, 2);
    Zinc_assert_ptr(test, car, "ptr car");
    Zinc_expect_int_equal(test, car->type, Cent_ast_type_expr_object, "type car");
    Zinc_expect_string(test, &car->text, "Vehicle", "text car");

    /* car variant set */
    Cent_ast* car_var_set = Cent_ast_get(car, 0);
    Zinc_assert_ptr(test, car_var_set, "ptr car_var_set");
    Zinc_expect_int_equal(test, car_var_set->type, Cent_ast_type_variant_set, "type car_var_set");

    /* car @tag */
    Cent_ast* car_tag = Cent_ast_get(car, 1);
    Zinc_assert_ptr(test, car_tag, "ptr car_tag");
    Zinc_expect_int_equal(test, car_tag->type, Cent_ast_type_prop_set, "type car_tag");
    
    /* car @tag name */
    Cent_ast* car_tag_name = Cent_ast_get(car_tag, 0);
    Zinc_assert_ptr(test, car_tag_name, "ptr car_tag_name");
    Zinc_expect_int_equal(test, car_tag_name->type, Cent_ast_type_id, "type, car_tag_name");
    Zinc_expect_string(test, &car_tag_name->text, "@tag", "text car_tag_name");

    /* car @tag namespace */
    Cent_ast* car_tag_ns = Cent_ast_get(car_tag, 1);
    Zinc_assert_ptr(test, car_tag_ns, "ptr car_tag_ns");
    Zinc_expect_int_equal(test, car_tag_ns->type, Cent_ast_type_namespace, "type, car_tag_ns");
    Zinc_expect_int_equal(test, car_tag_ns->value_type, Cent_value_type_enum, "value_type car_tag_ns");
    Zinc_expect_size_t_equal(test, car_tag_ns->data.enumeration.number, 0, "enumeration number car_tag_ns");

    /* car @tag namespace id1 */
    Cent_ast* car_tag_ns_id1 = Cent_ast_get(car_tag_ns, 0);
    Zinc_assert_ptr(test, car_tag_ns_id1, "ptr car_tag_ns_id1");
    Zinc_expect_int_equal(test, car_tag_ns_id1->type, Cent_ast_type_id, "type car_tag_ns_id1");
    Zinc_expect_string(test, &car_tag_ns_id1->text, "Vehicle_type", "text car_tag_ns_id1");

    /* car @tag namespace id2 */
    Cent_ast* car_tag_ns_id2 = Cent_ast_get(car_tag_ns, 1);
    Zinc_assert_ptr(test, car_tag_ns_id2, "ptr car_tag_ns_id2");
    Zinc_expect_int_equal(test, car_tag_ns_id2->type, Cent_ast_type_id, "type car_tag_ns_id2");
    Zinc_expect_string(test, &car_tag_ns_id2->text, "Car", "text car_tag_ns_id2");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CentUnit_parse_variant(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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
    Zinc_expect_no_errors(test, errors);

    /* root */
    Cent_ast* root = ct->primary->pr.root;
    Zinc_assert_ptr(test, root, "ptr root");
    Zinc_expect_int_equal(test, root->type, Cent_ast_type_stmts, "type root");

    /* element type */
    Cent_ast* element = Cent_ast_get(root, 0);
    Zinc_assert_ptr(test, element, "ptr element");
    Zinc_expect_int_equal(test, element->type, Cent_ast_type_element_type, "type element");
    Zinc_expect_string(test, &element->text, "Monster", "text element");

    /* enum type */
    Cent_ast* enum_ = Cent_ast_get(root, 1);
    Zinc_assert_ptr(test, enum_, "ptr enum_");
    Zinc_expect_int_equal(test, enum_->type, Cent_ast_type_enum_type, "type enum_");
    Zinc_expect_string(test, &enum_->text, "MonsterKind", "text enum_");

    /* variant */
    Cent_ast* variant = Cent_ast_get(root, 2);
    Zinc_assert_ptr(test, variant, "ptr variant");
    Zinc_expect_int_equal(test, variant->type, Cent_ast_type_variant, "type variant");

    /* ns */
    Cent_ast* vns = Cent_ast_get(variant, 0);
    Zinc_assert_ptr(test, vns, "ptr vns");
    Zinc_expect_int_equal(test, vns->type, Cent_ast_type_namespace, "type vns");

    /* el */
    Cent_ast* el = Cent_ast_get(vns, 0);
    Zinc_assert_ptr(test, el, "ptr el");
    Zinc_expect_int_equal(test, el->type, Cent_ast_type_id, "type el");
    Zinc_expect_string(test, &el->text, "Monster", "text el");

    /* kind */
    Cent_ast* kind = Cent_ast_get(vns, 1);
    Zinc_assert_ptr(test, kind, "ptr kind");
    Zinc_expect_int_equal(test, kind->type, Cent_ast_type_id, "type kind");
    Zinc_expect_string(test, &kind->text, "Bat", "text kind");

    /* variant properties */
    Cent_ast* vnt_properties = Cent_ast_get(variant, 1);
    Zinc_assert_ptr(test, vnt_properties, "ptr vnt_properties");
    Zinc_expect_int_equal(test, vnt_properties->type, Cent_ast_type_prop, "type vnt_properties");

    Cent_ast* siphon_prop = Cent_ast_get(vnt_properties, 0);
    Zinc_assert_ptr(test, siphon_prop, "ptr siphon_prop");
    Zinc_expect_int_equal(test, siphon_prop->type, Cent_ast_type_prop_dec, "type siphon_prop");

    Cent_ast* siphon_id = Cent_ast_get(siphon_prop, 0);
    Zinc_assert_ptr(test, siphon_id, "ptr siphon");
    Zinc_expect_int_equal(test, siphon_id->type, Cent_ast_type_id, "type siphon");
    Zinc_expect_string(test, &siphon_id->text, "siphon", "text siphon");

    Cent_ast* siphon_type = Cent_ast_get(siphon_prop, 1);
    Zinc_assert_ptr(test, siphon_type, "ptr siphon_type");
    Zinc_expect_int_equal(test, siphon_type->type, Cent_ast_type_id, "type siphon_type");
    Zinc_expect_string(test, &siphon_type->text, "Integer", "text siphon_type");

    Cent_comp_table_destroy(ct);
    free(ct);
}

void CoverUnit_parse(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CentUnit_parse_element);
        Zinc_test_register(test, CentUnit_parse_enumerate);
        Zinc_test_register(test, CentUnit_parse_top_level_assignment);
        Zinc_test_register(test, CentUnit_parse_error_unhandled_token);
        Zinc_test_register(test, CentUnit_parse_element_error_expected_id);
        Zinc_test_register(test, CentUnit_parse_element_error_expected_left_curly_brace);
        Zinc_test_register(test, CentUnit_parse_element_error_name_already_exits);
        Zinc_test_register(test, CentUnit_parse_element_error_properties_expected_right_curly_brace);
        Zinc_test_register(test, CentUnit_parse_element_error_property_expected_colon);
        Zinc_test_register(test, CentUnit_parse_element_error_property_expected_id);
        Zinc_test_register(test, CentUnit_parse_element_error_children_expected_right_curly_brace);
        Zinc_test_register(test, CentUnit_parse_enumerate_error_expected_id);
        Zinc_test_register(test, CentUnit_parse_enumerate_error_expected_right_curly_brace);
        Zinc_test_register(test, CentUnit_parse_value_error_enum_expected_id);
        Zinc_test_register(test, CentUnit_parse_value_error_object_expected_rcb);
        Zinc_test_register(test, CentUnit_parse_value_error_object_property_expected_id);
        Zinc_test_register(test, CentUnit_parse_value_error_object_property_expected_equal);
        Zinc_test_register(test, CentUnit_parse_element_property_unknown_type);
        Zinc_test_register(test, CentUnit_parse_element_property_type_not_element);
        Zinc_test_register(test, CentUnit_parse_element_child_unknown_type);
        Zinc_test_register(test, CentUnit_parse_element_child_type_not_an_element_type);
        Zinc_test_register(test, CentUnit_parse_enum_error_duplicate_enum_value);
        Zinc_test_register(test, CentUnit_parse_enum_duplicate_id);
        Zinc_test_register(test, CentUnit_parse_enum_error_could_not_find_enum);
        Zinc_test_register(test, CentUnit_parse_enum_error_could_not_find_enum_id);

        Zinc_test_register(test, CentUnit_parse_include);
        Zinc_test_register(test, CentUnit_parse_include_multiple_namespace);

        Zinc_test_register(test, CentUnit_parse_include_value);
        Zinc_test_register(test, CentUnit_parse_include_value_error);

        Zinc_test_register(test, CentUnit_parse_include_glob);
        Zinc_test_register(test, CentUnit_parse_include_error_expected_id);

        Zinc_test_register(test, CentUnit_parse_namespace_error_expected_id);
        Zinc_test_register(test, CentUnit_parse_namespace_error_expected_id2);

        Zinc_test_register(test, CentUnit_parse_const);
        Zinc_test_register(test, CentUnit_parse_const_error_shadow_type);
        Zinc_test_register(test, CentUnit_parse_const_error_shadow_module);
        Zinc_test_register(test, CentUnit_parse_const_error_shadow_local);

        Zinc_test_register(test, CentUnit_parse_module_id_error);

        Zinc_test_register(test, CentUnit_parse_bad_id);

        Zinc_test_register(test, CentUnit_parse_element_tagged);

        Zinc_test_register(test, CentUnit_parse_variant);

        return;
    }

    Zinc_test_perform(test);
}
