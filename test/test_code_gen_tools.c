#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "zinc/memory.h"
#include "akela/input.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"
#include <string.h>
#include "zinc/input_char_string.h"

bool cg_setup(const char* text, struct buffer* value)
{
    struct comp_unit* cu = NULL;
    malloc_safe((void**)&cu, sizeof(struct comp_unit));

    Vector* vector = NULL;
    VectorCreate(&vector, sizeof(char));
    VectorAdd(vector, text, strlen(text));
    InputCharString* input = NULL;
    InputCharStringCreate(&input, vector);

    comp_unit_init(cu);
    bool valid = comp_unit_compile(cu, input, input->input_vtable);
    expect_true(valid, "valid");
    expect_no_errors(&cu->el);

    cg_jit(cu, value, true, true);

    VectorDestroy(vector);
    free(vector);
    free(input);
    free(cu);

    return valid;
}