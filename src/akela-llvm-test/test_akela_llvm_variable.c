#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_let_void1()
{
    test_name(__func__);
    Code_gen_result result;
    Code_gen_result_init(&result);

    cg_setup("let a: i64", &result);
    expect_str(&result.value, "", "blank");

    Code_gen_result_destroy(&result);
}

void test_code_gen_let_void2()
{
    test_name(__func__);
    Code_gen_result result;
    Code_gen_result_init(&result);

    cg_setup("let a: i64 = 1", &result);
    expect_str(&result.value, "", "blank");

    Code_gen_result_destroy(&result);
}

void test_code_gen_let_int()
{
    test_name(__func__);
    Code_gen_result result;
    Code_gen_result_init(&result);

    cg_setup("let a: i64 = 31\n"
             "a"
            ,&result);
    expect_str(&result.value, "31", "31");

    Code_gen_result_destroy(&result);
}

void test_akela_variable_let_assign_multiple()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a, b, c: i64 = 1, 2, 3\n"
             "a\n"
            ,&result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a, b, c: i64 = 1, 2, 3\n"
             "b\n"
            ,&result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a, b, c: i64 = 1, 2, 3\n"
             "c\n"
            ,&result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: i64\n"
             "a = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_multiple()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a,b,c: i64\n"
             "a = b = c = 44\n"
             "a",
             &result);
    expect_str(&result.value, "44", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a,b,c: i64\n"
             "a = b = c = 44\n"
             "b",
             &result);
    expect_str(&result.value, "44", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a,b,c: i64\n"
             "a = b = c = 44\n"
             "c",
             &result);
    expect_str(&result.value, "44", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a,b,c: i64\n"
             "a = b = c = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_copy_from_variable()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: i64 = 50\n"
             "let b: i64 = a\n"
             "a + b\n",
             &result);
    expect_str(&result.value, "100", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_int64()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: i64 = 2\n"
             "a\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_int32()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: i32 = 2\n"
             "a\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_int32_int64()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: i64 = 1\n"
             "let b: i32 = a\n"
             "b\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_scalar_id()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: i64 = 11\n"
             "let b: i64\n"
             "b = a\n"
             "b\n",
             &result);
    expect_str(&result.value, "11", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_eseq()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a, b, c, d: i64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "a",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a, b, c, d: i64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "b",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a, b, c, d: i64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "c",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a, b, c, d: i64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "d",
             &result);
    expect_str(&result.value, "4", "value");
    Code_gen_result_destroy(&result);

}

void test_akela_llvm_variable()
{
    test_code_gen_let_void1();
    test_code_gen_let_void2();
    test_code_gen_let_int();
    test_akela_variable_let_assign_multiple();
    test_code_gen_assign();
    test_code_gen_assign_multiple();
    test_code_gen_copy_from_variable();
    test_code_gen_int64();
    test_code_gen_int32();
    test_code_gen_int32_int64();
    test_code_gen_assign_scalar_id();
    test_code_gen_assign_eseq();
}