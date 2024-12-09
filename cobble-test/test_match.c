#include "zinc/unit_test.h"
#include <stdbool.h>
#include "test_compile_tools.h"
#include "cobble/compile.h"
#include "cobble/match.h"
#include <string.h>
#include "zinc/error_unit_test.h"
#include "cobble/unit_test_string_slice.h"

bool match_run(char* pattern, char* text, struct buffer_list** groups)
{
    struct compile_data* cd = NULL;
    setup_compile(&cd, pattern);
    bool valid = true;
    Ast_node* root = NULL;
    valid = compile(cd, &root);
    assert_true(valid, "valid");

    const size_t size = strlen(text);
    const String_slice slice = {text, size};
    buffer_list_create(groups);
    bool matched = re_match(root, slice, *groups);
    teardown_compile(cd, root);
    return matched;
}

void match_teardown(struct buffer_list* groups)
{
    buffer_list_destroy(groups);
}

void test_match_empty()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("", "", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_literal()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_literal_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a", "b", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_concat()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("ab", "ab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "ab", "item groups");
    match_teardown(groups);
}

void test_match_concat_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("ab", "ax", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_concat_not_match2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("ab", "xb", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_union1()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a|b", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_union2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a|b", "b", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "b", "item groups");
    match_teardown(groups);
}

void test_match_union_not_match1()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a|b", "c", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_union3()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a|b|c", "c", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "c", "item groups");
    match_teardown(groups);
}

void test_match_union_cat1()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("ab|cd", "ab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "ab", "item groups");
    match_teardown(groups);
}

void test_match_union_cat2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("ab|cd", "cd", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "cd", "item groups");
    match_teardown(groups);
}

void test_match_closure_zero()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a*", "", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_closure_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a*", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_closure_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a*", "aa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aa", "item groups");
    match_teardown(groups);
}

void test_match_closure_three()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a*", "aaa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aaa", "item groups");
    match_teardown(groups);
}

void test_match_closure_concat()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a*b", "aab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aab", "item groups");
    match_teardown(groups);
}

void test_match_group()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("(ab)", "ab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 2, "count groups");
    expect_buffer_list_item(groups, 0, "ab", "item groups");
    expect_buffer_list_item(groups, 1, "ab", "item groups");
    match_teardown(groups);
}

void test_match_group2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a()b", "ab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 2, "count groups");
    expect_buffer_list_item(groups, 0, "ab", "item groups");
    expect_buffer_list_item(groups, 1, "", "item groups");
    match_teardown(groups);
}

void test_match_union_backtrack()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("(aa|a)a", "aa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 2, "count groups");
    expect_buffer_list_item(groups, 0, "aa", "item groups");
    expect_buffer_list_item(groups, 1, "a", "item groups");
    match_teardown(groups);
}

void test_match_closure_backtrack()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a*a", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_pos_closure_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a+", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_pos_closure_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a+", "aa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aa", "item groups");
    match_teardown(groups);
}

void test_match_pos_closure_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a+", "b", &groups);
    expect_false(matched, "m");
    match_teardown(groups);
}

void test_match_pos_closure_backtrack1()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a+a", "aa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aa", "item groups");
    match_teardown(groups);
}

void test_match_repeat_zero()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{0}", "", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_repeat_zero2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{0}", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_repeat_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{1}", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_repeat_one_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{1}", "b", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_repeat_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2}", "aa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aa", "item groups");
    match_teardown(groups);
}

void test_match_repeat_two_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2}", "ab", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_repeat_two_not_match_short()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2}", "a", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_repeat_range_zero_zero()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{0,0}", "", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_repeat_range_zero_zero_more()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{0,0}", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_repeat_range_zero_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{0,1}", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_repeat_range_one_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{1,1}", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_repeat_range_two_four_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2,4}", "aa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aa", "item groups");
    match_teardown(groups);
}

void test_match_repeat_range_two_four_three()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2,4}", "aaa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aaa", "item groups");
    match_teardown(groups);
}

void test_match_repeat_range_two_four_four()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2,4}", "aaaa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aaaa", "item groups");
    match_teardown(groups);
}

