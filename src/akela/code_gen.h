#ifndef AKELA_CODE_GEN_H
#define AKELA_CODE_GEN_H

#include "akela/ast.h"
#include "symbol_table.h"
#include "comp_unit.h"

typedef struct {
    u_int8_t jit_offset;
} Code_gen_vtable;

typedef struct {
    Ake_comp_unit* cu;
    Zinc_string value;
    Zinc_string module_text;
    Zinc_string function_text;
    void* return_address;
    long long return_size;
    bool debug;
} Ake_code_gen_result;

typedef bool (*Ake_code_gen_interface)(void* cg_obj, Ake_ast* n, Ake_code_gen_result* result);

void Ake_code_gen_result_init(Ake_code_gen_result* result);
void Ake_code_gen_result_destroy(Ake_code_gen_result* result);
bool Ake_code_gen_jit(void* cg_obj, Code_gen_vtable* cg_vtable, Ake_ast* n, Ake_code_gen_result* result);

#endif