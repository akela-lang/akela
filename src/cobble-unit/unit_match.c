#include "cobble/compile.h"
#include "cobble/match.h"
#include <string.h>
#include "zinc/test.h"
#include "zinc/expect.h"

Cob_result match_run(Zinc_test* test, char* pattern, char* text)
{
    Cob_re re = Cob_compile_str(pattern);
    Zinc_test_assert_no_errors(test, re.errors);

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

void CobUnit_match_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_result mr = match_run(test, "", "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_literal(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_result mr = match_run(test, "a", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_literal_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_result mr = match_run(test, "a", "b");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_concat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_result mr = match_run(test, "ab", "ab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_concat_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_result mr = match_run(test, "ab", "ax");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_concat_not_match2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cob_result mr = match_run(test, "ab", "xb");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_union1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a|b", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_union2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a|b", "b");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_union_not_match1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a|b", "c");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_union3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a|b|c", "c");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "c", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_union_cat1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "ab|cd", "ab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_union_cat2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "ab|cd", "cd");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "cd", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_closure_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a*", "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_closure_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a*", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_closure_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a*", "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_closure_three(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a*", "aaa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aaa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_closure_concat(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a*b", "aab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aab", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_group(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "(ab)", "ab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 2, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "ab", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "ab", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_group2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a()b", "ab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 2, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "ab", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_union_backtrack(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "(aa|a)a", "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 2, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aa", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_closure_backtrack(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a*a", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_pos_closure_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a+", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_pos_closure_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a+", "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_pos_closure_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a+", "b");
    Zinc_test_expect_false(test, mr.matched, "m");
    match_teardown(&mr);
}

void CobUnit_match_pos_closure_backtrack1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a+a", "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{0}", "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_zero2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{0}", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{1}", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_one_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{1}", "b");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2}", "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_two_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2}", "ab");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_two_not_match_short(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2}", "a");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_zero_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{0,0}", "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_zero_zero_more(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{0,0}", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_zero_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{0,1}", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_one_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{1,1}", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_two_four_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2,4}", "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_two_four_three(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2,4}", "aaa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aaa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_two_four_four(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2,4}", "aaaa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aaaa", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_two_four_not_match_less(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2,4}", "a");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_two_four_more(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a{2,4}", "aaaaa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aaaa", "count groups");
    match_teardown(&mr);
}

void CobUnit_match_option_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a?", "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_option_one_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a?", "b");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_option_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a?", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_option_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a?", "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_option_two_not_match2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a?", "ab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "count groups");
    match_teardown(&mr);
}

void CobUnit_match_wildcard_one_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, ".", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_wildcard_one_one2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, ".", "b");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_wildcard_one_zero_not_matched(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, ".", "");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_wildcard_two_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "..", "ab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_wildcard_three_three(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a.c", "abc");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "abc", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_wildcard_newline_not_matched(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, ".", "\n");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_begin_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "^a", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_begin_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "^", "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_begin_one_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a^", "");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_end_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "$", "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_end_line(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a$", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_end_line_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "a$b", "ab");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_escape_one_backslash(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\\\", "\\");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "\\", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_escape_one_closure(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\*", "*");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "*", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_escape_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\a\\b", "ab");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "ab", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_escape_one_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\a", "b");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_one(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[a]", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_one_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[a]", "b");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_two(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[ab]", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_two2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[ab]", "b");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_three(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[abc]", "b");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_range(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[a-z]", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_range2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[a-z]", "b");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "b", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_range3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[a-z]", "z");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "z", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_range_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[a-z]", "1");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[^abc]", "d");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "d", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_opposite_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[^abc]", "b");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_opposite_range(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[^a-z]", "0");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "0", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_class_opposite_range_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "[^a-z]", "l");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w", "x");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "x", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w", "z");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "z", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word4(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w", "A");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "A", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word5(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w", "X");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "X", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word6(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w", "Z");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "Z", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word7(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w\\w\\w", "a0_");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a0_", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word_unicode(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    // unicode characters not supported in character types
    Cob_result mr = match_run(test, "\\w\\w\\w", "αβγ");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w", "|");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\W\\W\\W", "+;*");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "+;*", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_word_opposite_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\W", "a");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_digit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\d", "0");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "0", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_digit2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\d", "5");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "5", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_digit3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\d", "9");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "9", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_digit_unicode(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    // unicode character types not supported
    Cob_result mr = match_run(test, "\\d", "٠");  /* Arabic-Indic Digit Zero: \u0660 */
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_digit_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\d", "a");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_digit_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\D\\D\\D", "a.|");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a.|", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_digit_opposite_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\D", "0");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_space(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\s\\s\\s", " \t\n");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, " \t\n", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_space_unicode(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    // unicode character types not supported
	Cob_result mr = match_run(test, "\\s", " ");  // Ogham Space Mark : \u1680
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_space_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\s", "a");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_space_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\S\\S\\S", "abc");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "abc", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_space_opposite_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\S", " ");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_escape_newline(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\\n", "\n");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "\n", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_escape_newline_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\\n", "x");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_newline_opposite(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\N", "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_character_type_newline_opposite_not_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\N", "\n");
    Zinc_test_expect_false(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 0, "count groups");
    match_teardown(&mr);
}

