#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "zinc/memory.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"
#include "akela-llvm/akela_llvm.h"
#include <string.h>
#include "zinc/input_unicode_string.h"
#include "zinc/buffer_list.h"

bool cg_setup(const char* text, Ake_code_gen_result* result)
{
    struct Ake_comp_unit* cu = NULL;
    malloc_safe((void**)&cu, sizeof(struct Ake_comp_unit));

    Vector* vector = NULL;
    VectorCreate(&vector, sizeof(char));
    VectorAdd(vector, text, strlen(text));
    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, vector);

    bool valid;

    Ake_comp_unit_init(cu);
    valid = Ake_comp_unit_compile(cu, input, input->input_vtable);
    expect_true(valid, "valid");

    result->cu = cu;

    if (valid) {
        Code_gen_llvm* cg = NULL;
        Code_gen_llvm_create(&cg, &cu->el, &cu->extern_list);
        valid = Ake_code_gen_jit(cg, &Code_gen_llvm_vtable, cu->root, result);
        Code_gen_llvm_destroy(cg);
        expect_true(valid, "valid");
    }

    if (!valid && result->text.size > 0) {
        struct location loc;
        location_init(&loc);
        Zinc_string_finish(&result->text);
        error_list_set(&cu->el, &loc, "Module:\n%s", result->text.buf);
    }
    expect_no_errors(&cu->el);

    VectorDestroy(vector);
    free(vector);
    free(input);

    return valid;
}