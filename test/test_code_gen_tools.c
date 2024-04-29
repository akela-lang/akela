#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "zinc/memory.h"
#include "akela/parse_tools.h"
#include "akela/comp_unit.h"
#include "akela/code_gen.h"
#include "akela/code_gen_llvm.h"
#include "akela/code_gen_llvm2.h"
#include <string.h>
#include "zinc/input_unicode_string.h"

bool cg_setup(const char* text, struct buffer* value)
{
    struct comp_unit* cu = NULL;
    malloc_safe((void**)&cu, sizeof(struct comp_unit));

    Vector* vector = NULL;
    VectorCreate(&vector, sizeof(char));
    VectorAdd(vector, text, strlen(text));
    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, vector);

    bool valid;

    comp_unit_init(cu);
    valid = comp_unit_compile(cu, input, input->input_vtable);
    expect_true(valid, "valid");

    if (false) {
        CodeGenLLVM* cg = NULL;
        CodeGenLLVMCreate(&cg, &cu->el);
        valid = CodeGenJIT(cg, &CodeGenLLVMVTable, cu->root, value);
        free(cg);
    } else {
        CodeGenLLVM2* cg = NULL;
        CodeGenLLVM2Create(&cg, &cu->el);
        valid = CodeGenJIT(cg, &CodeGenLLVM2VTable, cu->root, value);
        CodeGenLLVM2Destroy(cg);
    }

    expect_true(valid, "valid");
    expect_no_errors(&cu->el);

    VectorDestroy(vector);
    free(vector);
    free(input);
    comp_unit_destroy(cu);
    free(cu);

    return valid;
}