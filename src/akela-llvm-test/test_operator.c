#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_cg_tools.h"

void test_code_gen_add()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("1 + 40", &result);
    Zinc_expect_str(&result.value, "41", "41");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_add2()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("let a: Int32 = 4\n"
             "a + 61\n",
             &result);
    Zinc_expect_str(&result.value, "65", "65");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_sub()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("15-2", &result);
    Zinc_expect_str(&result.value, "13", "13");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_sub2()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("let a: Int32 = 10\n"
             "a - 2\n",
             &result);
    Zinc_expect_str(&result.value, "8", "8");

    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_operator()
{
    test_code_gen_add();
    test_code_gen_add2();
    test_code_gen_sub();
    test_code_gen_sub2();
}