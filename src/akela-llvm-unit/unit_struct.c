#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_struct_struct3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Point\n"
             "  x: Real64\n"
             "  y: Real64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "var line: Line = Line\n"
             "  p0: Point\n"
             "    x: 1.5\n"
             "    y: 2.5\n"
             "  end\n"
             "  p1: Point\n"
             "    x: 3.0\n"
             "    y: 4.0\n"
             "  end\n"
             "end\n"
             "line.p0.x = 1.6\n"
             "line.p0.x\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1.600000", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Foo\n"
             "  compute: fn(Int32)->Int32\n"
             "end\n"
             "const foo: Foo = Foo\n"
             "  compute: fn (x: Int32)->Int32\n"
             "    x + 1\n"
             "  end\n"
             "end\n"
             "foo.compute(77)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "78", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Foo\n"
             "  compute: fn(Int32)->Int32\n"
             "end\n"
             "const foo: Foo = Foo\n"
             "  compute: fn (x: Int32)->Int32\n"
             "    x + 2\n"
             "  end\n"
             "end\n"
             "foo.compute(77)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "79", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_struct_struct3);
        Zinc_test_register(test, AkeLlvmUnit_struct_function);

        return;
    }

    Zinc_test_perform(test);
}