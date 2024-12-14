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

void test_parse()
{
    test_parse_string();
    test_parse_string_escape();
    test_parse_string_escape_unicode();
}