#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_akela_llvm_struct_1()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: Float64\n"
             "  y: Float64\n"
             "end\n"
             "let a: Point\n"
             "a.x = 1.0\n"
             "a.y = 2.0\n"
             "a.x\n",
             &result);
    expect_str(&result.value, "1.000000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: Float64\n"
             "  y: Float64\n"
             "end\n"
             "let a: Point\n"
             "a.x = 1.0\n"
             "a.y = 2.0\n"
             "a.y\n",
             &result);
    expect_str(&result.value, "2.000000", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct()
{
    test_akela_llvm_struct_1();
}