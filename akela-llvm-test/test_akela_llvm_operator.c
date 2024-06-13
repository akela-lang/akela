#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

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

    cg_setup("let a: Int64 = 4\n"
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

    cg_setup("let a: Int64 = 10\n"
             "a - 2\n",
             &result);
    expect_str(&result.value, "8", "8");

    CodeGenResultDestroy(&result);
}

void test_akela_llvm_operator()
{
    test_code_gen_add();
    test_code_gen_add2();
    test_code_gen_sub();
    test_code_gen_sub2();
}