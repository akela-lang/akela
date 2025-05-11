#include "json/parse.h"
#include "json/dom.h"
#include "unit_parse_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Json_unit_parse_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_result res = Json_parse_str( "\"hello\"");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_string, "type dom");
    Zinc_test_expect_string(test, &res.root->value.string, "hello", "string dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_string_escape(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_result res = Json_parse_str( "\"hello\\tworld\"");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_string, "type dom");
    Zinc_test_expect_string(test, &res.root->value.string, "hello\tworld", "string dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_string_escape_unicode(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_result res = Json_parse_str( "\"\\u0061\"");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_string, "type dom");
    Zinc_test_expect_string(test, &res.root->value.string, "a", "string dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_number_integer(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_result res = Json_parse_str( "30");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_number, "type dom");
    Zinc_test_expect_int_equal(test, res.root->number_type, Json_number_type_integer, "number_type dom");
    Zinc_test_expect_long_long_equal(test, res.root->value.integer, 30, "integer dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_number_fp(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "1.8");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_number, "type dom");
    Zinc_test_expect_int_equal(test, res.root->number_type, Json_number_type_fp, "number_type dom");
    Zinc_test_expect_double_equal(test, res.root->value.fp, 1.8, "fp dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_array_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "[]");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_array, "type dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_array_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "[1.2]");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_array, "type dom");

    Json_dom* a = res.root->head;
    Zinc_test_assert_ptr(test, a, "ptr a");
    Zinc_test_expect_int_equal(test, a->type, Json_dom_type_number, "type a");
    Zinc_test_expect_int_equal(test, a->number_type, Json_number_type_fp, "number_type a");
    Zinc_test_expect_double_equal(test, a->value.fp, 1.2, "fp a");

    Json_result_destroy(&res);
}

void Json_unit_parse_array_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_parse_data pd;
    Json_result res = Json_parse_str( "[1.2, \"hello\"]");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_array, "type dom");

    Json_dom* a = res.root->head;
    Zinc_test_assert_ptr(test, a, "ptr a");
    Zinc_test_expect_int_equal(test, a->type, Json_dom_type_number, "type a");
    Zinc_test_expect_int_equal(test, a->number_type, Json_number_type_fp, "number_type a");
    Zinc_test_expect_double_equal(test, a->value.fp, 1.2, "fp a");

    Json_dom* b = a->next;
    Zinc_test_assert_ptr(test, b, "ptr b");
    Zinc_test_expect_int_equal(test, b->type, Json_dom_type_string, "type b");
    Zinc_test_expect_string(test, &b->value.string, "hello", "string b");

    Json_result_destroy(&res);
}

void Json_unit_parse_array_error_no_right_square_bracket(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "[1.2, \"hello\"");

    Zinc_test_expect_has_errors(test, res.el);
    struct Zinc_error* e = Zinc_test_expect_source_error(test, res.el, "expected right square bracket");
    Zinc_test_assert_ptr(test, e, "ptr e");
    Zinc_test_expect_size_t_equal(test, e->loc.start_pos, 13, "start pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.end_pos, 16, "end pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.line, 1, "end pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.col, 14, "end pos e");

    Json_result_destroy(&res);
}

void Json_unit_parse_array_error_expected_value_after_comma(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "[1.2, \"hello\",");

    Zinc_test_expect_has_errors(test, res.el);
    struct Zinc_error* e = Zinc_test_expect_source_error(test, res.el, "expected value");
    Zinc_test_assert_ptr(test, e, "ptr e");
    Zinc_test_expect_size_t_equal(test, e->loc.start_pos, 14, "start pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.end_pos, 17, "end pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.line, 1, "end pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.col, 15, "end pos e");

    Json_result_destroy(&res);
}

void Json_unit_parse_boolean_true(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_parse_data pd;
    Json_result res = Json_parse_str( "true");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_boolean, "type dom");
    Zinc_test_expect_true(test, res.root->value.boolean, "boolean dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_boolean_false(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "false");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_boolean, "type dom");
    Zinc_test_expect_false(test, res.root->value.boolean, "boolean dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_null(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "null");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_null, "type dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_error_token(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "null{}");

    Zinc_test_expect_has_errors(test, res.el);
    struct Zinc_error* e = Zinc_test_expect_source_error(test, res.el, "Could not process token: left curly brace");
    Zinc_test_assert_ptr(test, e, "ptr e");
    Zinc_test_expect_size_t_equal(test, e->loc.start_pos, 4, "start pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.end_pos, 5, "end pos e");
    Zinc_test_expect_size_t_equal(test, e->loc.line, 1, "line e");
    Zinc_test_expect_size_t_equal(test, e->loc.col, 5, "col e");

    Json_result_destroy(&res);
}

void Json_unit_parse_object_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str( "{}");

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_assert_ptr(test, res.root, "ptr dom");
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_object, "type dom");

    Json_result_destroy(&res);
}

