#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_function_function_param(Zinc_test* test)
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
            "fn add_one(x: Int32)->Int32\n"
            "  x + 1\n"
            "end\n"
            "fn add_two(x: Int32)->Int32\n"
            "  x + 2\n"
            "end\n"
            "fn compute(foo: fn(Int32)->Int32, x: Int32)->Int32\n"
            "  foo(x)\n"
            "end\n"
            "compute(add_one, 15)\n",
            &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "16", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup(
            "fn add_one(x: Int32)->Int32\n"
            "  x + 1\n"
            "end\n"
            "fn add_two(x: Int32)->Int32\n"
            "  x + 2\n"
            "end\n"
            "fn compute(foo: fn(Int32)->Int32, x: Int32)->Int32\n"
            "  foo(x)\n"
            "end\n"
            "compute(add_two, 15)\n",
            &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "17", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_in_array(Zinc_test* test)
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
            "var func_array: [5]fn(Int32)->Int32\n"
            "func_array[0] = fn (x: Int32)->Int32 x*2 end\n"
            "func_array[0](10)",
            &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "20", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_function_function_param);
        Zinc_test_register(test, AkeLlvmUnit_function_in_array);
        
        return;
    }

    Zinc_test_perform(test);
}