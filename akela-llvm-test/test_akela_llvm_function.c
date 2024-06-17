#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_function()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)\n"
             "end\n",
             &result);
    expect_str(&result.value, "Function", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_function_ret()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->i64\n"
             "  a + b + c\n"
             "end\n",
             &result);
    expect_str(&result.value, "Function", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_call()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->i64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "6", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_call_ptr()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let foo: fn (i64, i64, i64)->i64 = fn (a: i64, b: i64, c: i64)->i64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "6", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_call2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->bool\n"
             "  true\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "true", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_call3()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->[6 const]u8\n"
             "  \"hello\"\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "hello", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_anonymous_function()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let foo: fn (i64, i64, i64)->i64 = fn (a: i64, b: i64, c: i64)->i64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 1, 1)\n",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_extern()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("extern pow(x: f64, y: f64)->f64\n"
             "pow(5.0, 2.0)\n",
             &result);
    expect_str(&result.value, "25.000000", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_function_id()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo()->i64 1 end\n"
             "let bar: fn ()->i64\n"
             "bar = foo\n"
             "bar()\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_function_id2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo()->i64 1 end\n"
             "fn bar()->i64 2 end\n"
             "foo = bar\n"
             "foo()\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_function_copy()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo(a: i64)->i64\n"
             "  a + 1\n"
             "end\n"
             "let a: fn (i64)->i64 = foo\n"
             "a(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_function_expression()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn foo(a: i64)->i64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_anonymous_function_expression()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("fn (a: i64)->i64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_function()
{
    test_code_gen_function_copy();
    test_code_gen_function_expression();
    test_code_gen_anonymous_function_expression();
    test_code_gen_assign_function_id();
    test_code_gen_function();
    test_code_gen_function_ret();
    test_code_gen_call();
    test_code_gen_call_ptr();
    test_code_gen_call2();
    test_code_gen_call3();
    test_code_gen_anonymous_function();
    test_code_gen_extern();
    test_code_gen_assign_function_id2();
}