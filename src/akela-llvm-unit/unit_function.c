#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_function_declare(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo(a: Int32, b: Int32, c: Int32)\n"
             "end\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "Function", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_declare_ret(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo(a: Int32, b: Int32, c: Int32)->Int32\n"
             "  a + b + c\n"
             "end\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "Function", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_call(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo(a: Int32, b: Int32, c: Int32)->Int32\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "6", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_anonymous_call(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const foo: fn (Int32, Int32, Int32)->Int32 = fn (a: Int32, b: Int32, c: Int32)->Int32\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "6", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_ret_boolean(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo(a: Int32, b: Int32, c: Int32)->Bool\n"
             "  true\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "true", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_ret_array(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo(a: Int32, b: Int32, c: Int32)->[6 const]Nat8\n"
             "  \"hello\"\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "hello", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_extern(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("extern pow(x: Real64, y: Real64)->Real64\n"
             "pow(5.0, 2.0)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "25.000000", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_assign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo()->Int32 1 end\n"
             "var bar: fn ()->Int32\n"
             "bar = foo\n"
             "bar()\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_assign2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo()->Int32 1 end\n"
             "fn bar()->Int32 2 end\n"
             "foo = bar\n"
             "foo()\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_const_assign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo(a: Int32)->Int32\n"
             "  a + 1\n"
             "end\n"
             "const a: fn (Int32)->Int32 = foo\n"
             "a(1)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_expression(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn foo(a: Int32)->Int32\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_anonymous_expression(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn (a: Int32)->Int32\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_array_param(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn add_ten(array: [10]Int32, i: Nat64)->Int32\n"
             "  array[i] + 10\n"
             "end\n"
             "const a: [10]Int32 = [100,200,300,400,500,600,700,800,900,1000]\n"
             "add_ten(a, 0)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "110", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn add_ten(array: [10]Int32, i: Nat64)->Int32\n"
             "  array[i] + 10\n"
             "end\n"
             "const a: [10]Int32 = [100,200,300,400,500,600,700,800,900,1000]\n"
             "add_ten(a, 1)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "210", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("fn add_ten(array: [10]Int32, i: Nat64)->Int32\n"
             "  array[i] + 10\n"
             "end\n"
             "const a: [10]Int32 = [100,200,300,400,500,600,700,800,900,1000]\n"
             "add_ten(a, 2)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "310", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_function_struct_param(Zinc_test* test)
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
            "struct Point\n"
            "  x: Real64\n"
            "  y: Real64\n"
            "end\n"
            "fn add_ten(p: Point)->Real64\n"
             "  p.x + 10.0\n"
             "end\n"
             "const p: Point = Point\n"
             "  x: 2.5\n"
             "  y: 3.5\n"
             "end\n"
             "add_ten(p)",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "12.500000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup(
            "struct Point\n"
            "  x: Real64\n"
            "  y: Real64\n"
            "end\n"
            "fn add_ten(p: Point)->Real64\n"
            "  p.y + 10.0\n"
            "end\n"
            "const p: Point = Point\n"
            "  x: 2.5\n"
            "  y: 3.5\n"
            "end\n"
            "add_ten(p)",
            &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "13.500000", "value");
    Ake_code_gen_result_destroy(&result);
}

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

        Zinc_test_register(test, AkeLlvmUnit_function_declare);
        Zinc_test_register(test, AkeLlvmUnit_function_declare_ret);
        Zinc_test_register(test, AkeLlvmUnit_function_call);
        Zinc_test_register(test, AkeLlvmUnit_function_anonymous_call);
        Zinc_test_register(test, AkeLlvmUnit_function_ret_boolean);
        Zinc_test_register(test, AkeLlvmUnit_function_ret_array);
        Zinc_test_register(test, AkeLlvmUnit_function_extern);
        Zinc_test_register(test, AkeLlvmUnit_function_assign);
        Zinc_test_register(test, AkeLlvmUnit_function_assign2);
        Zinc_test_register(test, AkeLlvmUnit_function_const_assign);
        Zinc_test_register(test, AkeLlvmUnit_function_expression);
        Zinc_test_register(test, AkeLlvmUnit_function_anonymous_expression);
        Zinc_test_register(test, AkeLlvmUnit_function_array_param);
        Zinc_test_register(test, AkeLlvmUnit_function_struct_param);
        Zinc_test_register(test, AkeLlvmUnit_function_function_param);
        Zinc_test_register(test, AkeLlvmUnit_function_in_array);
        
        return;
    }

    Zinc_test_perform(test);
}