#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_variable_const_void1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32", &result);
    Zinc_expect_string(test, &result.value, "", "blank");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_const_void2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32 = 1", &result);
    Zinc_expect_string(test, &result.value, "", "blank");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_const_int(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32 = 31\n"
             "a"
            ,&result);
    Zinc_expect_string(test, &result.value, "31", "31");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_const_assign_multiple(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c: Int32 = 1, 2, 3\n"
             "a\n"
            ,&result);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c: Int32 = 1, 2, 3\n"
             "b\n"
            ,&result);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c: Int32 = 1, 2, 3\n"
             "c\n"
            ,&result);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_assign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: Int32\n"
             "a = 44\n",
             &result);
    Zinc_expect_string(test, &result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_assign_multiple(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n"
             "a",
             &result);
    Zinc_expect_string(test, &result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n"
             "b",
             &result);
    Zinc_expect_string(test, &result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n"
             "c",
             &result);
    Zinc_expect_string(test, &result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a,b,c: Int32\n"
             "a = b = c = 44\n",
             &result);
    Zinc_expect_string(test, &result.value, "44", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_variable_copy_from_variable(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = 50\n"
             "const b: Int32 = a\n"
             "a + b\n",
             &result);
    Zinc_expect_string(test, &result.value, "100", "value");
    Ake_code_gen_result_destroy(&result);
}

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
             "const b: Int32\n"
             "b = a\n"
             "b\n",
             &result);
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
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "a",
             &result);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "b",
             &result);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "c",
             &result);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a, b, c, d: Int32\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "d",
             &result);
    Zinc_expect_string(test, &result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);

}

void AkeLlvmUnit_variable(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_variable_const_void1);
        Zinc_test_register(test, AkeLlvmUnit_variable_const_void2);
        Zinc_test_register(test, AkeLlvmUnit_variable_const_int);
        Zinc_test_register(test, AkeLlvmUnit_variable_const_assign_multiple);
        Zinc_test_register(test, AkeLlvmUnit_variable_assign);
        Zinc_test_register(test, AkeLlvmUnit_variable_assign_multiple);
        Zinc_test_register(test, AkeLlvmUnit_variable_copy_from_variable);
        Zinc_test_register(test, AkeLlvmUnit_variable_int64);
        Zinc_test_register(test, AkeLlvmUnit_variable_int32);
        Zinc_test_register(test, AkeLlvmUnit_variable_int32_int64);
        Zinc_test_register(test, AkeLlvmUnit_variable_assign_scalar_id);
        Zinc_test_register(test, AkeLlvmUnit_variable_assign_eseq);

        return;
    }

    Zinc_test_perform(test);
}