#include "zinc/unit_test.h"
#include <stdbool.h>
#include "test_compile_tools.h"
#include "cobble/compile.h"
#include "cobble/match.h"
#include <string.h>
#include "zinc/error_unit_test.h"

Cob_result match_run(char* pattern, char* text)
{
    Cob_compile_data* cd = NULL;
    setup_compile(&cd, pattern);
    Cob_re re = Cob_compile(cd);
    Zinc_assert_no_errors(re.el);

    const size_t size = strlen(text);
    const Zinc_string_slice slice = {text, size};
    Cob_result mr = Cob_match(&re, slice);
    teardown_compile(cd, &re);
    return mr;
}

void match_teardown(Cob_result* mr)
{
    Cob_result_destroy(mr);
}

void test_match_empty()
{
    test_name(__func__);
    Cob_result mr = match_run("", "");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_literal()
{
    test_name(__func__);
    Cob_result mr = match_run("a", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_literal_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("a", "b");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_concat()
{
    test_name(__func__);
    Cob_result mr = match_run("ab", "ab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_concat_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("ab", "ax");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_concat_not_match2()
{
    test_name(__func__);
    Cob_result mr = match_run("ab", "xb");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_union1()
{
    test_name(__func__);
    Cob_result mr = match_run("a|b", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_union2()
{
    test_name(__func__);
    Cob_result mr = match_run("a|b", "b");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_union_not_match1()
{
    test_name(__func__);
    Cob_result mr = match_run("a|b", "c");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_union3()
{
    test_name(__func__);
    Cob_result mr = match_run("a|b|c", "c");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "c", "item groups");
    match_teardown(&mr);
}

void test_match_union_cat1()
{
    test_name(__func__);
    Cob_result mr = match_run("ab|cd", "ab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_union_cat2()
{
    test_name(__func__);
    Cob_result mr = match_run("ab|cd", "cd");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "cd", "item groups");
    match_teardown(&mr);
}

void test_match_closure_zero()
{
    test_name(__func__);
    Cob_result mr = match_run("a*", "");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_closure_one()
{
    test_name(__func__);
    Cob_result mr = match_run("a*", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_closure_two()
{
    test_name(__func__);
    Cob_result mr = match_run("a*", "aa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_closure_three()
{
    test_name(__func__);
    Cob_result mr = match_run("a*", "aaa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aaa", "item groups");
    match_teardown(&mr);
}

void test_match_closure_concat()
{
    test_name(__func__);
    Cob_result mr = match_run("a*b", "aab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aab", "item groups");
    match_teardown(&mr);
}

void test_match_group()
{
    test_name(__func__);
    Cob_result mr = match_run("(ab)", "ab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 2, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    expect_buffer_list_item(&mr.groups, 1, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_group2()
{
    test_name(__func__);
    Cob_result mr = match_run("a()b", "ab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 2, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    expect_buffer_list_item(&mr.groups, 1, "", "item groups");
    match_teardown(&mr);
}

void test_match_union_backtrack()
{
    test_name(__func__);
    Cob_result mr = match_run("(aa|a)a", "aa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 2, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    expect_buffer_list_item(&mr.groups, 1, "a", "item groups");
    match_teardown(&mr);
}

void test_match_closure_backtrack()
{
    test_name(__func__);
    Cob_result mr = match_run("a*a", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_pos_closure_one()
{
    test_name(__func__);
    Cob_result mr = match_run("a+", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_pos_closure_two()
{
    test_name(__func__);
    Cob_result mr = match_run("a+", "aa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_pos_closure_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("a+", "b");
    expect_false(mr.matched, "m");
    match_teardown(&mr);
}

void test_match_pos_closure_backtrack1()
{
    test_name(__func__);
    Cob_result mr = match_run("a+a", "aa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_zero()
{
    test_name(__func__);
    Cob_result mr = match_run("a{0}", "");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_zero2()
{
    test_name(__func__);
    Cob_result mr = match_run("a{0}", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_one()
{
    test_name(__func__);
    Cob_result mr = match_run("a{1}", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_one_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("a{1}", "b");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_two()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2}", "aa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_two_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2}", "ab");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_two_not_match_short()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2}", "a");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_range_zero_zero()
{
    test_name(__func__);
    Cob_result mr = match_run("a{0,0}", "");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_zero_zero_more()
{
    test_name(__func__);
    Cob_result mr = match_run("a{0,0}", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_zero_one()
{
    test_name(__func__);
    Cob_result mr = match_run("a{0,1}", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_one_one()
{
    test_name(__func__);
    Cob_result mr = match_run("a{1,1}", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_two()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_three()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aaa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aaa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_four()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aaaa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aaaa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_not_match_less()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "a");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_more()
{
    test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aaaaa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "aaaa", "count groups");
    match_teardown(&mr);
}

void test_match_option_zero()
{
    test_name(__func__);
    Cob_result mr = match_run("a?", "");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_option_one_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("a?", "b");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_option_one()
{
    test_name(__func__);
    Cob_result mr = match_run("a?", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_option_two()
{
    test_name(__func__);
    Cob_result mr = match_run("a?", "aa");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_option_two_not_match2()
{
    test_name(__func__);
    Cob_result mr = match_run("a?", "ab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "count groups");
    match_teardown(&mr);
}

void test_match_wildcard_one_one()
{
    test_name(__func__);
    Cob_result mr = match_run(".", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_one_one2()
{
    test_name(__func__);
    Cob_result mr = match_run(".", "b");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_one_zero_not_matched()
{
    test_name(__func__);
    Cob_result mr = match_run(".", "");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_wildcard_two_two()
{
    test_name(__func__);
    Cob_result mr = match_run("..", "ab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_three_three()
{
    test_name(__func__);
    Cob_result mr = match_run("a.c", "abc");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "abc", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_newline_not_matched()
{
    test_name(__func__);
    Cob_result mr = match_run(".", "\n");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_begin_one()
{
    test_name(__func__);
    Cob_result mr = match_run("^a", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_begin_zero()
{
    test_name(__func__);
    Cob_result mr = match_run("^", "");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_begin_one_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("a^", "");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_end_zero()
{
    test_name(__func__);
    Cob_result mr = match_run("$", "");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_end_line()
{
    test_name(__func__);
    Cob_result mr = match_run("a$", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_end_line_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("a$b", "ab");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_escape_one_backslash()
{
    test_name(__func__);
    Cob_result mr = match_run("\\\\", "\\");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "\\", "item groups");
    match_teardown(&mr);
}

void test_match_escape_one_closure()
{
    test_name(__func__);
    Cob_result mr = match_run("\\*", "*");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "*", "item groups");
    match_teardown(&mr);
}

void test_match_escape_two()
{
    test_name(__func__);
    Cob_result mr = match_run("\\a\\b", "ab");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_escape_one_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\a", "b");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_one()
{
    test_name(__func__);
    Cob_result mr = match_run("[a]", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_one_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("[a]", "b");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_two()
{
    test_name(__func__);
    Cob_result mr = match_run("[ab]", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_two2()
{
    test_name(__func__);
    Cob_result mr = match_run("[ab]", "b");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_three()
{
    test_name(__func__);
    Cob_result mr = match_run("[abc]", "b");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range()
{
    test_name(__func__);
    Cob_result mr = match_run("[a-z]", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range2()
{
    test_name(__func__);
    Cob_result mr = match_run("[a-z]", "b");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range3()
{
    test_name(__func__);
    Cob_result mr = match_run("[a-z]", "z");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "z", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("[a-z]", "1");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite()
{
    test_name(__func__);
    Cob_result mr = match_run("[^abc]", "d");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "d", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("[^abc]", "b");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite_range()
{
    test_name(__func__);
    Cob_result mr = match_run("[^a-z]", "0");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "0", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite_range_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("[^a-z]", "l");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_word()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word2()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w", "x");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "x", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word3()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w", "z");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "z", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word4()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w", "A");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "A", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word5()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w", "X");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "X", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word6()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w", "Z");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "Z", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word7()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w\\w\\w", "a0_");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a0_", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word_unicode()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w\\w\\w", "αβγ");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "αβγ", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w", "|");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_word_opposite()
{
    test_name(__func__);
    Cob_result mr = match_run("\\W\\W\\W", "+;*");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "+;*", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word_opposite_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\W", "a");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_digit()
{
    test_name(__func__);
    Cob_result mr = match_run("\\d", "0");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "0", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit2()
{
    test_name(__func__);
    Cob_result mr = match_run("\\d", "5");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "5", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit3()
{
    test_name(__func__);
    Cob_result mr = match_run("\\d", "9");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "9", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_unicode()
{
    test_name(__func__);
    Cob_result mr = match_run("\\d", "\u0660");  /* Arabic-Indic Digit Zero */
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "\u0660", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\d", "a");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_opposite()
{
    test_name(__func__);
    Cob_result mr = match_run("\\D\\D\\D", "a.|");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a.|", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_opposite_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\D", "0");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_space()
{
    test_name(__func__);
    Cob_result mr = match_run("\\s\\s\\s", " \t\n");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, " \t\n", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_space_unicode()
{
    test_name(__func__);
    Cob_result mr = match_run("\\s", "\u1680");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "\u1680", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_space_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\s", "a");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_space_opposite()
{
    test_name(__func__);
    Cob_result mr = match_run("\\S\\S\\S", "abc");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "abc", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_space_opposite_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\S", " ");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_escape_newline()
{
    test_name(__func__);
    Cob_result mr = match_run("\\\n", "\n");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "\n", "item groups");
    match_teardown(&mr);
}

void test_match_escape_newline_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\\n", "x");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_newline_opposite()
{
    test_name(__func__);
    Cob_result mr = match_run("\\N", "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_newline_opposite_not_match()
{
    test_name(__func__);
    Cob_result mr = match_run("\\N", "\n");
    expect_false(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_scan_all()
{
    test_name(__func__);
    struct Zinc_string_list* groups = NULL;
    Cob_result mr = match_run("a", "za");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_word_type()
{
    test_name(__func__);
    Cob_result mr = match_run("\\w\\w\\w", "a0_");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 1, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a0_", "item groups");
    match_teardown(&mr);
}

void test_match_coverage_line()
{
    test_name(__func__);
    Cob_result mr = match_run(
        "\\s*(\\-|\\d+):\\s*(\\d+):Source:(.+)",
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 4, "count groups");
    expect_buffer_list_item(
        &mr.groups,
        0,
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    expect_buffer_list_item(
        &mr.groups,
        1,
        "-", "item groups");
    expect_buffer_list_item(
        &mr.groups,
        2,
        "0", "item groups");
    expect_buffer_list_item(
        &mr.groups,
        3,
        "/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    match_teardown(&mr);
}

void test_match_coverage_line2()
{
    test_name(__func__);
    Cob_result mr = match_run(
        "\\s*(\\-):\\s*(\\d+):(Source:)?(.+)",
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 5, "count groups");
    expect_buffer_list_item(
        &mr.groups,
        0,
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    expect_buffer_list_item(
        &mr.groups,
        1,
        "-", "item groups");
    expect_buffer_list_item(
        &mr.groups,
        2,
        "0", "item groups");
    expect_buffer_list_item(
        &mr.groups,
        3,
        "Source:", "item groups");
    expect_buffer_list_item(
        &mr.groups,
        4,
        "/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    match_teardown(&mr);
}

void test_match_coverage_line3()
{
    test_name(__func__);
    Cob_result mr = match_run(
        "\\s*(\\-):\\s*(\\d+):(Source:)?(.+)",
        "        -:    1:#include \"zinc/unit_test.h\"");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 5, "count groups");
    expect_buffer_list_item(
        &mr.groups,
        0,
        "        -:    1:#include \"zinc/unit_test.h\"",
        "item groups");
    expect_buffer_list_item(
        &mr.groups,
        1,
        "-",
        "item groups");
    expect_buffer_list_item(
        &mr.groups,
        2,
        "1",
        "item groups");
    expect_buffer_list_item(
        &mr.groups,
        3,
        "",
        "item groups");
    expect_buffer_list_item(
        &mr.groups,
        4,
        "#include \"zinc/unit_test.h\"",
        "item groups");
    match_teardown(&mr);
}

void test_match_missing_group()
{
    test_name(__func__);
    Cob_result mr = match_run(
        "(c|xyx()|a)",
        "a");
    expect_true(mr.matched, "m");
    expect_buffer_list_count(&mr.groups, 3, "count groups");
    expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    expect_buffer_list_item(&mr.groups, 1, "a", "item groups");
    expect_buffer_list_item(&mr.groups, 2, "", "item groups");
    match_teardown(&mr);
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
     test_match_wildcard_newline_not_matched();

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
     test_match_character_type_word7();
     test_match_character_type_word_unicode();
     test_match_character_type_word_not_match();

     test_match_character_type_word_opposite();
     test_match_character_type_word_opposite_not_match();

     test_match_character_type_digit();
     test_match_character_type_digit2();
     test_match_character_type_digit3();
     test_match_character_type_digit_unicode();
     test_match_character_type_digit_not_match();

     test_match_character_type_digit_opposite();
     test_match_character_type_digit_opposite_not_match();

     test_match_character_type_space();
     test_match_character_type_space_unicode();
     test_match_character_type_space_not_match();

     test_match_character_type_space_opposite();
     test_match_character_type_space_opposite_not_match();

     test_match_escape_newline();
     test_match_escape_newline_not_match();
     test_match_character_type_newline_opposite();
     test_match_character_type_newline_opposite_not_match();

     test_match_scan_all();

     test_match_word_type();

     test_match_coverage_line();
     test_match_coverage_line2();
     test_match_coverage_line3();

    test_match_missing_group();
}
