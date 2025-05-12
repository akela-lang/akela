#include <json/token.h>
#include "json/dom.h"
#include "json/stringify.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Json_unit_stringify_null(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_null);

    Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);
    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "null", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_true(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_boolean);
    dom->value.boolean = true;

    Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);
    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "true", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_false(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_boolean);
    dom->value.boolean = false;

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);
    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "false", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    Zinc_string_add_str(&dom->value.string, "hello");

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "\"hello\"", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_string2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    Zinc_string_add_str(&dom->value.string, "θθθ");

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "\"\\u03b8\\u03b8\\u03b8\"", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_string3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    Zinc_string_add_str(&dom->value.string, "🩒");

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "\"\\u1fa52\"", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_string4(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    Zinc_string_add_str(&dom->value.string, "\t");

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "\"\\t\"", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_string_error_invalid_char(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_string);
    char c[2] = {7, '\0'};      /* bell */
    Zinc_string_add_str(&dom->value.string, c);

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_has_errors(test, el);
    Zinc_error* e = Zinc_test_expect_source_error(test, el, "invalid string character");
    Zinc_test_assert_ptr(test, e, "ptr e");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_number_integer(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_number);
    dom->number_type = Json_number_type_integer;
    dom->value.integer = 123;

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "123", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_number_fraction(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_number);
    dom->number_type = Json_number_type_fp;
    dom->value.fp = 5.1;

    Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "5.100000", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_number_exponent(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_number);
    dom->number_type = Json_number_type_fp;
    dom->value.fp = 5.1e1;

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "51.000000", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_array_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_array);

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "[]", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_array_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_array);

    Json_dom* dom0 = NULL;
    Json_dom_create(&dom0);
    Json_dom_set_type(dom0, Json_dom_type_number);
    dom0->number_type = Json_number_type_integer;
    dom0->value.integer = 3;
    Json_dom_add_element(dom, dom0);

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "[3]", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_array2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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
    Zinc_string_add_str(&dom2->value.string, "hello");
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

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "[1,5.100000,\"hello\",true,false,null]", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_object_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_object);

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "{}", "bf");

    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_test_stringify_object_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_object);

    struct Zinc_string name0;
    Zinc_string_init(&name0);
    Zinc_string_add_str(&name0, "one");

    Json_dom* value0 = NULL;
    Json_dom_create(&value0);
    Json_dom_set_type(value0, Json_dom_type_number);
    value0->number_type = Json_number_type_integer;
    value0->value.integer = 1;

    Json_dom_add_property(dom, &name0, value0);

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_string(test, &bf, "{\"one\":1}", "bf");

    Zinc_string_destroy(&name0);
    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify_object_many(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_dom* dom = NULL;
    Json_dom_create(&dom);
    Json_dom_set_type(dom, Json_dom_type_object);

    struct Zinc_string name0;
    Zinc_string_init(&name0);
    Zinc_string_add_str(&name0, "one");

    Json_dom* value0 = NULL;
    Json_dom_create(&value0);
    Json_dom_set_type(value0, Json_dom_type_number);
    value0->number_type = Json_number_type_integer;
    value0->value.integer = 1;

    struct Zinc_string name1;
    Zinc_string_init(&name1);
    Zinc_string_add_str(&name1, "two");

    Json_dom* value1 = NULL;
    Json_dom_create(&value1);
    Json_dom_set_type(value1, Json_dom_type_number);
    value1->number_type = Json_number_type_fp;
    value1->value.fp = 5.5;

    struct Zinc_string name2;
    Zinc_string_init(&name2);
    Zinc_string_add_str(&name2, "three");

    Json_dom* value2 = NULL;
    Json_dom_create(&value2);
    Json_dom_set_type(value2, Json_dom_type_string);
    Zinc_string_add_str(&value2->value.string, "hello");

    Json_dom_add_property(dom, &name0, value0);
    Json_dom_add_property(dom, &name1, value1);
    Json_dom_add_property(dom, &name2, value2);

    struct Zinc_error_list* el = NULL;
    Zinc_error_list_create(&el);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Json_stringify(el, dom, &bf);

    Zinc_test_expect_no_errors(test, el);
    Zinc_test_expect_true(test, 
        Zinc_string_compare_str(&bf, "{\"one\":1,\"two\":5.500000,\"three\":\"hello\"}")
        || Zinc_string_compare_str(&bf, "{\"one\":1,\"three\":\"hello\",\"two\":5.500000}")
        || Zinc_string_compare_str(&bf, "{\"two\":5.500000,\"one\":1,\"three\":\"hello\"}")
        || Zinc_string_compare_str(&bf, "{\"two\":5.500000,\"three\":\"hello\",\"one\":1}")
        || Zinc_string_compare_str(&bf, "{\"three\":\"hello\",\"one\":1,\"two\":5.500000}")
        || Zinc_string_compare_str(&bf, "{\"three\":\"hello\",\"two\":5.500000},\"one\":1"),
        "bf");

    Zinc_string_destroy(&name0);
    Zinc_string_destroy(&name1);
    Zinc_string_destroy(&name2);
    Zinc_string_destroy(&bf);
    Json_dom_destroy(dom);
    free(dom);
    Zinc_error_list_destroy(el);
    free(el);
}

void Json_unit_stringify(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Json_unit_stringify_null);
        Zinc_test_register(test, Json_unit_stringify_true);
        Zinc_test_register(test, Json_unit_stringify_false);

        Zinc_test_register(test, Json_unit_stringify_string);
        Zinc_test_register(test, Json_unit_stringify_string2);
        Zinc_test_register(test, Json_unit_stringify_string3);
        Zinc_test_register(test, Json_unit_stringify_string4);
        Zinc_test_register(test, Json_unit_stringify_string_error_invalid_char);

        Zinc_test_register(test, Json_unit_stringify_number_integer);
        Zinc_test_register(test, Json_unit_stringify_number_fraction);
        Zinc_test_register(test, Json_unit_stringify_number_exponent);

        Zinc_test_register(test, Json_unit_stringify_array_empty);
        Zinc_test_register(test, Json_unit_stringify_array_one);
        Zinc_test_register(test, Json_unit_stringify_array2);

        Zinc_test_register(test, Json_unit_stringify_object_empty);
        Zinc_test_register(test, Json_test_stringify_object_one);
        Zinc_test_register(test, Json_unit_stringify_object_many);

        return;
    }

    Zinc_test_perform(test);
}