void CobUnit_match_scan_all(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    struct Zinc_string_list* groups = NULL;
    Cob_result mr = match_run(test, "a", "za");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_word_type(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, "\\w\\w\\w", "a0_");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 1, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a0_", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_coverage_line(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "\\s*(\\-|\\d+):\\s*(\\d+):Source:(.+)",
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 4, "count groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        0,
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        1,
        "-", "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        2,
        "0", "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        3,
        "/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_coverage_line2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "\\s*(\\-):\\s*(\\d+):(Source:)?(.+)",
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 5, "count groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        0,
        "        -:    0:Source:/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        1,
        "-", "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        2,
        "0", "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        3,
        "Source:", "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        4,
        "/home/miguel/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_coverage_line3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "\\s*(\\-):\\s*(\\d+):(Source:)?(.+)",
        "        -:    1:#include \"zinc/unit_test.h\"");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 5, "count groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        0,
        "        -:    1:#include \"zinc/unit_test.h\"",
        "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        1,
        "-",
        "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        2,
        "1",
        "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        3,
        "",
        "item groups");
    Zinc_test_expect_buffer_list_item(test, 
        &mr.groups,
        4,
        "#include \"zinc/unit_test.h\"",
        "item groups");
    match_teardown(&mr);
}

void CobUnit_match_missing_group(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "(c|xyx()|a)",
        "a");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 3, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "a", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "a", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 2, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_group_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "()",
        "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 2, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_group_closure_empty(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "(a*)",
        "");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 2, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_greedy(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "(a{1,2})(a*)",
        "aa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 3, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aa", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "aa", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 2, "", "item groups");
    match_teardown(&mr);
}

void CobUnit_match_repeat_range_greedy2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Cob_result mr = match_run(test, 
        "(a{1,2})(a*)",
        "aaa");
    Zinc_test_expect_true(test, mr.matched, "m");
    Zinc_test_expect_buffer_list_count(test, &mr.groups, 3, "count groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 0, "aaa", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 1, "aa", "item groups");
    Zinc_test_expect_buffer_list_item(test, &mr.groups, 2, "a", "item groups");
    match_teardown(&mr);
}

