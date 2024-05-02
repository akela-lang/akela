#include "test_code_gen_tools.h"
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"

void test_code_gen_constant_integer()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("1", &result);
    expect_str(&result.value, "1", "1");

    CodeGenResultDestroy(&result);
}

void test_code_gen_constant_double()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    struct buffer v_exp;
    buffer_init(&v_exp);
    buffer_add_format(&v_exp, "%lf", 1.5);
    buffer_finish(&v_exp);

    cg_setup("1.5", &result);
    expect_str(&result.value, v_exp.buf, "1.5");

    buffer_destroy(&v_exp);
    CodeGenResultDestroy(&result);
}

void test_code_gen_constant_string()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("\"hello\"", &result);
    expect_str(&result.value, "\"hello\"", "hello");

    CodeGenResultDestroy(&result);
}

void test_code_gen_var_void1()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("var a::Int64", &result);
    expect_str(&result.value, "", "blank");

    CodeGenResultDestroy(&result);
}

void test_code_gen_var_void2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("var a::Int64 = 1", &result);
    expect_str(&result.value, "", "blank");

    CodeGenResultDestroy(&result);
}

void test_code_gen_var_int()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("var a::Int64 = 31\n"
             "a"
             ,&result);
    expect_str(&result.value, "31", "31");

    CodeGenResultDestroy(&result);
}

void test_code_gen_boolean_true()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("true",
             &result);
    expect_str(&result.value, "true", "true");

    CodeGenResultDestroy(&result);
}

void test_code_gen_boolean_false()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("true",
             &result);
    expect_str(&result.value, "true", "true");

    CodeGenResultDestroy(&result);
}

void test_code_gen_add()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("1 + 40", &result);
    expect_str(&result.value, "41", "41");

    CodeGenResultDestroy(&result);
}

void test_code_gen_add2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("var a::Int64 = 4\n"
             "a + 61\n",
             &result);
    expect_str(&result.value, "65", "65");

    CodeGenResultDestroy(&result);
}

void test_code_gen_sub()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("15-2", &result);
    expect_str(&result.value, "13", "13");

    CodeGenResultDestroy(&result);
}

void test_code_gen_sub2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("var a::Int64 = 10\n"
             "a - 2\n",
             &result);
    expect_str(&result.value, "8", "8");

    CodeGenResultDestroy(&result);
}

void test_code_gen_last()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("var a::Int64 = 1\n"
             "var b::Int64 = 2\n"
             "a + b\n",
             &result);
    expect_str(&result.value, "3", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_if()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("if true\n"
             "  1\n"
             "end\n",
             &result);
    expect_str(&result.value, "", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_if_else()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("if true\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    expect_str(&result.value, "1", "1 value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    expect_str(&result.value, "2", "2 value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_if_elseif_else()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("if true\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif false\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("var a::Int64\n"
             "a = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_multiple()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n"
             "a",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n"
             "b",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n"
             "c",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)\n"
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
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::Int64\n"
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
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::Int64\n"
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
    cg_setup("var foo::Function{Input{Int64,Int64,Int64},Output{Int64}} = function (a::Int64, b::Int64, c::Int64)::Int64\n"
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
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::Bool\n"
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
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::String\n"
             "  \"hello\"\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "\"hello\"", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_anonymous_function()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("var foo::Function{Input{Int64,Int64,Int64},Output{Int64}} = function (a::Int64, b::Int64, c::Int64)::Int64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 1, 1)\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_copy_from_variable()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("var a::Int64 = 50\n"
             "var b::Int64 = a\n"
             "a + b\n",
             &result);
    expect_str(&result.value, "100", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function_copy()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64)::Int64\n"
             "  a + 1\n"
             "end\n"
             "var a::Function{Input{Int64},Output{Int64}} = foo\n"
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
    cg_setup("function foo(a::Int64)::Int64\n"
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
    cg_setup("function (a::Int64)::Int64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen()
{
    test_code_gen_constant_integer();
    test_code_gen_constant_double();
    test_code_gen_constant_string();
    test_code_gen_var_void1();
    test_code_gen_var_void2();
    test_code_gen_var_int();
    test_code_gen_boolean_true();
    test_code_gen_boolean_false();
    test_code_gen_add();
    test_code_gen_add2();
    test_code_gen_sub();
    test_code_gen_sub2();
    test_code_gen_last();
    test_code_gen_if();
    test_code_gen_if_else();
    test_code_gen_if_elseif_else();
    test_code_gen_assign();
    test_code_gen_assign_multiple();
    test_code_gen_function();
    test_code_gen_function_ret();
    test_code_gen_call();
    test_code_gen_call_ptr();
    test_code_gen_call2();
    test_code_gen_call3();
    test_code_gen_anonymous_function();
    test_code_gen_copy_from_variable();
    test_code_gen_function_copy();
    test_code_gen_function_expression();
    test_code_gen_anonymous_function_expression();
}
