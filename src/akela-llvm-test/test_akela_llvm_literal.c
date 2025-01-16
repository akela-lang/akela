#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_akela_llvm_literal_integer()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("1", &result);
    Zinc_expect_str(&result.value, "1", "1");

    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_literal_double()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    struct Zinc_string v_exp;
    Zinc_string_init(&v_exp);
    Zinc_string_add_format(&v_exp, "%lf", 1.5);
    Zinc_string_finish(&v_exp);

    cg_setup("1.5", &result);
    Zinc_expect_str(&result.value, v_exp.buf, "1.5");

    Zinc_string_destroy(&v_exp);
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_boolean_true()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("true",
             &result);
    Zinc_expect_str(&result.value, "true", "true");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_boolean_false()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("false",
             &result);
    Zinc_expect_str(&result.value, "false", "false");

    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_literal()
{
    test_akela_llvm_literal_integer();
    test_akela_llvm_literal_double();
    test_code_gen_boolean_true();
    test_code_gen_boolean_false();
}