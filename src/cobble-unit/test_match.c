#include "zinc/unit_test.h"
#include <stdbool.h>
#include "test_compile_tools.h"
#include "cobble/compile.h"
#include "cobble/match.h"
#include <string.h>
#include "zinc/error_unit_test.h"
#include "zinc/os.h"
#include <Windows.h>

Cob_result match_run(char* pattern, char* text)
{
    Cob_re re = Cob_compile_str(pattern);
    Zinc_assert_no_errors(re.errors);

    const size_t size = strlen(text);
    const Zinc_string_slice slice = {text, size};
    Cob_result mr = Cob_match(&re, slice);
    Cob_re_destroy(&re);
    return mr;
}

void match_teardown(Cob_result* mr)
{
    Cob_result_destroy(mr);
}

void test_match_empty()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("", "");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_literal()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_literal_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a", "b");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_concat()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("ab", "ab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_concat_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("ab", "ax");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_concat_not_match2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("ab", "xb");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_union1()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a|b", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_union2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a|b", "b");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_union_not_match1()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a|b", "c");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_union3()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a|b|c", "c");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "c", "item groups");
    match_teardown(&mr);
}

void test_match_union_cat1()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("ab|cd", "ab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_union_cat2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("ab|cd", "cd");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "cd", "item groups");
    match_teardown(&mr);
}

void test_match_closure_zero()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a*", "");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_closure_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a*", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_closure_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a*", "aa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_closure_three()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a*", "aaa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aaa", "item groups");
    match_teardown(&mr);
}

void test_match_closure_concat()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a*b", "aab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aab", "item groups");
    match_teardown(&mr);
}

void test_match_group()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("(ab)", "ab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 2, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    Zinc_expect_buffer_list_item(&mr.groups, 1, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_group2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a()b", "ab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 2, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    Zinc_expect_buffer_list_item(&mr.groups, 1, "", "item groups");
    match_teardown(&mr);
}

void test_match_union_backtrack()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("(aa|a)a", "aa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 2, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    Zinc_expect_buffer_list_item(&mr.groups, 1, "a", "item groups");
    match_teardown(&mr);
}

void test_match_closure_backtrack()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a*a", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_pos_closure_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a+", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_pos_closure_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a+", "aa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_pos_closure_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a+", "b");
    Zinc_expect_false(mr.matched, "m");
    match_teardown(&mr);
}

void test_match_pos_closure_backtrack1()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a+a", "aa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_zero()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{0}", "");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_zero2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{0}", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{1}", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_one_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{1}", "b");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2}", "aa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_two_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2}", "ab");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_two_not_match_short()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2}", "a");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_range_zero_zero()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{0,0}", "");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_zero_zero_more()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{0,0}", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_zero_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{0,1}", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_one_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{1,1}", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_three()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aaa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aaa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_four()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aaaa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aaaa", "item groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_not_match_less()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "a");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_repeat_range_two_four_more()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a{2,4}", "aaaaa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "aaaa", "count groups");
    match_teardown(&mr);
}

void test_match_option_zero()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a?", "");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_option_one_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a?", "b");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_option_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a?", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_option_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a?", "aa");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_option_two_not_match2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a?", "ab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "count groups");
    match_teardown(&mr);
}

void test_match_wildcard_one_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(".", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_one_one2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(".", "b");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_one_zero_not_matched()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(".", "");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_wildcard_two_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("..", "ab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_three_three()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a.c", "abc");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "abc", "item groups");
    match_teardown(&mr);
}

void test_match_wildcard_newline_not_matched()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(".", "\n");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_begin_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("^a", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_begin_zero()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("^", "");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_begin_one_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a^", "");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_end_zero()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("$", "");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void test_match_end_line()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a$", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_end_line_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("a$b", "ab");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_escape_one_backslash()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\\\", "\\");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "\\", "item groups");
    match_teardown(&mr);
}

void test_match_escape_one_closure()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\*", "*");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "*", "item groups");
    match_teardown(&mr);
}