void test_match_repeat_range_two_four_not_match_less()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2,4}", "a", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_repeat_range_two_four_more()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a{2,4}", "aaaaa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "aaaa", "count groups");
    match_teardown(groups);
}

void test_match_option_zero()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a?", "", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_option_one_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a?", "b", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_option_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a?", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_option_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a?", "aa", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_option_two_not_match2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a?", "ab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "count groups");
    match_teardown(groups);
}

void test_match_wildcard_one_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run(".", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_wildcard_one_one2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run(".", "b", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "b", "item groups");
    match_teardown(groups);
}

void test_match_wildcard_one_zero_not_matched()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run(".", "", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_wildcard_two_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("..", "ab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "ab", "item groups");
    match_teardown(groups);
}

void test_match_wildcard_three_three()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a.c", "abc", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "abc", "item groups");
    match_teardown(groups);
}

void test_match_begin_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("^a", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_begin_zero()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("^", "", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_begin_one_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a^", "", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_end_zero()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("$", "", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "", "item groups");
    match_teardown(groups);
}

void test_match_end_line()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a$b", "a\nb", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a\nb", "item groups");
    match_teardown(groups);
}

void test_match_end_line_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a$b", "ab", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_escape_one_backslash()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\\\", "\\", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "\\", "item groups");
    match_teardown(groups);
}

void test_match_escape_one_closure()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\*", "*", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "*", "item groups");
    match_teardown(groups);
}

void test_match_escape_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\a\\b", "ab", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "ab", "item groups");
    match_teardown(groups);
}

void test_match_escape_one_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\a", "b", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_class_one()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[a]", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_character_class_one_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[a]", "b", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_class_two()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[ab]", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_character_class_two2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[ab]", "b", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "b", "item groups");
    match_teardown(groups);
}

void test_match_character_class_three()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[abc]", "b", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "b", "item groups");
    match_teardown(groups);
}

void test_match_character_class_range()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[a-z]", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_character_class_range2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[a-z]", "b", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "b", "item groups");
    match_teardown(groups);
}

void test_match_character_class_range3()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[a-z]", "z", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "z", "item groups");
    match_teardown(groups);
}

void test_match_character_class_range_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[a-z]", "1", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_class_opposite()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[^abc]", "d", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "d", "item groups");
    match_teardown(groups);
}

void test_match_character_class_opposite_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[^abc]", "b", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_class_opposite_range()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[^a-z]", "0", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "0", "item groups");
    match_teardown(groups);
}

void test_match_character_class_opposite_range_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("[^a-z]", "l", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_type_word()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w", "a", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_character_type_word2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w", "x", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "x", "item groups");
    match_teardown(groups);
}

void test_match_character_type_word3()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w", "z", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "z", "item groups");
    match_teardown(groups);
}

void test_match_character_type_word4()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w", "A", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "A", "item groups");
    match_teardown(groups);
}

void test_match_character_type_word5()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w", "X", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "X", "item groups");
    match_teardown(groups);
}

void test_match_character_type_word6()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w", "Z", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "Z", "item groups");
    match_teardown(groups);
}

void test_match_character_type_word_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w", "|", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_type_word_opposite()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\W\\W\\W", "+-*", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "+-*", "item groups");
    match_teardown(groups);
}

void test_match_character_type_word_opposite_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\W", "a", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_type_digit()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\d", "0", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "0", "item groups");
    match_teardown(groups);
}

void test_match_character_type_digit2()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\d", "5", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "5", "item groups");
    match_teardown(groups);
}

void test_match_character_type_digit3()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\d", "9", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "9", "item groups");
    match_teardown(groups);
}

void test_match_character_type_digit_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\d", "a", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_type_digit_opposite()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\D\\D\\D", "a.|", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a.|", "item groups");
    match_teardown(groups);
}

void test_match_character_type_digit_opposite_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\D", "0", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_type_space()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\s\\s\\s", " \t\n", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, " \t\n", "item groups");
    match_teardown(groups);
}

void test_match_character_type_space_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\s", "a", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_character_type_space_opposite()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\S\\S\\S", "abc", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "abc", "item groups");
    match_teardown(groups);
}

