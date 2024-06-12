#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_array_const()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::[4 const]UInt8 = [1,2,3,4]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "2");

    CodeGenResultDestroy(&result);
}

void test_code_gen_const_string()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("\"hello\"\n", &result);
    expect_str(&result.value, "hello", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_const_string2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::[6 const]UInt8 = \"hello\"\n"
             "a\n",
             &result);
    expect_str(&result.value, "hello", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_array_const2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value 0");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value 1");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value 2");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value 3");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "0", "value 4");
    CodeGenResultDestroy(&result);
}

void test_code_gen_array_const3()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "0", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_let_void1()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64", &result);
    expect_str(&result.value, "", "blank");

    CodeGenResultDestroy(&result);
}

void test_code_gen_let_void2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 1", &result);
    expect_str(&result.value, "", "blank");

    CodeGenResultDestroy(&result);
}

void test_code_gen_let_int()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 31\n"
             "a"
            ,&result);
    expect_str(&result.value, "31", "31");

    CodeGenResultDestroy(&result);
}

void test_code_gen_assign()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64\n"
             "a = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_multiple()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n"
             "a",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n"
             "b",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n"
             "c",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_copy_from_variable()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 50\n"
             "let b::Int64 = a\n"
             "a + b\n",
             &result);
    expect_str(&result.value, "100", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function_copy()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64)::Int64\n"
             "  a + 1\n"
             "end\n"
             "let a::Function{Input{Int64},Output{Int64}} = foo\n"
             "a(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function_expression()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64)::Int64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_anonymous_function_expression()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function (a::Int64)::Int64\n"
             "  a + 1\n"
             "end(1)\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_array_literal()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[5]Int64 = [1,2,3,4,5]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5]Int64 = [1,2,3,4,5]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5]Int64 = [1,2,3,4,5]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5]Int64 = [1,2,3,4,5]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5]Int64 = [1,2,3,4,5]\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "5", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_array_literal_multidimensional()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[2][2]Int64 = [[1,2],[3,4]]\n"
             "a[0][0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[2][2]Int64 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[2][2]Int64 = [[1,2],[3,4]]\n"
             "a[1][0]\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[2][2]Int64 = [[1,2],[3,4]]\n"
             "a[1][1]\n",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_array_literal_signed()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[2][2]Int64 = [[1,-2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "-2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_int64()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 2\n"
             "a\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_int32()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int32 = 2\n"
             "a\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_int32_int64()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 1\n"
             "let b::Int32 = a\n"
             "b\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_array_literal_int32()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[2][2]Int32 = [[1,2],[3,4]]\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_array_literal_ptr()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Int64 = [1,2,3,4]\n"
             "let b::[4]Int64 = a\n"
             "b[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void Test_code_gen_array_boolean()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Bool = [true,false,true,false]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "true", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Bool = [true,false,true,false]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "false", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Bool = [true,false,true,false]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "true", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Bool = [true,false,true,false]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "false", "value");
    CodeGenResultDestroy(&result);

}

void Test_code_gen_array_float() {
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1.000000", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2.000000", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3.000000", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Float64 = [1.0, 2.0, 3.0, 4.0]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4.000000", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_array_id()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[4]Int64 = [1, 2, 3, 4]\n"
             "let b::[4]Int64\n"
             "b = a\n"
             "b[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_scalar_id()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 11\n"
             "let b::Int64\n"
             "b = a\n"
             "b\n",
             &result);
    expect_str(&result.value, "11", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_array_id2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let mut a::[4]Int64 = [1, 2, 3, 4]\n"
             "let b::[4]Int64 = [5, 6, 7, 8]\n"
             "a = b\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "5", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_array_subscript()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let mut a::[4]Int64 = [1, 2, 3, 4]\n"
             "a[0] = 10\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "10", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_array_subscript2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let mut a::[2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[0][2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][0]\n",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][1]\n",
             &result);
    expect_str(&result.value, "5", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[2][3]Int64 = [[1, 2, 3], [4, 5, 6]]\n"
             "a[1][2] = 60\n"
             "a[1][2]\n",
             &result);
    expect_str(&result.value, "60", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_array_allocate()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let mut a::[4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let mut a::[4]Int64\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_eseq()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "a",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "b",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "c",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "d",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);

}

void test_akela_llvm_variable()
{
    test_code_gen_array_const();
    test_code_gen_const_string();
    test_code_gen_const_string2();
    test_code_gen_array_const2();
    test_code_gen_array_const3();
    test_code_gen_let_void1();
    test_code_gen_let_void2();
    test_code_gen_let_int();
    test_code_gen_assign();
    test_code_gen_assign_multiple();
    test_code_gen_copy_from_variable();
    test_code_gen_function_copy();
    test_code_gen_function_expression();
    test_code_gen_anonymous_function_expression();
    test_code_gen_array_literal();
    test_code_gen_array_literal_multidimensional();
    test_code_gen_array_literal_signed();
    test_code_gen_int64();
    test_code_gen_int32();
    test_code_gen_int32_int64();
    test_code_gen_array_literal_int32();
    test_code_gen_array_literal_ptr();
    Test_code_gen_array_boolean();
    Test_code_gen_array_float();
    test_code_gen_assign_array_id();
    test_code_gen_assign_scalar_id();
    test_code_gen_assign_array_id2();
    test_code_gen_assign_array_subscript();
    test_code_gen_assign_array_subscript2();
    test_code_gen_assign_array_allocate();
    test_code_gen_assign_eseq();
}