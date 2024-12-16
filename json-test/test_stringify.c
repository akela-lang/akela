#include <zinc/error_unit_test.h>

#include "zinc/unit_test.h"
#include "json/dom.h"
#include "json/stringify.h"

void test_stringify_null()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_null);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);
    expect_no_errors(el);
    expect_str(&bf, "null", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify_true()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_boolean);
    dom->value.boolean = true;

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);
    expect_no_errors(el);
    expect_str(&bf, "true", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify_false()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_boolean);
    dom->value.boolean = false;

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);
    expect_no_errors(el);
    expect_str(&bf, "false", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify_string()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    buffer_copy_str(&dom->value.string, "hello");

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "\"hello\"", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify_string2()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    buffer_copy_str(&dom->value.string, "θθθ");

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "\"\\u03b8\\u03b8\\u03b8\"", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify_string3()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    buffer_copy_str(&dom->value.string, "🩒");

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "\"\\u1fa52\"", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify_string4()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    buffer_copy_str(&dom->value.string, "\t");

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "\"\\t\"", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify_string_error_invalid_char()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    char c[2] = {7, '\0'};      /* bell */
    buffer_copy_str(&dom->value.string, c);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_has_errors(el);
    struct error* e = expect_source_error(el, "invalid string character");
    assert_ptr(e, "ptr e");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
}

void test_stringify()
{
    test_stringify_null();
    test_stringify_true();
    test_stringify_false();
    test_stringify_string();
    test_stringify_string2();
    test_stringify_string3();
    test_stringify_string4();
    test_stringify_string_error_invalid_char();
}