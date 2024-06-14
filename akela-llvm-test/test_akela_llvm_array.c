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
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4 const]UInt8 = [1,2,3,4]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "2");
    Code_gen_result_destroy(&result);
}

void test_code_gen_const_string()
{
    test_name(__func__);
    Code_gen_result result;
    Code_gen_result_init(&result);

    cg_setup("\"hello\"\n", &result);
    expect_str(&result.value, "hello", "value");

    Code_gen_result_destroy(&result);
}

void test_code_gen_const_string2()
{
    test_name(__func__);
    Code_gen_result result;
    Code_gen_result_init(&result);

    cg_setup("let a: [6 const]UInt8 = \"hello\"\n"
             "a\n",
             &result);
    expect_str(&result.value, "hello", "value");

    Code_gen_result_destroy(&result);
}

void test_code_gen_array_const2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8 = [1,2,3,4,0]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value 0");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8 = [1,2,3,4,0]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value 1");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8 = [1,2,3,4,0]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value 2");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8 = [1,2,3,4,0]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value 3");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8 = [1,2,3,4,0]\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "0", "value 4");
    Code_gen_result_destroy(&result);
}

void test_code_gen_array_const3()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "0", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_array_literal()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [5]Int64 = [1,2,3,4,5]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5]Int64 = [1,2,3,4,5]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5]Int64 = [1,2,3,4,5]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5]Int64 = [1,2,3,4,5]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [5]Int64 = [1,2,3,4,5]\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_multidimensional()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [2][2]Int64 = [[1,2],[3,4]]\n"
             "a[0][0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [2][2]Int64 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [2][2]Int64 = [[1,2],[3,4]]\n"
             "a[1][0]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [2][2]Int64 = [[1,2],[3,4]]\n"
             "a[1][1]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_signed()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [2][2]Int64 = [[1,-2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "-2", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_int32()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [2][2]Int32 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_array_literal_ptr()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Int64 = [1,2,3,4]\n"
             "let b: [4]Int64 = a\n"
             "b[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);
}

void Test_code_gen_array_boolean()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Bool = [true,false,true,false]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "true", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Bool = [true,false,true,false]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "false", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Bool = [true,false,true,false]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "true", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Bool = [true,false,true,false]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "false", "value");
    Code_gen_result_destroy(&result);

}

void Test_code_gen_array_float() {
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1.000000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2.000000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3.000000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4.000000", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_id2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let mut a: [4]Int64 = [1, 2, 3, 4]\n"
             "let b: [4]Int64 = [5, 6, 7, 8]\n"
             "a = b\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_subscript()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let mut a: [4]Int64 = [1, 2, 3, 4]\n"
             "a[0] = 10\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "10", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_subscript2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][0]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][1]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][2]\n",
             &result);
    expect_str(&result.value, "60", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_allocate()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let mut a: [4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let mut a: [4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Code_gen_result_destroy(&result);
}

void test_code_gen_assign_array_id()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Int64 = [1, 2, 3, 4]\n"
             "let b: [4]Int64\n"
             "b = a\n"
             "b[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_array_assign_sub_array()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4][4]Int64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]Int64\n"
             "b = a[1]\n"
             "b[0]\n",
             &result);
    expect_str(&result.value, "5", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4][4]Int64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]Int64\n"
             "b = a[1]\n"
             "b[1]\n",
             &result);
    expect_str(&result.value, "6", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4][4]Int64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]Int64\n"
             "b = a[1]\n"
             "b[2]\n",
             &result);
    expect_str(&result.value, "7", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4][4]Int64 = ["
             "  [1, 2, 3, 4],\n"
             "  [5, 6, 7, 8],\n"
             "  [9, 10, 11, 12],\n"
             "  [13, 14, 15, 16]\n"
             "]\n"
             "let b: [4]Int64\n"
             "b = a[1]\n"
             "b[3]\n",
             &result);
    expect_str(&result.value, "8", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_array_subscript_exit()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4]Int64 = [1,2,3,4]\n"
             "a[4]\n",
             &result);
    expect_true(true, "exit");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_array()
{
    test_code_gen_array_const();
    test_code_gen_const_string();
    test_code_gen_const_string2();
    test_code_gen_array_const2();
    test_code_gen_array_const3();
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
}