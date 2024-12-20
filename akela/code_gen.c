#include "code_gen.h"

void Code_gen_result_init(Code_gen_result* result)
{
    result->cu = NULL;
    buffer_init(&result->value);
    buffer_init(&result->text);
    result->debug = false;
}

void Code_gen_result_destroy(Code_gen_result* result)
{
    comp_unit_destroy(result->cu);
    free(result->cu);
    buffer_destroy(&result->value);
    buffer_destroy(&result->text);
}

bool Code_gen_jit(void* cg_obj, Code_gen_vtable* cg_vtable, Cob_ast* n, Code_gen_result* result)
{
    if (cg_obj && cg_vtable) {
        Code_gen_interface* code_gen_jit = cg_obj + cg_vtable->jit_offset;
        return (*code_gen_jit)(cg_obj, n, result);
    }
    return true;
}