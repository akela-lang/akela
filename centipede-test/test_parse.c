#include <string.h>
#include <string.h>
#include <zinc/error_unit_test.h>

#include "zinc/unit_test.h"
#include "centipede/parse.h"
#include "centipede/parse_data.h"
#include "zinc/input_unicode_string.h"
#include "zinc/error.h"
#include "test_parse_tools.h"

void test_parse_element()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
        "element Test_suite\n"
        "    properties\n"
        "        name: String `required`\n"
        "        solo: Bool      # only run this suite\n"
        "        mute: Bool      # exclude this suite from running\n"
        "    end\n"
        "    children\n"
        "        Test\n"
        "    end\n"
        "end\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);
    expect_error_count(pd.errors, 1);
    expect_source_error(pr.errors, "unknown type: Test");

    /* root */
    assert_ptr(pr.root, "ptr pr.root");
    expect_int_equal(pr.root->type, Cent_ast_type_stmts, "type pr.root");

    /* element */
    Cent_ast* element = Cent_ast_get(pr.root, 0);
    assert_ptr(element, "ptr element");
    expect_int_equal(element->type, Cent_ast_type_element_type, "type element");
    expect_str(&element->text, "Test_suite", "value element");

    /* properties */
    Cent_ast* prop = Cent_ast_get(element, 0);
    assert_ptr(prop, "ptr prop");
    expect_int_equal(prop->type, Cent_ast_type_prop, "type prop");

    /* name */
    Cent_ast* dec0 = Cent_ast_get(prop, 0);
    assert_ptr(dec0, "ptr dec0");
    expect_int_equal(dec0->type, Cent_ast_type_prop_dec, "type dec0");

    Cent_ast* name0 = Cent_ast_get(dec0, 0);
    assert_ptr(name0, "ptr name0");
    expect_int_equal(name0->type, Cent_ast_type_id, "type name0");
    expect_str(&name0->text, "name", "value name0");

    Cent_ast* type0 = Cent_ast_get(dec0, 1);
    assert_ptr(type0, "ptr type0");
    expect_int_equal(type0->type, Cent_ast_type_id, "type type0");
    expect_str(&type0->text, "String", "value type0");

    Cent_ast* mod0 = Cent_ast_get(dec0, 2);
    assert_ptr(mod0, "ptr mod0");
    expect_int_equal(mod0->type, Cent_ast_type_modifier, "type mod0");
    expect_str(&mod0->text, "required", "value mod0");

    /* solo */
    Cent_ast* dec1 = Cent_ast_get(prop, 1);
    assert_ptr(dec1, "ptr dec1");
    expect_int_equal(dec1->type, Cent_ast_type_prop_dec, "type dec1");

    Cent_ast* name1 = Cent_ast_get(dec1, 0);
    assert_ptr(name1, "ptr name1");
    expect_int_equal(name1->type, Cent_ast_type_id, "type name1");
    expect_str(&name1->text, "solo", "value name1");

    Cent_ast* type1 = Cent_ast_get(dec1, 1);
    assert_ptr(type1, "ptr type1");
    expect_int_equal(type1->type, Cent_ast_type_id, "type type1");
    expect_str(&type1->text, "Bool", "value type1");

    /* mute */
    Cent_ast* dec2 = Cent_ast_get(prop, 2);
    assert_ptr(dec2, "ptr dec2");
    expect_int_equal(dec2->type, Cent_ast_type_prop_dec, "type dec2");

    Cent_ast* name2 = Cent_ast_get(dec2, 0);
    assert_ptr(name2, "ptr name2");
    expect_int_equal(name2->type, Cent_ast_type_id, "type name2");
    expect_str(&name2->text, "mute", "value name2");

    Cent_ast* type2 = Cent_ast_get(dec2, 1);
    assert_ptr(type2, "ptr type2");
    expect_int_equal(type2->type, Cent_ast_type_id, "type type2");
    expect_str(&type2->text, "Bool", "value type2");

    /* children */
    Cent_ast* children = Cent_ast_get(element, 1);
    assert_ptr(children, "ptr children");
    expect_int_equal(children->type, Cent_ast_type_children, "type children");

    /* Test */
    Cent_ast* test = Cent_ast_get(children, 0);
    assert_ptr(test, "ptr test");
    expect_int_equal(test->type, Cent_ast_type_id, "type test");
    expect_str(&test->text, "Test", "value test");

    test_parse_teardown(&pd, &pr);
}