void Json_unit_parse_object_one_property(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str(
    "{\n"
        "  \"repeat\": true"
        "}\n"
    );

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_assert_ptr(test, res.root, "ptr dom");
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_object, "type dom");

    Json_dom* repeat = Zinc_hash_map_string_get_str(&res.root->value.object, "repeat");
    Zinc_test_assert_ptr(test, repeat, "ptr repeat");
    Zinc_test_expect_int_equal(test, repeat->type, Json_dom_type_boolean, "type repeat");
    Zinc_test_expect_true(test, repeat->value.boolean, "repeat");

    Json_result_destroy(&res);
}

void Json_unit_parse_object_two_properties(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Json_result res = Json_parse_str(
    "{\n"
        "  \"first_name\": \"Fred\","
        "  \"last_name\": \"Smith\""
        "}\n"
    );

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_assert_ptr(test, res.root, "ptr dom");
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_object, "type dom");

    Json_dom* first_name = Zinc_hash_map_string_get_str(&res.root->value.object, "first_name");
    Zinc_test_assert_ptr(test, first_name, "ptr first_name");
    Zinc_test_expect_int_equal(test, first_name->type, Json_dom_type_string, "type first_name");
    Zinc_test_expect_string(test, &first_name->value.string, "Fred", "str first_name");

    Json_dom* last_name = Zinc_hash_map_string_get_str(&res.root->value.object, "last_name");
    Zinc_test_assert_ptr(test, last_name, "ptr last_name");
    Zinc_test_expect_int_equal(test, last_name->type, Json_dom_type_string, "type last_name");
    Zinc_test_expect_string(test, &last_name->value.string, "Smith", "str last_name");

    Json_result_destroy(&res);
}

void Json_unit_parse_object_three_properties(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    
    Json_result res = Json_parse_str(
        "{\n"
        "  \"first_name\": \"Fred\",\n"
        "  \"last_name\": \"Smith\",\n"
        "  \"age\": 31\n"
        "}\n"
    );

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_assert_ptr(test, res.root, "ptr dom");
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_object, "type dom");

    Json_dom* first_name = Zinc_hash_map_string_get_str(&res.root->value.object, "first_name");
    Zinc_test_assert_ptr(test, first_name, "ptr first_name");
    Zinc_test_expect_int_equal(test, first_name->type, Json_dom_type_string, "type first_name");
    Zinc_test_expect_string(test, &first_name->value.string, "Fred", "str first_name");

    Json_dom* last_name = Zinc_hash_map_string_get_str(&res.root->value.object, "last_name");
    Zinc_test_assert_ptr(test, last_name, "ptr last_name");
    Zinc_test_expect_int_equal(test, last_name->type, Json_dom_type_string, "type last_name");
    Zinc_test_expect_string(test, &last_name->value.string, "Smith", "str last_name");

    Json_dom* age = Zinc_hash_map_string_get_str(&res.root->value.object, "age");
    Zinc_test_assert_ptr(test, age, "ptr age");
    Zinc_test_expect_int_equal(test, age->type, Json_dom_type_number, "type age");
    Zinc_test_expect_int_equal(test, age->number_type, Json_number_type_integer, "type age");
    Zinc_test_expect_long_long_equal(test, age->value.integer, 31, "str age");

    Json_result_destroy(&res);
}

void Json_unit_parse_string_true(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string string;
    Zinc_string_init(&string);
    Zinc_string_add_str(&string, "true");
    Json_result res = Json_parse_zstring(&string);

    Zinc_test_expect_no_errors(test, res.el);
    Zinc_test_assert_ptr(test, res.root, "ptr dom");
    Zinc_test_expect_int_equal(test, res.root->type, Json_dom_type_boolean, "type dom");
    Zinc_test_expect_true(test, res.root->value.boolean, "root true");

    Json_result_destroy(&res);
    Zinc_string_destroy(&string);
}

void Json_unit_parse(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Json_unit_parse_string);
        Zinc_test_register(test, Json_unit_parse_string_escape);
        Zinc_test_register(test, Json_unit_parse_string_escape_unicode);

        Zinc_test_register(test, Json_unit_parse_number_integer);
        Zinc_test_register(test, Json_unit_parse_number_fp);

        Zinc_test_register(test, Json_unit_parse_array_empty);
        Zinc_test_register(test, Json_unit_parse_array_one);
        Zinc_test_register(test, Json_unit_parse_array_two);
        Zinc_test_register(test, Json_unit_parse_array_error_no_right_square_bracket);
        Zinc_test_register(test, Json_unit_parse_array_error_expected_value_after_comma);

        Zinc_test_register(test, Json_unit_parse_boolean_true);
        Zinc_test_register(test, Json_unit_parse_boolean_false);
        Zinc_test_register(test, Json_unit_parse_null);

        Zinc_test_register(test, Json_unit_parse_error_token);

        Zinc_test_register(test, Json_unit_parse_object_empty);
        Zinc_test_register(test, Json_unit_parse_object_one_property);
        Zinc_test_register(test, Json_unit_parse_object_two_properties);
        Zinc_test_register(test, Json_unit_parse_object_three_properties);

        Zinc_test_register(test, Json_unit_parse_string_true);

        return;
    }

    Zinc_test_perform(test);
}
