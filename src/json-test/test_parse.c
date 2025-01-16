#include <zinc/error_unit_test.h>
#include "zinc/unit_test.h"
#include "json/parse.h"
#include "json/dom.h"
#include "test_parse_tools.h"

void test_parse_string()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "\"hello\"");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_string, "type dom");
    Zinc_expect_str(&dom->value.string, "hello", "string dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_string_escape()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "\"hello\\tworld\"");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_string, "type dom");
    Zinc_expect_str(&dom->value.string, "hello\tworld", "string dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_string_escape_unicode()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "\"\\u0061\"");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_string, "type dom");
    Zinc_expect_str(&dom->value.string, "a", "string dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_number_integer()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "30");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_number, "type dom");
    Zinc_expect_int_equal(dom->number_type, Json_dom_number_type_integer, "number_type dom");
    Zinc_expect_long_long_equal(dom->value.integer, 30, "integer dom");

     Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_number_fp()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "1.8");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_number, "type dom");
    Zinc_expect_int_equal(dom->number_type, Json_dom_number_type_fp, "number_type dom");
    Zinc_expect_double_equal(dom->value.fp, 1.8, "fp dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_array_empty()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[]");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_array, "type dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_array_one()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2]");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_array, "type dom");

    Json_dom* a = dom->head;
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Json_dom_type_number, "type a");
    Zinc_expect_int_equal(a->number_type, Json_number_type_fp, "number_type a");
    Zinc_expect_double_equal(a->value.fp, 1.2, "fp a");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_array_two()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2, \"hello\"]");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_array, "type dom");

    Json_dom* a = dom->head;
    Zinc_assert_ptr(a, "ptr a");
    Zinc_expect_int_equal(a->type, Json_dom_type_number, "type a");
    Zinc_expect_int_equal(a->number_type, Json_number_type_fp, "number_type a");
    Zinc_expect_double_equal(a->value.fp, 1.2, "fp a");

    Json_dom* b = a->next;
    Zinc_assert_ptr(b, "ptr b");
    Zinc_expect_int_equal(b->type, Json_dom_type_string, "type b");
    Zinc_expect_str(&b->value.string, "hello", "string b");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_array_error_no_right_square_bracket()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2, \"hello\"");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_has_errors(pd.el);
    struct Zinc_error* e = Zinc_expect_source_error(pd.el, "expected right square bracket");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.start_pos, 13, "start pos e");
    Zinc_expect_size_t_equal(e->loc.end_pos, 16, "end pos e");
    Zinc_expect_size_t_equal(e->loc.line, 1, "end pos e");
    Zinc_expect_size_t_equal(e->loc.col, 14, "end pos e");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_array_error_expected_value_after_comma()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2, \"hello\",");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_has_errors(pd.el);
    struct Zinc_error* e = Zinc_expect_source_error(pd.el, "expected value");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.start_pos, 14, "start pos e");
    Zinc_expect_size_t_equal(e->loc.end_pos, 17, "end pos e");
    Zinc_expect_size_t_equal(e->loc.line, 1, "end pos e");
    Zinc_expect_size_t_equal(e->loc.col, 15, "end pos e");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_boolean_true()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "true");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_boolean, "type dom");
    Zinc_expect_true(dom->value.boolean, "boolean dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_boolean_false()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "false");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_boolean, "type dom");
    Zinc_expect_false(dom->value.boolean, "boolean dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_null()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "null");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_expect_int_equal(dom->type, Json_dom_type_null, "type dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_error_token()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "null{}");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_has_errors(pd.el);
    struct Zinc_error* e = Zinc_expect_source_error(pd.el, "Could not process token: left curly brace");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.start_pos, 4, "start pos e");
    Zinc_expect_size_t_equal(e->loc.end_pos, 5, "end pos e");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 5, "col e");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_object_empty()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "{}");

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_assert_ptr(dom, "ptr dom");
    Zinc_expect_int_equal(dom->type, Json_dom_type_object, "type dom");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_object_one_property()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd,
    "{\n"
        "  \"repeat\": true"
        "}\n"
    );

    Json_dom* dom = Json_parse(&pd);

    Zinc_expect_no_errors(pd.el);
    Zinc_assert_ptr(dom, "ptr dom");
    Zinc_expect_int_equal(dom->type, Json_dom_type_object, "type dom");

    Json_dom* repeat = Zinc_hash_map_string_get_str(&dom->value.object, "repeat");
    Zinc_assert_ptr(repeat, "ptr repeat");
    Zinc_expect_int_equal(repeat->type, Json_dom_type_boolean, "type repeat");
    Zinc_expect_true(repeat->value.boolean, "repeat");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_object_two_properties()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd,
    "{\n"
        "  \"first_name\": \"Fred\","
        "  \"last_name\": \"Smith\""
        "}\n"
    );

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_assert_ptr(dom, "ptr dom");
    Zinc_expect_int_equal(dom->type, Json_dom_type_object, "type dom");

    Json_dom* first_name = Zinc_hash_map_string_get_str(&dom->value.object, "first_name");
    Zinc_assert_ptr(first_name, "ptr first_name");
    Zinc_expect_int_equal(first_name->type, Json_dom_type_string, "type first_name");
    Zinc_expect_str(&first_name->value.string, "Fred", "str first_name");

    Json_dom* last_name = Zinc_hash_map_string_get_str(&dom->value.object, "last_name");
    Zinc_assert_ptr(last_name, "ptr last_name");
    Zinc_expect_int_equal(last_name->type, Json_dom_type_string, "type last_name");
    Zinc_expect_str(&last_name->value.string, "Smith", "str last_name");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse_object_three_properties()
{
    Zinc_test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd,
    "{\n"
        "  \"first_name\": \"Fred\",\n"
        "  \"last_name\": \"Smith\",\n"
        "  \"age\": 31\n"
        "}\n"
    );

    Json_dom* dom = Json_parse(&pd);
    Zinc_expect_no_errors(pd.el);
    Zinc_assert_ptr(dom, "ptr dom");
    Zinc_expect_int_equal(dom->type, Json_dom_type_object, "type dom");

    Json_dom* first_name = Zinc_hash_map_string_get_str(&dom->value.object, "first_name");
    Zinc_assert_ptr(first_name, "ptr first_name");
    Zinc_expect_int_equal(first_name->type, Json_dom_type_string, "type first_name");
    Zinc_expect_str(&first_name->value.string, "Fred", "str first_name");

    Json_dom* last_name = Zinc_hash_map_string_get_str(&dom->value.object, "last_name");
    Zinc_assert_ptr(last_name, "ptr last_name");
    Zinc_expect_int_equal(last_name->type, Json_dom_type_string, "type last_name");
    Zinc_expect_str(&last_name->value.string, "Smith", "str last_name");

    Json_dom* age = Zinc_hash_map_string_get_str(&dom->value.object, "age");
    Zinc_assert_ptr(age, "ptr age");
    Zinc_expect_int_equal(age->type, Json_dom_type_number, "type age");
    Zinc_expect_int_equal(age->number_type, Json_dom_number_type_integer, "type age");
    Zinc_expect_long_long_equal(age->value.integer, 31, "str age");

    Json_dom_destroy(dom);
    test_parse_destroy(&pd);
}

void test_parse()
{
    test_parse_string();
    test_parse_string_escape();
    test_parse_string_escape_unicode();

    test_parse_number_integer();
    test_parse_number_fp();

    test_parse_array_empty();
    test_parse_array_one();
    test_parse_array_two();
    test_parse_array_error_no_right_square_bracket();
    test_parse_array_error_expected_value_after_comma();

    test_parse_boolean_true();
    test_parse_boolean_false();
    test_parse_null();

    test_parse_error_token();

    test_parse_object_empty();
    test_parse_object_one_property();
    test_parse_object_two_properties();
    test_parse_object_three_properties();
}