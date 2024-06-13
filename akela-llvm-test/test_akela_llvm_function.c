#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_function()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo(a: Int64, b: Int64, c: Int64)\n"
             "end\n",
             &result);
    expect_str(&result.value, "Function", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function_ret()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo(a: Int64, b: Int64, c: Int64)->Int64\n"
             "  a + b + c\n"
             "end\n",
             &result);
    expect_str(&result.value, "Function", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo(a: Int64, b: Int64, c: Int64)->Int64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "6", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call_ptr()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let foo: fn (Int64, Int64, Int64)->Int64 = fn (a: Int64, b: Int64, c: Int64)->Int64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "6", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo(a: Int64, b: Int64, c: Int64)->Bool\n"
             "  true\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "true", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call3()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo(a: Int64, b: Int64, c: Int64)->[6 const]UInt8\n"
             "  \"hello\"\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "hello", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_anonymous_function()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let foo: fn (Int64, Int64, Int64)->Int64 = fn (a: Int64, b: Int64, c: Int64)->Int64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 1, 1)\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_extern()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("extern pow(x: Float64, y: Float64)->Float64\n"
             "pow(5.0, 2.0)\n",
             &result);
    expect_str(&result.value, "25.000000", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_function_id()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo()->Int64 1 end\n"
             "let bar: fn ()->Int64\n"
             "bar = foo\n"
             "bar()\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_function_id2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo()->Int64 1 end\n"
             "fn bar()->Int64 2 end\n"
             "foo = bar\n"
             "foo()\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function_copy()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo(a: Int64)->Int64\n"
             "  a + 1\n"
             "end\n"
             "let a: fn (Int64)->Int64 = foo\n"
             "a(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function_expression()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn foo(a: Int64)->Int64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_anonymous_function_expression()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("fn (a: Int64)->Int64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
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