void test_parse_enumerate()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
    "enum Symbol_type\n"
    "    Variable\n"
    "    Type\n"
    "    Info\n"
    "end\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);

    /* root */
    expect_no_errors(pr.errors);
    assert_ptr(pr.root, "ptr pr.root");
    expect_int_equal(pr.root->type, Cent_ast_type_stmts, "type pr.root");

    /* enumerate */
    Cent_ast* enumerate = Cent_ast_get(pr.root, 0);
    assert_ptr(enumerate, "ptr enumerate");
    expect_int_equal(enumerate->type, Cent_ast_type_enumerate, "type enumerate");
    expect_str(&enumerate->text, "Symbol_type", "value enumerate");

    /* Variable */
    Cent_ast* value0 = Cent_ast_get(enumerate, 0);
    assert_ptr(value0, "ptr value0");
    expect_int_equal(value0->type, Cent_ast_type_id, "type value0");
    expect_str(&value0->text, "Variable", "value value0");

    /* Type */
    Cent_ast* value1 = Cent_ast_get(enumerate, 1);
    assert_ptr(value1, "ptr value1");
    expect_int_equal(value1->type, Cent_ast_type_id, "type value1");
    expect_str(&value1->text, "Type", "value value1");

    /* Info */
    Cent_ast* value2 = Cent_ast_get(enumerate, 2);
    assert_ptr(value2, "ptr value2");
    expect_int_equal(value2->type, Cent_ast_type_id, "type value2");
    expect_str(&value2->text, "Info", "value value2");

    test_parse_teardown(&pd, &pr);
}

