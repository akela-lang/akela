#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_code_gen_array_literal(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [5]Int32 = [1,2,3,4,5]\n"
             "a[3]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [5]Int32 = [1,2,3,4,5]\n"
             "a[4]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_array_literal_multidimensional(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [2][2]Int32 = [[1,2],[3,4]]\n"
             "a[0][0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [2][2]Int32 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [2][2]Int32 = [[1,2],[3,4]]\n"
             "a[1][0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [2][2]Int32 = [[1,2],[3,4]]\n"
             "a[1][1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_array_literal_signed(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [2][2]Int32 = [[1,-2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "-2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_array_literal_int32(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [2][2]Int32 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_array_literal_ptr(Zinc_test* test)
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
             "const b: [4]Int32 = a\n"
             "b[1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_array_boolean(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Bool = [true,false,true,false]\n"
             "a[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "true", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Bool = [true,false,true,false]\n"
             "a[1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "false", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Bool = [true,false,true,false]\n"
             "a[2]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "true", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Bool = [true,false,true,false]\n"
             "a[3]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "false", "value");
    Ake_code_gen_result_destroy(&result);

}

void AkeLlvmUnit_code_gen_array_float(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1.000000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2.000000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[2]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "3.000000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Real64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[3]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4.000000", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_assign_array_id2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [4]Int32 = [1, 2, 3, 4]\n"
             "const b: [4]Int32 = [5, 6, 7, 8]\n"
             "a = b\n"
             "a[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_assign_array_subscript(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [4]Int32 = [1, 2, 3, 4]\n"
             "a[0] = 10\n"
             "a[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "10", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_assign_array_subscript2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][2]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [2][3]Int32 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][2]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "60", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_assign_array_allocate(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [4]Int32\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [4]Int32\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [4]Int32\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[2]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("var a: [4]Int32\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[3]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_code_gen_assign_array_id(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Int32 = [1, 2, 3, 4]\n"
             "var b: [4]Int32\n"
             "b = a\n"
             "b[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

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

        Zinc_test_register(test, AkeLlvmUnit_code_gen_array_literal);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_array_literal_multidimensional);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_array_literal_signed);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_array_literal_int32);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_array_literal_ptr);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_array_boolean);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_array_float);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_assign_array_id);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_assign_array_id2);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_assign_array_subscript);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_assign_array_subscript2);
        Zinc_test_register(test, AkeLlvmUnit_code_gen_assign_array_allocate);
        Zinc_test_register(test, AkeLlvmUnit_array_assign_sub_array);
        //Zinc_test_register(test, AkeLlvmUnit_array_subscript_exit);
        Zinc_test_register(test, AkeLlvmUnit_array_assign_subscript);

        return;
    }

    Zinc_test_perform(test);
}