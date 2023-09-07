#include "test_code_gen_tools.h"
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"

void test_code_gen_constant_number()
{
    test_name(__func__);
    struct buffer value;
    buffer_init(&value);

    cg_setup("1", &value);
    expect_str(&value, "1", "1");

    buffer_destroy(&value);
}

void test_code_gen()
{
    test_code_gen_constant_number();
}