void test_match_character_type_space_opposite_not_match()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\S", " ", &groups);
    expect_false(matched, "m");
    expect_buffer_list_count(groups, 0, "count groups");
    match_teardown(groups);
}

void test_match_scan_all()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("a", "za", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a", "item groups");
    match_teardown(groups);
}

void test_match_word_type()
{
    test_name(__func__);
    struct buffer_list* groups = NULL;
    bool matched = match_run("\\w\\w\\w", "a0_", &groups);
    expect_true(matched, "m");
    expect_buffer_list_count(groups, 1, "count groups");
    expect_buffer_list_item(groups, 0, "a0_", "item groups");
    match_teardown(groups);
}

void test_match()
{
    test_match_empty();

    test_match_literal();
    test_match_literal_not_match();

    test_match_concat();
    test_match_concat_not_match();
    test_match_concat_not_match2();

    test_match_union1();
    test_match_union2();
    test_match_union_not_match1();
    test_match_union3();
    test_match_union_cat1();
    test_match_union_cat2();

    test_match_closure_zero();
    test_match_closure_one();
    test_match_closure_two();
    test_match_closure_three();
    test_match_closure_concat();

    test_match_group();
    test_match_group2();

    test_match_union_backtrack();
    test_match_closure_backtrack();

    test_match_pos_closure_one();
    test_match_pos_closure_two();
    test_match_pos_closure_not_match();
    test_match_pos_closure_backtrack1();

    test_match_repeat_zero();
    test_match_repeat_zero2();
    test_match_repeat_one();
    test_match_repeat_one_not_match();
    test_match_repeat_two();
    test_match_repeat_two_not_match();
    test_match_repeat_two_not_match_short();

    test_match_repeat_range_zero_zero();
    test_match_repeat_range_zero_zero_more();
    test_match_repeat_range_zero_one();
    test_match_repeat_range_one_one();
    test_match_repeat_range_two_four_two();
    test_match_repeat_range_two_four_three();
    test_match_repeat_range_two_four_four();
    test_match_repeat_range_two_four_not_match_less();
    test_match_repeat_range_two_four_more();

    test_match_option_zero();
    test_match_option_one_not_match();
    test_match_option_one();
    test_match_option_two();
    test_match_option_two_not_match2();

    test_match_wildcard_one_one();
    test_match_wildcard_one_one2();
    test_match_wildcard_one_zero_not_matched();
    test_match_wildcard_two_two();
    test_match_wildcard_three_three();

    test_match_begin_one();
    test_match_begin_zero();
    test_match_begin_one_not_match();

    test_match_end_zero();
    test_match_end_line();
    test_match_end_line_not_match();

    test_match_escape_one_backslash();
    test_match_escape_one_closure();
    test_match_escape_two();
    test_match_escape_one_not_match();

    test_match_character_class_one();
    test_match_character_class_one_not_match();
    test_match_character_class_two();
    test_match_character_class_two2();
    test_match_character_class_three();

    test_match_character_class_range();
    test_match_character_class_range2();
    test_match_character_class_range3();
    test_match_character_class_range_not_match();

    test_match_character_class_opposite();
    test_match_character_class_opposite_not_match();

    test_match_character_class_opposite_range();
    test_match_character_class_opposite_range_not_match();

    test_match_character_type_word();
    test_match_character_type_word2();
    test_match_character_type_word3();
    test_match_character_type_word4();
    test_match_character_type_word5();
    test_match_character_type_word6();
    test_match_character_type_word_not_match();

    test_match_character_type_word_opposite();
    test_match_character_type_word_opposite_not_match();

    test_match_character_type_digit();
    test_match_character_type_digit2();
    test_match_character_type_digit3();
    test_match_character_type_digit_not_match();

    test_match_character_type_digit_opposite();
    test_match_character_type_digit_opposite_not_match();

    test_match_character_type_space();
    test_match_character_type_space_not_match();

    test_match_character_type_space_opposite();
    test_match_character_type_space_opposite_not_match();

    test_match_scan_all();

    test_match_word_type();
}
