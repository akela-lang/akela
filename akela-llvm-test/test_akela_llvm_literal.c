#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_akela_llvm_literal_integer()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("1", &result);
    expect_str(&result.value, "1", "1");

    CodeGenResultDestroy(&result);
}

void test_akela_llvm_literal_double()
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

    cg_setup("false",
             &result);
    expect_str(&result.value, "false", "false");

    CodeGenResultDestroy(&result);
}

void test_akela_llvm_literal()
{
    test_akela_llvm_literal_integer();
    test_akela_llvm_literal_double();
    test_code_gen_boolean_true();
    test_code_gen_boolean_false();
}