void test_parse_top_level_assignment()
{
    test_name(__func__);

    Cent_parse_data pd;
    test_parse_setup(&pd,
    "# built-in element defs\n"
    "i32 = Type_def {\n"
    "    .type = Type_def_type::Integer\n"
    "    .name = \"i32\"\n"
    "    .bit_count = 32\n"
    "    .is_signed = true\n"
    "}\n"
    );

    Cent_parse_result pr = Cent_parse(&pd);

    /* root */
    expect_no_errors(pr.errors);
    assert_ptr(pr.root, "ptr pr.root");
    expect_int_equal(pr.root->type, Cent_ast_type_stmts, "type pr.root");

    /* assign */
    Cent_ast* assign = Cent_ast_get(pr.root, 0);
    assert_ptr(assign, "ptr var");
    expect_int_equal(assign->type, Cent_ast_type_assign, "type assign");

    /* name */
    Cent_ast* name = Cent_ast_get(assign, 0);
    assert_ptr(name, "ptr name");
    expect_int_equal(name->type, Cent_ast_type_id, "type name");
    expect_str(&name->text, "i32", "value name");

    /* value */
    Cent_ast* value = Cent_ast_get(assign, 1);
    assert_ptr(value, "ptr value");
    expect_int_equal(value->type, Cent_ast_type_value, "type value");

    /* object stmts */
    Cent_ast* object_stmts = Cent_ast_get(value, 0);
    assert_ptr(object_stmts, "ptr object_stmts");
    expect_int_equal(object_stmts->type, Cent_ast_type_object_stmts, "type object_stmts");

    /* type prop set */
    Cent_ast* type_prop_set = Cent_ast_get(object_stmts, 0);
    assert_ptr(type_prop_set, "ptr type_prop_set");
    expect_int_equal(type_prop_set->type, Cent_ast_type_prop_set, "type type_prop_set");

    /* type */
    Cent_ast* type = Cent_ast_get(type_prop_set, 0);
    assert_ptr(type, "ptr type");
    expect_int_equal(type->type, Cent_ast_type_id, "type type");
    expect_str(&type->text, "type", "value type");

    /* type value */
    Cent_ast* type_value = Cent_ast_get(type_prop_set, 1);
    assert_ptr(type_value, "ptr type_value");
    expect_int_equal(type_value->type, Cent_ast_type_value, "type type");

    /* type value enum */
    Cent_value* value0 = type_value->value;
    assert_ptr(value0, "ptr value0");
    expect_int_equal(value0->type, Cent_value_type_enum, "type value0");
    expect_str(&value0->display, "Type_def_type::Integer", "value value0");;
    expect_false(value0->has_error, "has error value0");

    /* name prop set */
    Cent_ast* name_prop_set = Cent_ast_get(object_stmts, 1);
    assert_ptr(name_prop_set, "ptr name_prop_set");
    expect_int_equal(name_prop_set->type, Cent_ast_type_prop_set, "type name_prop_set");

    /* name */
    Cent_ast* name1 = Cent_ast_get(name_prop_set, 0);
    assert_ptr(name1, "ptr type");
    expect_int_equal(name1->type, Cent_ast_type_id, "type name1");
    expect_str(&name1->text, "name", "text name");

    /* name value */
    Cent_ast* name_value = Cent_ast_get(name_prop_set, 1);
    assert_ptr(name_value, "ptr name_value");
    expect_int_equal(name_value->type, Cent_ast_type_value, "type name_value");

    /* i32 value */
    Cent_value* value1 = name_value->value;
    assert_ptr(value1, "ptr value1");
    expect_int_equal(value1->type, Cent_value_type_string, "type value1");
    expect_str(&value1->data.string, "i32", "string value1");

    /* bit_count prop set */
    Cent_ast* bit_count_prop_set = Cent_ast_get(object_stmts, 2);
    assert_ptr(bit_count_prop_set, "ptr bit_count_prop_set");
    expect_int_equal(bit_count_prop_set->type, Cent_ast_type_prop_set, "type bit_count_prop_set");

    /* bit_count */
    Cent_ast* bit_count = Cent_ast_get(bit_count_prop_set, 0);
    assert_ptr(bit_count, "ptr bit_count");
    expect_int_equal(bit_count->type, Cent_ast_type_id, "type bit_count");
    expect_str(&bit_count->text, "bit_count", "text bit_count");

    /* bit_count value */
    Cent_ast* bit_count_value = Cent_ast_get(bit_count_prop_set, 1);
    assert_ptr(bit_count_value, "ptr bit_count_value");
    expect_int_equal(bit_count_value->type, Cent_ast_type_value, "type bit_count_value");

    /* 32 value */
    Cent_value* value2 = bit_count_value->value;
    assert_ptr(value2, "ptr value2");
    expect_int_equal(value2->type, Cent_value_type_number, "type value2");
    expect_int_equal(value2->number_type, Cent_number_type_integer, "number_type value2");
    expect_long_long_equal(value2->data.integer, 32, "integer value2");

    /* is_signed prop set */
    Cent_ast* is_signed_prop_set = Cent_ast_get(object_stmts, 3);
    assert_ptr(is_signed_prop_set, "ptr is_signed_prop_set");
    expect_int_equal(is_signed_prop_set->type, Cent_ast_type_prop_set, "type is_signed_prop_set");

    /* is_signed */
    Cent_ast* is_signed = Cent_ast_get(is_signed_prop_set, 0);
    assert_ptr(is_signed, "ptr is_signed");
    expect_int_equal(is_signed->type, Cent_ast_type_id, "type is_signed");
    expect_str(&is_signed->text, "is_signed", "text is_signed");

    /* is_signed value */
    Cent_ast* is_signed_value = Cent_ast_get(is_signed_prop_set, 1);
    assert_ptr(is_signed_value, "ptr is_signed_value");
    expect_int_equal(is_signed_value->type, Cent_ast_type_value, "type is_signed_value");

    /* true value */
    Cent_value* value3 = is_signed_value->value;
    assert_ptr(value3, "ptr value3");
    expect_int_equal(value3->type, Cent_value_type_boolean, "type value3");
    expect_long_long_equal(value3->data.boolean, true, "boolean value3");

    test_parse_teardown(&pd, &pr);
}

void test_parse()
{
    test_parse_element();
    test_parse_enumerate();
    test_parse_top_level_assignment();
}