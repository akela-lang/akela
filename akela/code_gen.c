#include "code_gen.h"

void CodeGenResultInit(CodeGenResult* result)
{
    result->cu = NULL;
    buffer_init(&result->value);
    buffer_init(&result->text);
    result->debug = false;
}

void CodeGenResultDestroy(CodeGenResult* result)
{
    comp_unit_destroy(result->cu);
    free(result->cu);
    buffer_destroy(&result->value);
    buffer_destroy(&result->text);
}

bool CodeGenJIT(void* cg_obj, CodeGenVTable* cg_vtable, struct ast_node* n, CodeGenResult* result)
{
    if (cg_obj && cg_vtable) {
        CodeGenInterface* code_gen_jit = cg_obj + cg_vtable->jit_offset;
        return (*code_gen_jit)(cg_obj, n, result);
    }
    return true;
}