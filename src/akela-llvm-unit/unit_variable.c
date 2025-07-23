#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_variable_int64(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int64 = 2\n"
             "a\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_int32(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 2\n"
             "a\n",
             &result);
    Zinc_expect_string(test, &result.value, "2", "value");
    Zinc_expect_no_errors(test, &result.cu->errors);
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_int32_int64(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 1\n"
             "const b: Int64 = a\n"
             "b\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_assign_scalar_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 11\n"
             "var b: Int32\n"
             "b = a\n"
             "b\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "11", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_assign_eseq(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup(
        "var a: Int32\n"
        "var b: Int32\n"
        "var c: Int32\n"
        "var d: Int32\n"
        "a, b, c, d = 1, 2, 3, 4\n"
        "a",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup(
        "var a: Int32\n"
        "var b: Int32\n"
        "var c: Int32\n"
        "var d: Int32\n"
         "a, b, c, d = 1, 2, 3, 4\n"
             "b",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup(
        "var a: Int32\n"
        "var b: Int32\n"
        "var c: Int32\n"
        "var d: Int32\n"
        "a, b, c, d = 1, 2, 3, 4\n"
        "c",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup(
        "var a: Int32\n"
        "var b: Int32\n"
        "var c: Int32\n"
        "var d: Int32\n"
         "a, b, c, d = 1, 2, 3, 4\n"
             "d",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);

}

void AkeLlvmUnit_variable(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_variable_int64);
        Zinc_test_register(test, AkeLlvmUnit_variable_int32);
        Zinc_test_register(test, AkeLlvmUnit_variable_int32_int64);
        Zinc_test_register(test, AkeLlvmUnit_variable_assign_scalar_id);
        Zinc_test_register(test, AkeLlvmUnit_variable_assign_eseq);

        return;
    }

    Zinc_test_perform(test);
}