void CobUnit_match(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CobUnit_match_empty);

        Zinc_test_register(test, CobUnit_match_literal);
        Zinc_test_register(test, CobUnit_match_literal_not_match);

        Zinc_test_register(test, CobUnit_match_concat);
        Zinc_test_register(test, CobUnit_match_concat_not_match);
        Zinc_test_register(test, CobUnit_match_concat_not_match2);

        Zinc_test_register(test, CobUnit_match_union1);
        Zinc_test_register(test, CobUnit_match_union2);
        Zinc_test_register(test, CobUnit_match_union_not_match1);
        Zinc_test_register(test, CobUnit_match_union3);
        Zinc_test_register(test, CobUnit_match_union_cat1);
        Zinc_test_register(test, CobUnit_match_union_cat2);

        Zinc_test_register(test, CobUnit_match_closure_zero);
        Zinc_test_register(test, CobUnit_match_closure_one);
        Zinc_test_register(test, CobUnit_match_closure_two);
        Zinc_test_register(test, CobUnit_match_closure_three);
        Zinc_test_register(test, CobUnit_match_closure_concat);

        Zinc_test_register(test, CobUnit_match_group);
        Zinc_test_register(test, CobUnit_match_group2);

        Zinc_test_register(test, CobUnit_match_union_backtrack);
        Zinc_test_register(test, CobUnit_match_closure_backtrack);

        Zinc_test_register(test, CobUnit_match_pos_closure_one);
        Zinc_test_register(test, CobUnit_match_pos_closure_two);
        Zinc_test_register(test, CobUnit_match_pos_closure_not_match);
        Zinc_test_register(test, CobUnit_match_pos_closure_backtrack1);

        Zinc_test_register(test, CobUnit_match_repeat_zero);
        Zinc_test_register(test, CobUnit_match_repeat_zero2);
        Zinc_test_register(test, CobUnit_match_repeat_one);
        Zinc_test_register(test, CobUnit_match_repeat_one_not_match);
        Zinc_test_register(test, CobUnit_match_repeat_two);
        Zinc_test_register(test, CobUnit_match_repeat_two_not_match);
        Zinc_test_register(test, CobUnit_match_repeat_two_not_match_short);

        Zinc_test_register(test, CobUnit_match_repeat_range_zero_zero);
        Zinc_test_register(test, CobUnit_match_repeat_range_zero_zero_more);
        Zinc_test_register(test, CobUnit_match_repeat_range_zero_one);
        Zinc_test_register(test, CobUnit_match_repeat_range_one_one);
        Zinc_test_register(test, CobUnit_match_repeat_range_two_four_two);
        Zinc_test_register(test, CobUnit_match_repeat_range_two_four_three);
        Zinc_test_register(test, CobUnit_match_repeat_range_two_four_four);
        Zinc_test_register(test, CobUnit_match_repeat_range_two_four_not_match_less);
        Zinc_test_register(test, CobUnit_match_repeat_range_two_four_more);

        Zinc_test_register(test, CobUnit_match_option_zero);
        Zinc_test_register(test, CobUnit_match_option_one_not_match);
        Zinc_test_register(test, CobUnit_match_option_one);
        Zinc_test_register(test, CobUnit_match_option_two);
        Zinc_test_register(test, CobUnit_match_option_two_not_match2);

        Zinc_test_register(test, CobUnit_match_wildcard_one_one);
        Zinc_test_register(test, CobUnit_match_wildcard_one_one2);
        Zinc_test_register(test, CobUnit_match_wildcard_one_zero_not_matched);
        Zinc_test_register(test, CobUnit_match_wildcard_two_two);
        Zinc_test_register(test, CobUnit_match_wildcard_three_three);
        Zinc_test_register(test, CobUnit_match_wildcard_newline_not_matched);

        Zinc_test_register(test, CobUnit_match_begin_one);
        Zinc_test_register(test, CobUnit_match_begin_zero);
        Zinc_test_register(test, CobUnit_match_begin_one_not_match);

        Zinc_test_register(test, CobUnit_match_end_zero);
        Zinc_test_register(test, CobUnit_match_end_line);
        Zinc_test_register(test, CobUnit_match_end_line_not_match);

        Zinc_test_register(test, CobUnit_match_escape_one_backslash);
        Zinc_test_register(test, CobUnit_match_escape_one_closure);
        Zinc_test_register(test, CobUnit_match_escape_two);
        Zinc_test_register(test, CobUnit_match_escape_one_not_match);

        Zinc_test_register(test, CobUnit_match_character_class_one);
        Zinc_test_register(test, CobUnit_match_character_class_one_not_match);
        Zinc_test_register(test, CobUnit_match_character_class_two);
        Zinc_test_register(test, CobUnit_match_character_class_two2);
        Zinc_test_register(test, CobUnit_match_character_class_three);

        Zinc_test_register(test, CobUnit_match_character_class_range);
        Zinc_test_register(test, CobUnit_match_character_class_range2);
        Zinc_test_register(test, CobUnit_match_character_class_range3);
        Zinc_test_register(test, CobUnit_match_character_class_range_not_match);

        Zinc_test_register(test, CobUnit_match_character_class_opposite);
        Zinc_test_register(test, CobUnit_match_character_class_opposite_not_match);

        Zinc_test_register(test, CobUnit_match_character_class_opposite_range);
        Zinc_test_register(test, CobUnit_match_character_class_opposite_range_not_match);

        Zinc_test_register(test, CobUnit_match_character_type_word);
        Zinc_test_register(test, CobUnit_match_character_type_word2);
        Zinc_test_register(test, CobUnit_match_character_type_word3);
        Zinc_test_register(test, CobUnit_match_character_type_word4);
        Zinc_test_register(test, CobUnit_match_character_type_word5);
        Zinc_test_register(test, CobUnit_match_character_type_word6);
        Zinc_test_register(test, CobUnit_match_character_type_word7);
        Zinc_test_register(test, CobUnit_match_character_type_word_unicode);
        Zinc_test_register(test, CobUnit_match_character_type_word_not_match);

        Zinc_test_register(test, CobUnit_match_character_type_word_opposite);
        Zinc_test_register(test, CobUnit_match_character_type_word_opposite_not_match);

        Zinc_test_register(test, CobUnit_match_character_type_digit);
        Zinc_test_register(test, CobUnit_match_character_type_digit2);
        Zinc_test_register(test, CobUnit_match_character_type_digit3);
        Zinc_test_register(test, CobUnit_match_character_type_digit_unicode);
        Zinc_test_register(test, CobUnit_match_character_type_digit_not_match);

        Zinc_test_register(test, CobUnit_match_character_type_digit_opposite);
        Zinc_test_register(test, CobUnit_match_character_type_digit_opposite_not_match);

        Zinc_test_register(test, CobUnit_match_character_type_space);
        Zinc_test_register(test, CobUnit_match_character_type_space_unicode);
        Zinc_test_register(test, CobUnit_match_character_type_space_not_match);

        Zinc_test_register(test, CobUnit_match_character_type_space_opposite);
        Zinc_test_register(test, CobUnit_match_character_type_space_opposite_not_match);

        Zinc_test_register(test, CobUnit_match_escape_newline);
        Zinc_test_register(test, CobUnit_match_escape_newline_not_match);
        Zinc_test_register(test, CobUnit_match_character_type_newline_opposite);
        Zinc_test_register(test, CobUnit_match_character_type_newline_opposite_not_match);

        Zinc_test_register(test, CobUnit_match_scan_all);

        Zinc_test_register(test, CobUnit_match_word_type);

        Zinc_test_register(test, CobUnit_match_coverage_line);
        Zinc_test_register(test, CobUnit_match_coverage_line2);
        Zinc_test_register(test, CobUnit_match_coverage_line3);

        Zinc_test_register(test, CobUnit_match_missing_group);

        Zinc_test_register(test, CobUnit_match_group_empty);
        Zinc_test_register(test, CobUnit_match_group_closure_empty);
        Zinc_test_register(test, CobUnit_match_repeat_range_greedy);
        Zinc_test_register(test, CobUnit_match_repeat_range_greedy2);

        return;
    }

    Zinc_test_perform(test);
}
