#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_akela_llvm_function_declare()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "Function", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_declare_ret()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->i64\n"
             "  a + b + c\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "Function", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_call()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->i64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_str(&result.value, "6", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_anonymous_call()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let foo: fn (i64, i64, i64)->i64 = fn (a: i64, b: i64, c: i64)->i64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_str(&result.value, "6", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_ret_boolean()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->bool\n"
             "  true\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_str(&result.value, "true", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_ret_array()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo(a: i64, b: i64, c: i64)->[6 const]u8\n"
             "  \"hello\"\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    Zinc_expect_str(&result.value, "hello", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_extern()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("extern pow(x: f64, y: f64)->f64\n"
             "pow(5.0, 2.0)\n",
             &result);
    Zinc_expect_str(&result.value, "25.000000", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_assign()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo()->i64 1 end\n"
             "let bar: fn ()->i64\n"
             "bar = foo\n"
             "bar()\n",
             &result);
    Zinc_expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_assign2()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo()->i64 1 end\n"
             "fn bar()->i64 2 end\n"
             "foo = bar\n"
             "foo()\n",
             &result);
    Zinc_expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_let_assign()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo(a: i64)->i64\n"
             "  a + 1\n"
             "end\n"
             "let a: fn (i64)->i64 = foo\n"
             "a(1)\n",
             &result);
    Zinc_expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_expression()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn foo(a: i64)->i64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    Zinc_expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_anonymous_expression()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn (a: i64)->i64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    Zinc_expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_array_param()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("fn add_ten(array: [10]i64, i: i64)->i64\n"
             "  array[i] + 10\n"
             "end\n"
             "let a: [10]i64 = [100,200,300,400,500,600,700,800,900,1000]\n"
             "add_ten(a, 0)\n",
             &result);
    Zinc_expect_str(&result.value, "110", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("fn add_ten(array: [10]i64, i: i64)->i64\n"
             "  array[i] + 10\n"
             "end\n"
             "let a: [10]i64 = [100,200,300,400,500,600,700,800,900,1000]\n"
             "add_ten(a, 1)\n",
             &result);
    Zinc_expect_str(&result.value, "210", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("fn add_ten(array: [10]i64, i: i64)->i64\n"
             "  array[i] + 10\n"
             "end\n"
             "let a: [10]i64 = [100,200,300,400,500,600,700,800,900,1000]\n"
             "add_ten(a, 2)\n",
             &result);
    Zinc_expect_str(&result.value, "310", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_struct_param()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup(
            "struct Point\n"
            "  x: f64\n"
            "  y: f64\n"
            "end\n"
            "fn add_ten(p: Point)->f64\n"
             "  p.x + 10.0\n"
             "end\n"
             "let p: Point = Point\n"
             "  x: 2.5\n"
             "  y: 3.5\n"
             "end\n"
             "add_ten(p)",
             &result);
    Zinc_expect_str(&result.value, "12.500000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup(
            "struct Point\n"
            "  x: f64\n"
            "  y: f64\n"
            "end\n"
            "fn add_ten(p: Point)->f64\n"
            "  p.y + 10.0\n"
            "end\n"
            "let p: Point = Point\n"
            "  x: 2.5\n"
            "  y: 3.5\n"
            "end\n"
            "add_ten(p)",
            &result);
    Zinc_expect_str(&result.value, "13.500000", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_function_param() {
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup(
            "fn add_one(x: i64)->i64\n"
            "  x + 1\n"
            "end\n"
            "fn add_two(x: i64)->i64\n"
            "  x + 2\n"
            "end\n"
            "fn compute(foo: fn(i64)->i64, x: i64)->i64\n"
            "  foo(x)\n"
            "end\n"
            "compute(add_one, 15)\n",
            &result);
    Zinc_expect_str(&result.value, "16", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup(
            "fn add_one(x: i64)->i64\n"
            "  x + 1\n"
            "end\n"
            "fn add_two(x: i64)->i64\n"
            "  x + 2\n"
            "end\n"
            "fn compute(foo: fn(i64)->i64, x: i64)->i64\n"
            "  foo(x)\n"
            "end\n"
            "compute(add_two, 15)\n",
            &result);
    Zinc_expect_str(&result.value, "17", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function_in_array() {
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup(
            "let func_array: [5]fn(i64)->i64\n"
            "func_array[0] = fn (x: i64)->i64 x*2 end\n"
            "func_array[0](10)",
            &result);
    Zinc_expect_str(&result.value, "20", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_function()
{
    test_akela_llvm_function_declare();
    test_akela_llvm_function_declare_ret();
    test_akela_llvm_function_call();
    test_akela_llvm_function_anonymous_call();
    test_akela_llvm_function_ret_boolean();
    test_akela_llvm_function_ret_array();
    test_akela_llvm_function_extern();
    test_akela_llvm_function_assign();
    test_akela_llvm_function_assign2();
    test_akela_llvm_function_let_assign();
    test_akela_llvm_function_expression();
    test_akela_llvm_function_anonymous_expression();
    test_akela_llvm_function_array_param();
    test_akela_llvm_function_struct_param();
    test_akela_llvm_function_function_param();
    test_akela_llvm_function_in_array();
}