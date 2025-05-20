#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "unit_cg_tools.h"

void test_code_gen_const_void1()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32", &result);
    Zinc_expect_string(&result.value, "", "blank");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_const_void2()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32 = 1", &result);
    Zinc_expect_string(&result.value, "", "blank");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_const_int()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32 = 31\n"
             "a"
            ,&result);
    Zinc_expect_string(&result.value, "31", "31");

    Ake_code_gen_result_destroy(&result);
}

void test_akela_variable_const_assign_multiple()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c: Int32 = 1, 2, 3\n"
             "a\n"
            ,&result);
    Zinc_expect_string(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c: Int32 = 1, 2, 3\n"
             "b\n"
            ,&result);
    Zinc_expect_string(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c: Int32 = 1, 2, 3\n"
             "c\n"
            ,&result);
    Zinc_expect_string(&result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32\n"
             "a = 44\n",
             &result);
    Zinc_expect_string(&result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_multiple()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n"
             "a",
             &result);
    Zinc_expect_string(&result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n"
             "b",
             &result);
    Zinc_expect_string(&result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n"
             "c",
             &result);
    Zinc_expect_string(&result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n",
             &result);
    Zinc_expect_string(&result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_copy_from_variable()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 50\n"
             "const b: Int32 = a\n"
             "a + b\n",
             &result);
    Zinc_expect_string(&result.value, "100", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_int64()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int64 = 2\n"
             "a\n",
             &result);
    Zinc_expect_string(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_int32()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 2\n"
             "a\n",
             &result);
    Zinc_expect_string(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_int32_int64()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 1\n"
             "const b: Int64 = a\n"
             "b\n",
             &result);
    Zinc_expect_string(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_scalar_id()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 11\n"
             "const b: Int32\n"
             "b = a\n"
             "b\n",
             &result);
    Zinc_expect_string(&result.value, "11", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_eseq()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "a",
             &result);
    Zinc_expect_string(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "b",
             &result);
    Zinc_expect_string(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "c",
             &result);
    Zinc_expect_string(&result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "d",
             &result);
    Zinc_expect_string(&result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);

}

void test_akela_llvm_variable()
{
    test_code_gen_const_void1();
    test_code_gen_const_void2();
    test_code_gen_const_int();
    test_akela_variable_const_assign_multiple();
    test_code_gen_assign();
    test_code_gen_assign_multiple();
    test_code_gen_copy_from_variable();
    test_code_gen_int64();
    test_code_gen_int32();
    test_code_gen_int32_int64();
    test_code_gen_assign_scalar_id();
    test_code_gen_assign_eseq();
}