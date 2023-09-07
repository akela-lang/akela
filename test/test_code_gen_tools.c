#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "zinc/memory.h"
#include "akela/input.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"

bool cg_setup(const char* text, struct buffer* value)
{
    struct comp_unit* cu = NULL;
    malloc_safe((void**)&cu, sizeof(struct comp_unit));

    struct buffer* bf = NULL;
    malloc_safe((void**)&bf, sizeof(struct buffer));
    buffer_init(bf);

    array2buffer(text, bf);

    struct string_data* sd = NULL;
    malloc_safe((void**)&sd, sizeof(struct string_data));

    string_data_init(bf, sd);

    comp_unit_init(cu);
    bool valid = comp_unit_compile(cu, (input_getchar)string_getchar, (input_data)sd);
    expect_true(valid, "valid");
    expect_no_errors(&cu->el);

    cg_jit(cu, value, false, false);

    buffer_destroy(bf);
    free(bf);
    free(sd);
    free(cu);

    return valid;
}