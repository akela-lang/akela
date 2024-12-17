#include <cobble-test/test_compile_tools.h>
#include <json/token.h>
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
    free(el);
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
    free(el);
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
    free(el);
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
    free(el);
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
    free(el);
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
    free(el);
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
    free(el);
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
    free(el);
}

void test_stringify_number_integer()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_number);
    dom->number_type = Json_number_type_integer;
    dom->value.integer = 123;

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "123", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_number_fraction()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_number);
    dom->number_type = Json_number_type_fp;
    dom->value.fp = 5.1;

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "5.100000", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_number_exponent()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_number);
    dom->number_type = Json_number_type_fp;
    dom->value.fp = 5.1e1;

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "51.000000", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_array_empty()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_array);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "[]", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_array_one()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_array);

    Json_dom* dom0 = NULL;
    Json_dom_create(&dom0);
    Json_dom_set_type(dom0, Json_dom_type_number);
    dom0->number_type = Json_number_type_integer;
    dom0->value.integer = 3;
    Json_dom_add_element(dom, dom0);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "[3]", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_array2()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_array);

    Json_dom* dom0 = NULL;
    Json_dom_create(&dom0);
    Json_dom_set_type(dom0, Json_dom_type_number);
    dom0->number_type = Json_number_type_integer;
    dom0->value.integer = 1;
    Json_dom_add_element(dom, dom0);

    Json_dom* dom1 = NULL;
    Json_dom_create(&dom1);
    Json_dom_set_type(dom1, Json_dom_type_number);
    dom1->number_type = Json_number_type_fp;
    dom1->value.fp = 5.1;
    Json_dom_add_element(dom, dom1);

    Json_dom* dom2 = NULL;
    Json_dom_create(&dom2);
    Json_dom_set_type(dom2, Json_dom_type_string);
    buffer_copy_str(&dom2->value.string, "hello");
    Json_dom_add_element(dom, dom2);

    Json_dom* dom3 = NULL;
    Json_dom_create(&dom3);
    Json_dom_set_type(dom3, Json_dom_type_boolean);
    dom3->value.boolean = true;
    Json_dom_add_element(dom, dom3);

    Json_dom* dom4 = NULL;
    Json_dom_create(&dom4);
    Json_dom_set_type(dom4, Json_dom_type_boolean);
    dom4->value.boolean = false;
    Json_dom_add_element(dom, dom4);

    Json_dom* dom5 = NULL;
    Json_dom_create(&dom5);
    Json_dom_set_type(dom5, Json_dom_type_null);
    Json_dom_add_element(dom, dom5);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "[1,5.100000,\"hello\",true,false,null]", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_object_empty()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_object);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "{}", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_object_one()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_object);

    struct buffer name0;
    buffer_init(&name0);
    buffer_copy_str(&name0, "one");

    Json_dom* value0 = NULL;
    Json_dom_create(&value0);
    Json_dom_set_type(value0, Json_dom_type_number);
    value0->number_type = Json_number_type_integer;
    value0->value.integer = 1;

    Json_dom_add_property(dom, &name0, value0);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_str(&bf, "{\"one\":1}", "bf");

    buffer_destroy(&name0);
    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
}

void test_stringify_object_many()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_object);

    struct buffer name0;
    buffer_init(&name0);
    buffer_copy_str(&name0, "one");

    Json_dom* value0 = NULL;
    Json_dom_create(&value0);
    Json_dom_set_type(value0, Json_dom_type_number);
    value0->number_type = Json_number_type_integer;
    value0->value.integer = 1;

    struct buffer name1;
    buffer_init(&name1);
    buffer_copy_str(&name1, "two");

    Json_dom* value1 = NULL;
    Json_dom_create(&value1);
    Json_dom_set_type(value1, Json_dom_type_number);
    value1->number_type = Json_number_type_fp;
    value1->value.fp = 5.5;

    struct buffer name2;
    buffer_init(&name2);
    buffer_copy_str(&name2, "three");

    Json_dom* value2 = NULL;
    Json_dom_create(&value2);
    Json_dom_set_type(value2, Json_dom_type_string);
    buffer_copy_str(&value2->value.string, "hello");

    Json_dom_add_property(dom, &name0, value0);
    Json_dom_add_property(dom, &name1, value1);
    Json_dom_add_property(dom, &name2, value2);

    struct error_list* el = NULL;
    error_list_create(&el);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(el, dom, &bf);

    expect_no_errors(el);
    expect_true(
        buffer_compare_str(&bf, "{\"one\":1,\"two\":5.500000,\"three\":\"hello\"}")
        || buffer_compare_str(&bf, "{\"one\":1,\"three\":\"hello\",\"two\":5.500000}")
        || buffer_compare_str(&bf, "{\"two\":5.500000,\"one\":1,\"three\":\"hello\"}")
        || buffer_compare_str(&bf, "{\"two\":5.500000,\"three\":\"hello\",\"one\":1}")
        || buffer_compare_str(&bf, "{\"three\":\"hello\",\"one\":1,\"two\":5.500000}")
        || buffer_compare_str(&bf, "{\"three\":\"hello\",\"two\":5.500000},\"one\":1"),
        "bf");

    buffer_destroy(&name0);
    buffer_destroy(&name1);
    buffer_destroy(&name2);
    buffer_destroy(&bf);
    Json_dom_destroy(dom);
    error_list_destroy(el);
    free(el);
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

    test_stringify_number_integer();
    test_stringify_number_fraction();
    test_stringify_number_exponent();

    test_stringify_array_empty();
    test_stringify_array_one();
    test_stringify_array2();

    test_stringify_object_empty();
    test_stringify_object_one();
    test_stringify_object_many();
}