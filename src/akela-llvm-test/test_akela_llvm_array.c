#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"
#include <signal.h>
#include <string.h>
#include <setjmp.h>

void test_code_gen_array_const()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4 const]u8 = [1,2,3,4]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "2");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_const_string()
{
    test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("\"hello\"\n", &result);
    expect_str(&result.value, "hello", "value");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_const_string2()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [6 const]u8 = \"hello\"\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "104", "value");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_array_const2()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5 const]u8 = [1,2,3,4,0]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value 0");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5 const]u8 = [1,2,3,4,0]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value 1");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5 const]u8 = [1,2,3,4,0]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value 2");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5 const]u8 = [1,2,3,4,0]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value 3");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5 const]u8 = [1,2,3,4,0]\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "0", "value 4");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_array_literal()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5]i64 = [1,2,3,4,5]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5]i64 = [1,2,3,4,5]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5]i64 = [1,2,3,4,5]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5]i64 = [1,2,3,4,5]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [5]i64 = [1,2,3,4,5]\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_multidimensional()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [2][2]i64 = [[1,2],[3,4]]\n"
             "a[0][0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [2][2]i64 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [2][2]i64 = [[1,2],[3,4]]\n"
             "a[1][0]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [2][2]i64 = [[1,2],[3,4]]\n"
             "a[1][1]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_signed()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [2][2]i64 = [[1,-2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "-2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_int32()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [2][2]i32 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_ptr()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]i64 = [1,2,3,4]\n"
             "let b: [4]i64 = a\n"
             "b[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void Test_code_gen_array_boolean()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]bool = [true,false,true,false]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "true", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]bool = [true,false,true,false]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "false", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]bool = [true,false,true,false]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "true", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]bool = [true,false,true,false]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "false", "value");
    Ake_code_gen_result_destroy(&result);

}

void Test_code_gen_array_float() {
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]f64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1.000000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]f64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2.000000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]f64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3.000000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]f64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4.000000", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_id2()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [4]i64 = [1, 2, 3, 4]\n"
             "let b: [4]i64 = [5, 6, 7, 8]\n"
             "a = b\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_subscript()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [4]i64 = [1, 2, 3, 4]\n"
             "a[0] = 10\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "10", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_subscript2()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]i64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]i64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]i64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]i64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][0]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]i64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][1]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]i64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][2]\n",
             &result);
    expect_str(&result.value, "60", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_allocate()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [4]i64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [4]i64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [4]i64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [4]i64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_id()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]i64 = [1, 2, 3, 4]\n"
             "let b: [4]i64\n"
             "b = a\n"
             "b[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_array_assign_sub_array()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4][4]i64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]i64\n"
             "b = a[1]\n"
             "b[0]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4][4]i64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]i64\n"
             "b = a[1]\n"
             "b[1]\n",
             &result);
    expect_str(&result.value, "6", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4][4]i64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]i64\n"
             "b = a[1]\n"
             "b[2]\n",
             &result);
    expect_str(&result.value, "7", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4][4]i64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]i64\n"
             "b = a[1]\n"
             "b[3]\n",
             &result);
    expect_str(&result.value, "8", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_array_subscript_exit()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: [4]i64 = [1,2,3,4]\n"
             "a[4]\n",
             &result);
    expect_true(true, "exit");

    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_array_assign_subscript()
{
    test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let mut a: [4][4]i64 = [[1,2,3,4],\n"
             "  [5,6,7,8],\n"
             "  [9,10,11,12],\n"
             "  [13,14,15,16]\n"
             "]\n"
             "let b: [4]i64 = [50,60,70,80]\n"
             "a[1] = b\n"
             "a[1][0]\n",
             &result);
    expect_str(&result.value, "50", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_array()
{
    test_code_gen_array_const();
    test_code_gen_const_string();
    test_code_gen_const_string2();
    test_code_gen_array_const2();
    test_code_gen_array_literal();
    test_code_gen_array_literal_multidimensional();
    test_code_gen_array_literal_signed();
    test_code_gen_array_literal_int32();
    test_code_gen_array_literal_ptr();
    Test_code_gen_array_boolean();
    Test_code_gen_array_float();
    test_code_gen_assign_array_id();
    test_code_gen_assign_array_id2();
    test_code_gen_assign_array_subscript();
    test_code_gen_assign_array_subscript2();
    test_code_gen_assign_array_allocate();
    test_akela_llvm_array_assign_sub_array();
    //test_akela_llvm_array_subscript_exit();
    test_akela_llvm_array_assign_subscript();
}