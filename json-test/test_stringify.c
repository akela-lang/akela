#include "zinc/unit_test.h"
#include "json/dom.h"
#include "json/stringify.h"

void test_stringify_null()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_null);

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(dom, &bf);
    expect_str(&bf, "null", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
}

void test_stringify_true()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_boolean);
    dom->value.boolean = true;

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(dom, &bf);
    expect_str(&bf, "true", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
}

void test_stringify_false()
{
    test_name(__func__);

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_boolean);
    dom->value.boolean = false;

    struct buffer bf;
    buffer_init(&bf);

    Json_stringify(dom, &bf);
    expect_str(&bf, "false", "bf");

    buffer_destroy(&bf);
    Json_dom_destroy(dom);
}

void test_stringify()
{
    test_stringify_null();
    test_stringify_true();
    test_stringify_false();
}