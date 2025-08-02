#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_array_assign_sub_array(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4][4]Int32 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "var b: [4]Int32\n"
             "b = a[1]\n"
             "b[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4][4]Int32 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "var b: [4]Int32\n"
             "b = a[1]\n"
             "b[1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "6", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4][4]Int32 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "var b: [4]Int32\n"
             "b = a[1]\n"
             "b[2]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "7", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4][4]Int32 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "var b: [4]Int32\n"
             "b = a[1]\n"
             "b[3]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "8", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_array_subscript_exit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Int32 = [1,2,3,4]\n"
             "a[4]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_true(test, true, "exit");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_array_assign_subscript(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [4][4]Int32 = [[1,2,3,4],\n"
             "  [5,6,7,8],\n"
             "  [9,10,11,12],\n"
             "  [13,14,15,16]\n"
             "]\n"
             "const b: [4]Int32 = [50,60,70,80]\n"
             "a[1] = b\n"
             "a[1][0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "50", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_array(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_array_assign_sub_array);
        //Zinc_test_register(test, AkeLlvmUnit_array_subscript_exit);
        Zinc_test_register(test, AkeLlvmUnit_array_assign_subscript);

        return;
    }

    Zinc_test_perform(test);
}