#include <zinc/error_unit_test.h>
#include "zinc/unit_test.h"
#include "json/parse.h"
#include "json/dom.h"
#include "test_parse_tools.h"

void test_parse_string()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "\"hello\"");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_string, "type dom");
    expect_str(&dom->value.string, "hello", "string dom");

    test_parse_destroy(&pd);
}

void test_parse_string_escape()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "\"hello\\tworld\"");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_string, "type dom");
    expect_str(&dom->value.string, "hello\tworld", "string dom");

    test_parse_destroy(&pd);
}

void test_parse_string_escape_unicode()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "\"\\u0061\"");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_string, "type dom");
    expect_str(&dom->value.string, "a", "string dom");

    test_parse_destroy(&pd);
}

void test_parse_number_integer()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "30");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_number, "type dom");
    expect_int_equal(dom->number_type, Json_dom_number_type_integer, "number_type dom");
    expect_long_long_equal(dom->value.integer, 30, "integer dom");

    test_parse_destroy(&pd);
}

void test_parse_number_fp()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "1.8");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_number, "type dom");
    expect_int_equal(dom->number_type, Json_dom_number_type_fp, "number_type dom");
    expect_double_equal(dom->value.fp, 1.8, "fp dom");

    test_parse_destroy(&pd);
}

void test_parse_array_empty()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[]");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_array, "type dom");

    test_parse_destroy(&pd);
}

void test_parse_array_one()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2]");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_array, "type dom");

    Json_dom* a = dom->head;
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Json_dom_type_number, "type a");
    expect_int_equal(a->number_type, Json_number_type_fp, "number_type a");
    expect_double_equal(a->value.fp, 1.2, "fp a");

    test_parse_destroy(&pd);
}

void test_parse_array_two()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2, \"hello\"]");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_array, "type dom");

    Json_dom* a = dom->head;
    assert_ptr(a, "ptr a");
    expect_int_equal(a->type, Json_dom_type_number, "type a");
    expect_int_equal(a->number_type, Json_number_type_fp, "number_type a");
    expect_double_equal(a->value.fp, 1.2, "fp a");

    Json_dom* b = a->next;
    assert_ptr(b, "ptr b");
    expect_int_equal(b->type, Json_dom_type_string, "type b");
    expect_str(&b->value.string, "hello", "string b");

    test_parse_destroy(&pd);
}

void test_parse_array_error_no_right_square_bracket()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2, \"hello\"");

    Json_dom* dom = Json_parse(&pd);
    expect_false(Json_parse_is_valid(&pd, dom), "valid");
    expect_has_errors(pd.el);
    struct error* e = expect_source_error(pd.el, "expected right square bracket");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.start_pos, 13, "start pos e");
    expect_size_t_equal(e->loc.end_pos, 16, "end pos e");
    expect_size_t_equal(e->loc.line, 1, "end pos e");
    expect_size_t_equal(e->loc.col, 14, "end pos e");

    test_parse_destroy(&pd);
}

void test_parse_array_error_expected_value_after_comma()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "[1.2, \"hello\",");

    Json_dom* dom = Json_parse(&pd);
    expect_false(Json_parse_is_valid(&pd, dom), "valid");
    expect_has_errors(pd.el);
    struct error* e = expect_source_error(pd.el, "expected value");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.start_pos, 14, "start pos e");
    expect_size_t_equal(e->loc.end_pos, 17, "end pos e");
    expect_size_t_equal(e->loc.line, 1, "end pos e");
    expect_size_t_equal(e->loc.col, 15, "end pos e");

    test_parse_destroy(&pd);
}

void test_parse_boolean_true()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "true");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_boolean, "type dom");
    expect_true(dom->value.boolean, "boolean dom");

    test_parse_destroy(&pd);
}

void test_parse_boolean_false()
{
    test_name(__func__);
    Json_parse_data pd;
    test_parse_setup(&pd, "false");

    Json_dom* dom = Json_parse(&pd);
    expect_true(Json_parse_is_valid(&pd, dom), "valid");
    expect_no_errors(pd.el);
    expect_int_equal(dom->type, Json_dom_type_boolean, "type dom");
    expect_false(dom->value.boolean, "boolean dom");

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
}