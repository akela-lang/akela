#include "test_code_gen_tools.h"
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"

void test_code_gen_constant_integer()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("1", &value);
    expect_str(&value, "1", "1");

    buffer_destroy(&value);
}

void test_code_gen_constant_double()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    struct buffer v_exp;
    buffer_init(&v_exp);
    buffer_add_format(&v_exp, "%lf", 1.5);
    buffer_finish(&v_exp);

    cg_setup("1.5", &value);
    expect_str(&value, v_exp.buf, "1.5");

    buffer_destroy(&v_exp);
    buffer_destroy(&value);
}

void test_code_gen_constant_string()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("\"hello\"", &value);
    expect_str(&value, "hello", "hello");

    buffer_destroy(&value);
}

void test_code_gen_var_void1()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("var a::Int64", &value);
    expect_str(&value, "", "blank");

    buffer_destroy(&value);
}

void test_code_gen_var_void2()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("var a::Int64 = 1", &value);
    expect_str(&value, "", "blank");

    buffer_destroy(&value);
}

void test_code_gen_var_int()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("var a::Int64 = 31\n"
             "a"
             ,&value);
    expect_str(&value, "31", "31");

    buffer_destroy(&value);
}

void test_code_gen_boolean_true()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("true",
             &value);
    expect_str(&value, "true", "true");

    buffer_destroy(&value);
}

void test_code_gen_boolean_false()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("true",
             &value);
    expect_str(&value, "true", "true");

    buffer_destroy(&value);
}

void test_code_gen_add()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("1 + 40", &value);
    expect_str(&value, "41", "41");

    buffer_destroy(&value);
}

void test_code_gen_add2()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("var a::Int64 = 4\n"
             "a + 61\n",
             &value);
    expect_str(&value, "65", "65");

    buffer_destroy(&value);
}

void test_code_gen_sub()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("15-2", &value);
    expect_str(&value, "13", "13");

    buffer_destroy(&value);
}

void test_code_gen_sub2()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("var a::Int64 = 10\n"
             "a - 2\n",
             &value);
    expect_str(&value, "8", "8");

    buffer_destroy(&value);
}

void test_code_gen_last()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("var a::Int64 = 1\n"
             "var b::Int64 = 2\n"
             "a + b\n",
             &value);
    expect_str(&value, "3", "value");

    buffer_destroy(&value);
}

void test_code_gen_if()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("if true\n"
             "  1\n"
             "end\n",
             &value);
    expect_str(&value, "", "value");

    buffer_destroy(&value);
}

void test_code_gen_if_else()
{
    test_name(__func__);
    struct buffer value;

    buffer_init(&value);
    cg_setup("if true\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &value);
    expect_str(&value, "1", "1 value");
    buffer_destroy(&value);

    buffer_init(&value);
    cg_setup("if false\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &value);
    expect_str(&value, "2", "2 value");
    buffer_destroy(&value);
}

void test_code_gen_if_elseif_else()
{
    test_name(__func__);
    struct buffer value;

    buffer_init(&value);
    cg_setup("if true\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &value);
    expect_str(&value, "1", "value");
    buffer_destroy(&value);

    buffer_init(&value);
    cg_setup("if false\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &value);
    expect_str(&value, "2", "value");
    buffer_destroy(&value);

    buffer_init(&value);
    cg_setup("if false\n"
             "  1\n"
             "elseif false\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &value);
    expect_str(&value, "3", "value");
    buffer_destroy(&value);
}

void test_code_gen_assign()
{
    test_name(__func__);
    struct buffer value;

    buffer_init(&value);
    cg_setup("var a::Int64\n"
             "a = 44\n",
             &value);
    expect_str(&value, "44", "value");
    buffer_destroy(&value);
}

void test_code_gen_assign_multiple()
{
    test_name(__func__);
    struct buffer value;

    buffer_init(&value);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n"
             "a",
             &value);
    expect_str(&value, "44", "value");
    buffer_destroy(&value);

    buffer_init(&value);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n"
             "b",
             &value);
    expect_str(&value, "44", "value");
    buffer_destroy(&value);

    buffer_init(&value);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n"
             "c",
             &value);
    expect_str(&value, "44", "value");
    buffer_destroy(&value);

    buffer_init(&value);
    cg_setup("var a,b,c::Int64\n"
             "a = b = c = 44\n",
             &value);
    expect_str(&value, "44", "value");
    buffer_destroy(&value);
}

void test_code_gen_function()
{
    test_name(__func__);
    struct buffer value;

    buffer_init(&value);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)\n"
             "end\n",
             &value);
    expect_str(&value, "Function", "value");
    buffer_destroy(&value);
}

void test_code_gen_function_ret()
{
    test_name(__func__);
    struct buffer value;

    buffer_init(&value);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::Int64\n"
             "  a + b + c\n"
             "end\n",
             &value);
    expect_str(&value, "Function", "value");
    buffer_destroy(&value);
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
}
