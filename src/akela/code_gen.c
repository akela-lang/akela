#include "code_gen.h"
#include <stdint.h>

void Ake_code_gen_result_init(Ake_code_gen_result* result)
{
    result->cu = NULL;
    Zinc_string_init(&result->value);
    Zinc_string_init(&result->module_text);
    Zinc_string_init(&result->function_text);
    result->debug = false;
    result->return_address = NULL;
    result->dry_run = false;
    result->test = NULL;
}

void Ake_code_gen_result_destroy(Ake_code_gen_result* result)
{
    Ake_comp_unit_destroy(result->cu);
    free(result->cu);
    Zinc_string_destroy(&result->value);
    Zinc_string_destroy(&result->module_text);
    Zinc_string_destroy(&result->function_text);
}

void* Ake_code_gen_init(void* cg_obj, Ake_Vt* cg_vtable)
{
    if (cg_obj && cg_vtable) {
        Ake_code_gen_init_interface* code_gen_init = (Ake_code_gen_init_interface*)((uint8_t*)cg_obj + cg_vtable->init_offset);
        return (*code_gen_init)();
    }
    return NULL;
}

bool Ake_code_gen_jit(void* cg_obj, Ake_Vt* cg_vtable, void* jd, Ake_Ast* n, Ake_code_gen_result* result)
{
    if (cg_obj && cg_vtable) {
        Ake_code_gen_jit_interface* code_gen_jit =
            (Ake_code_gen_jit_interface*)((uint8_t*)cg_obj + cg_vtable->jit_offset);
        return (*code_gen_jit)(cg_obj, jd, n, result);
    }
    return true;
}

void Ake_code_gen_destroy(void* cg_obj, Ake_Vt* cg_vtable, void* cg_jit)
{
    if (cg_obj && cg_vtable && cg_jit) {
        Ake_code_gen_destroy_interface* code_gen_destroy =
            (Ake_code_gen_destroy_interface*)((uint8_t*)cg_obj + cg_vtable->destroy_offset);
        (*code_gen_destroy)(cg_jit);
    }
}