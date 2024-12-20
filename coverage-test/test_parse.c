#include <string.h>

#include "zinc/unit_test.h"
#include "coverage/parse.h"
#include "cobble/match.h"
#include "zinc/buffer_list.h"

void test_parse_line_zero()
{
    test_name(__func__);

    Cob_compile_result cr = Cov_gov_line_re();
    char s[] = "        -:    0:Source:/home/username/workspace/trade/akela/coverage-test/test_data.c";
    String_slice slice;
    slice.p = s;
    slice.size = strlen(s);
    struct buffer_list groups;
    buffer_list_init(&groups);

    bool matched = re_match(cr.root, slice, &groups);

    expect_true(matched, "m");
    expect_buffer_list_count(&groups, 5, "count groups");
    expect_buffer_list_item(
        &groups,
        0,
        "        -:    0:Source:/home/username/workspace/trade/akela/coverage-test/test_data.c", "item groups");
    expect_buffer_list_item(
        &groups,
        1,
        "-", "item groups");
    expect_buffer_list_item(
        &groups,
        2,
        "0", "item groups");
    expect_buffer_list_item(
        &groups,
        3,
        "Source:", "item groups");
    expect_buffer_list_item(
        &groups,
        4,
        "/home/username/workspace/trade/akela/coverage-test/test_data.c", "item groups");

    buffer_list_destroy(&groups);
    Cov_re_cleanup(&cr);
}

void test_parse_not_covered_line()
{
    test_name(__func__);

    Cob_compile_result cr = Cov_gov_line_re();
    char s[] = "        -:    1:#include \"zinc/unit_test.h\"";
    String_slice slice;
    slice.p = s;
    slice.size = strlen(s);
    struct buffer_list groups;
    buffer_list_init(&groups);

    bool matched = re_match(cr.root, slice, &groups);

    expect_true(matched, "m");
    expect_buffer_list_count(&groups, 5, "count groups");
    expect_buffer_list_item(
        &groups,
        0,
        "        -:    1:#include \"zinc/unit_test.h\"",
        "item groups");
    expect_buffer_list_item(
        &groups,
        1,
        "-",
        "item groups");
    expect_buffer_list_item(
        &groups,
        2,
        "1",
        "item groups");
    expect_buffer_list_item(
        &groups,
        3,
        "",
        "item groups");
    expect_buffer_list_item(
        &groups,
        4,
        "#include \"zinc/unit_test.h\"",
        "item groups");

    buffer_list_destroy(&groups);
    Cov_re_cleanup(&cr);
}

void test_parse_covered_line()
{
    test_name(__func__);

    Cob_compile_result cr = Cov_gov_line_re();
    char s[] = "1:    4:void test_data_file_list_add()";
    String_slice slice;
    slice.p = s;
    slice.size = strlen(s);
    struct buffer_list groups;
    buffer_list_init(&groups);

    bool matched = re_match(cr.root, slice, &groups);

    expect_true(matched, "m");
    expect_buffer_list_count(&groups, 5, "count groups");
    expect_buffer_list_item(
        &groups,
        0,
        "1:    4:void test_data_file_list_add()",
        "item groups");
    expect_buffer_list_item(
        &groups,
        1,
        "1",
        "item groups");
    expect_buffer_list_item(
        &groups,
        2,
        "4",
        "item groups");
    expect_buffer_list_item(
        &groups,
        3,
        "",
        "item groups");
    expect_buffer_list_item(
        &groups,
        4,
        "void test_data_file_list_add()",
        "item groups");

    buffer_list_destroy(&groups);
    Cov_re_cleanup(&cr);
}

void test_parse_covered_line2()
{
    test_name(__func__);

    Cob_compile_result cr = Cov_gov_line_re();
    char s[] = "       1*:    9:    expect_true(is_word(\"x  \", NUM_BYTES(\"x\"[0])), \"ascii letter\");";
    String_slice slice;
    slice.p = s;
    slice.size = strlen(s);
    struct buffer_list groups;
    buffer_list_init(&groups);

    bool matched = re_match(cr.root, slice, &groups);

    expect_true(matched, "m");
    expect_buffer_list_count(&groups, 5, "count groups");
    expect_buffer_list_item(
        &groups,
        0,
        "       1*:    9:    expect_true(is_word(\"x  \", NUM_BYTES(\"x\"[0])), \"ascii letter\");",
        "item groups");
    expect_buffer_list_item(
        &groups,
        1,
        "1",
        "item groups");
    expect_buffer_list_item(
        &groups,
        2,
        "9",
        "item groups");
    expect_buffer_list_item(
        &groups,
        3,
        "",
        "item groups");
    expect_buffer_list_item(
        &groups,
        4,
        "    expect_true(is_word(\"x  \", NUM_BYTES(\"x\"[0])), \"ascii letter\");",
        "item groups");

    buffer_list_destroy(&groups);
    Cov_re_cleanup(&cr);
}

void test_parse()
{
    test_parse_line_zero();
    test_parse_not_covered_line();
    test_parse_covered_line();
    test_parse_covered_line2();
}