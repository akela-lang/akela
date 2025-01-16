#include <string.h>

#include "zinc/unit_test.h"
#include "coverage/parse.h"
#include "cobble/match.h"
#include "zinc/string_list.h"

void test_parse_line_zero()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_gcov_line_re();
    char s[] = "        -:    0:Source:/home/username/workspace/trade/akela/coverage-test/test_data.c";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 5, "count groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        0,
        "        -:    0:Source:/home/username/workspace/trade/akela/coverage-test/test_data.c", "item groups");
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
        "/home/username/workspace/trade/akela/coverage-test/test_data.c", "item groups");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_not_covered_line()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_gcov_line_re();
    char s[] = "        -:    1:#include \"zinc/unit_test.h\"";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

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

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_covered_line()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_gcov_line_re();
    char s[] = "1:    4:void test_data_file_list_add()";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 5, "count groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        0,
        "1:    4:void test_data_file_list_add()",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        1,
        "1",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        2,
        "4",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        3,
        "",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        4,
        "void test_data_file_list_add()",
        "item groups");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_covered_line2()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_gcov_line_re();
    char s[] = "       1*:    9:    expect_true(is_word(\"x  \", NUM_BYTES(\"x\"[0])), \"ascii letter\");";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 5, "count groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        0,
        "       1*:    9:    expect_true(is_word(\"x  \", NUM_BYTES(\"x\"[0])), \"ascii letter\");",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        1,
        "1",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        2,
        "9",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        3,
        "",
        "item groups");
    Zinc_expect_buffer_list_item(
        &mr.groups,
        4,
        "    expect_true(is_word(\"x  \", NUM_BYTES(\"x\"[0])), \"ascii letter\");",
        "item groups");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_gcov_ext1()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_gcov_ext_re();
    char s[] = ".gcov";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, ".gcov", "item groups");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_gcov_ext2()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_gcov_ext_re();
    char s[] = "ast.c.gcov";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, ".gcov", "item groups");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_gcov_ext3_not_match()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_gcov_ext_re();
    char s[] = "abc.gcovabc";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_false(mr.matched, "m");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_test_dir1()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_test_dir_re();
    char s[] = "-test";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "-test", "item groups");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_test_dir2()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_test_dir_re();
    char s[] = "akela-test";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_true(mr.matched, "m");
    Zinc_expect_buffer_list_count(&mr.groups, 1, "count groups");
    Zinc_expect_buffer_list_item(&mr.groups, 0, "-test", "item groups");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_test_dir3_not_match()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_test_dir_re();
    char s[] = "akela";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_false(mr.matched, "m");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse_test_dir4_not_match()
{
    Zinc_test_name(__func__);

    Cob_re cr = Cover_test_dir_re();
    char s[] = "akela-test-abc";
    Zinc_string_slice slice;
    slice.p = s;
    slice.size = strlen(s);

    Cob_result mr = Cob_match(&cr, slice);

    Zinc_expect_false(mr.matched, "m");

    Zinc_string_list_destroy(&mr.groups);
    Cover_re_cleanup(&cr);
}

void test_parse()
{
    test_parse_line_zero();
    test_parse_not_covered_line();
    test_parse_covered_line();
    test_parse_covered_line2();

    test_parse_gcov_ext1();
    test_parse_gcov_ext2();
    test_parse_gcov_ext3_not_match();

    test_parse_test_dir1();
    test_parse_test_dir2();
    test_parse_test_dir3_not_match();
    test_parse_test_dir4_not_match();
}