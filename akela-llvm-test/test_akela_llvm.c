#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_add()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("1 + 40", &result);
    expect_str(&result.value, "41", "41");

    CodeGenResultDestroy(&result);
}

void test_code_gen_add2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 4\n"
             "a + 61\n",
             &result);
    expect_str(&result.value, "65", "65");

    CodeGenResultDestroy(&result);
}

void test_code_gen_sub()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("15-2", &result);
    expect_str(&result.value, "13", "13");

    CodeGenResultDestroy(&result);
}

void test_code_gen_sub2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 10\n"
             "a - 2\n",
             &result);
    expect_str(&result.value, "8", "8");

    CodeGenResultDestroy(&result);
}

void test_code_gen_last()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 1\n"
             "let b::Int64 = 2\n"
             "a + b\n",
             &result);
    expect_str(&result.value, "3", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_if()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("if true\n"
             "  1\n"
             "end\n",
             &result);
    expect_str(&result.value, "", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_if_else()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("if true\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    expect_str(&result.value, "1", "1 value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    expect_str(&result.value, "2", "2 value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_if_elseif_else()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("if true\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif false\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "3", "value");
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

void test_code_gen_function()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)\n"
             "end\n",
             &result);
    expect_str(&result.value, "Function", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_function_ret()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::Int64\n"
             "  a + b + c\n"
             "end\n",
             &result);
    expect_str(&result.value, "Function", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::Int64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "6", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call_ptr()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let foo::Function{Input{Int64,Int64,Int64},Output{Int64}} = function (a::Int64, b::Int64, c::Int64)::Int64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "6", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::Bool\n"
             "  true\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "true", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_call3()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo(a::Int64, b::Int64, c::Int64)::[6 const]UInt8\n"
             "  \"hello\"\n"
             "end\n"
             "foo(1, 2, 3)\n",
             &result);
    expect_str(&result.value, "hello", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_anonymous_function()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let foo::Function{Input{Int64,Int64,Int64},Output{Int64}} = function (a::Int64, b::Int64, c::Int64)::Int64\n"
             "  a + b + c\n"
             "end\n"
             "foo(1, 1, 1)\n",
             &result);
    expect_str(&result.value, "3", "value");
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

void test_code_gen_if_expression()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = if true 1 else 2 end\n"
             "a\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = if false 1 else 2 end\n"
             "a\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_extern()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("extern pow(x::Float64, y::Float64)::Float64\n"
             "pow(5.0, 2.0)\n",
             &result);
    expect_str(&result.value, "25.000000", "value");
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

void test_code_gen_assign_function_id()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo()::Int64 1 end\n"
             "let bar::Function{Output{Int64}}\n"
             "bar = foo\n"
             "bar()\n",
             &result);
    expect_str(&result.value, "1", "value");
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

void test_code_gen_assign_function_id2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("function foo()::Int64 1 end\n"
             "function bar()::Int64 2 end\n"
             "foo = bar\n"
             "foo()\n",
             &result);
    expect_str(&result.value, "2", "value");
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

void test_code_gen()
{
    test_code_gen_array_const();
    test_code_gen_const_string();
    test_code_gen_const_string2();
    test_code_gen_array_const2();
    test_code_gen_array_const3();
    test_code_gen_let_void1();
    test_code_gen_let_void2();
    test_code_gen_let_int();
    test_code_gen_boolean_true();
    test_code_gen_boolean_false();
    test_code_gen_add();
    test_code_gen_add2();
    test_code_gen_sub();
    test_code_gen_sub2();
    test_code_gen_last();
    test_code_gen_if();
    test_code_gen_if_else();
    test_code_gen_if_elseif_else();
    test_code_gen_assign();
    test_code_gen_assign_multiple();
    test_code_gen_function();
    test_code_gen_function_ret();
    test_code_gen_call();
    test_code_gen_call_ptr();
    test_code_gen_call2();
    test_code_gen_call3();
    test_code_gen_anonymous_function();
    test_code_gen_copy_from_variable();
    test_code_gen_function_copy();
    test_code_gen_function_expression();
    test_code_gen_anonymous_function_expression();
    test_code_gen_if_expression();
    test_code_gen_extern();
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
    test_code_gen_assign_function_id();
    test_code_gen_assign_array_id();
    test_code_gen_assign_scalar_id();
    test_code_gen_assign_function_id2();
    test_code_gen_assign_array_id2();
    test_code_gen_assign_array_subscript();
    test_code_gen_assign_array_subscript2();
    test_code_gen_assign_array_allocate();
    test_code_gen_assign_eseq();
}