void test_match_escape_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\a\\b", "ab");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void test_match_escape_one_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\a", "b");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_one()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[a]", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_one_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[a]", "b");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_two()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[ab]", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_two2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[ab]", "b");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_three()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[abc]", "b");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[a-z]", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[a-z]", "b");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range3()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[a-z]", "z");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "z", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_range_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[a-z]", "1");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[^abc]", "d");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "d", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[^abc]", "b");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite_range()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[^a-z]", "0");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "0", "item groups");
    match_teardown(&mr);
}

void test_match_character_class_opposite_range_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("[^a-z]", "l");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_word()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w", "x");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "x", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word3()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w", "z");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "z", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word4()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w", "A");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "A", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word5()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w", "X");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "X", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word6()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w", "Z");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "Z", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word7()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w\\w\\w", "a0_");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a0_", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word_unicode()
{
    Zinc_test_name(__func__);
    // unicode characters not supported in character types
    Cob_result mr = match_run("\\w\\w\\w", "αβγ");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_word_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w", "|");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_word_opposite()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\W\\W\\W", "+;*");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "+;*", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_word_opposite_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\W", "a");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_digit()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\d", "0");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "0", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\d", "5");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "5", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit3()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\d", "9");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "9", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_unicode()
{
    Zinc_test_name(__func__);
	// unicode character types not supported
    Cob_result mr = match_run("\\d", "٠");  /* Arabic-Indic Digit Zero: \u0660 */
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\d", "a");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_opposite()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\D\\D\\D", "a.|");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a.|", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_digit_opposite_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\D", "0");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_space()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\s\\s\\s", " \t\n");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, " \t\n", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_space_unicode()
{
    Zinc_test_name(__func__);
	// unicode character types not supported
	Cob_result mr = match_run("\\s", " ");  // Ogham Space Mark : \u1680
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_space_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\s", "a");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_space_opposite()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\S\\S\\S", "abc");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "abc", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_space_opposite_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\S", " ");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_escape_newline()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\\n", "\n");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "\n", "item groups");
    match_teardown(&mr);
}

void test_match_escape_newline_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\\n", "x");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_character_type_newline_opposite()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\N", "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_character_type_newline_opposite_not_match()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\N", "\n");
    Zinc_expect_false(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void test_match_scan_all()
{
    Zinc_test_name(__func__);
    struct Zinc_string_list* groups = NULL;
    Cob_result mr = match_run("a", "za");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void test_match_word_type()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run("\\w\\w\\w", "a0_");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a0_", "item groups");
    match_teardown(&mr);
}

void test_match_coverage_line()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(
        "\\s*(\\-|\\d+):\\s*(\\d+):Source:(.+)",
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 4, "count groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        0,
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        1,
        "-", "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        2,
        "0", "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        3,
        "/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    match_teardown(&mr);
}

void test_match_coverage_line2()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(
        "\\s*(\\-):\\s*(\\d+):(Source:)?(.+)",
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 5, "count groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        0,
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        1,
        "-", "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        2,
        "0", "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        3,
        "Source:", "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        4,
        "/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    match_teardown(&mr);
}

void test_match_coverage_line3()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(
        "\\s*(\\-):\\s*(\\d+):(Source:)?(.+)",
        "        -:    1:#include \"zinc/unit_test.h\"");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 5, "count groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        0,
        "        -:    1:#include \"zinc/unit_test.h\"",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        1,
        "-",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        2,
        "1",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        3,
        "",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        4,
        "#include \"zinc/unit_test.h\"",
        "item groups");
    match_teardown(&mr);
}

void test_match_missing_group()
{
    Zinc_test_name(__func__);
    Cob_result mr = match_run(
        "(c|xyx()|a)",
        "a");
    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 3, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "a", "item groups");
    Zinc_expect_buffer_list_item(&mr.groups, 1, "a", "item groups");
    Zinc_expect_buffer_list_item(&mr.groups, 2, "", "item groups");
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
