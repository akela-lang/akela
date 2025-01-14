#include "code_gen.h"

void Ake_code_gen_result_init(Ake_code_gen_result* result)
{
    result->cu = NULL;
    Zinc_string_init(&result->value);
    Zinc_string_init(&result->text);
    result->debug = false;
}

void Ake_code_gen_result_destroy(Ake_code_gen_result* result)
{
    Ake_comp_unit_destroy(result->cu);
    free(result->cu);
    Zinc_string_destroy(&result->value);
    Zinc_string_destroy(&result->text);
}

bool Ake_code_gen_jit(void* cg_obj, Ake_code_gen_vtable* cg_vtable, Ake_ast* n, Ake_code_gen_result* result)
{
    if (cg_obj && cg_vtable) {
        Ake_code_gen_interface* code_gen_jit = cg_obj + cg_vtable->jit_offset;
        return (*code_gen_jit)(cg_obj, n, result);
    }
    return true;
}