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

void test_code_gen_var_void()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("var a::Int64 = 1", &value);
    expect_str(&value, "", "blank");

    buffer_destroy(&value);
}

void test_code_gen()
{
    test_code_gen_constant_integer();
    test_code_gen_constant_double();
    test_code_gen_constant_string();
    test_code_gen_var_void();
}
