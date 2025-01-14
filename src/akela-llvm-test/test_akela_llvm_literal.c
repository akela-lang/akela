#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_akela_llvm_literal_integer()
{
    test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("1", &result);
    expect_str(&result.value, "1", "1");

    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_literal_double()
{
    test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    struct buffer v_exp;
    buffer_init(&v_exp);
    buffer_add_format(&v_exp, "%lf", 1.5);
    buffer_finish(&v_exp);

    cg_setup("1.5", &result);
    expect_str(&result.value, v_exp.buf, "1.5");

    buffer_destroy(&v_exp);
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_boolean_true()
{
    test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("true",
             &result);
    expect_str(&result.value, "true", "true");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_boolean_false()
{
    test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("false",
             &result);
    expect_str(&result.value, "false", "false");

    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_literal()
{
    test_akela_llvm_literal_integer();
    test_akela_llvm_literal_double();
    test_code_gen_boolean_true();
    test_code_gen_boolean_false();
}