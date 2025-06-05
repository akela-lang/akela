#ifndef AKELA_CODE_GEN_H
#define AKELA_CODE_GEN_H

#include "akela/ast.h"
#include "symbol_table.h"
#include "comp_unit.h"
#include <stdint.h>

typedef struct {
    uint8_t init_offset;
    uint8_t jit_offset;
    uint8_t destroy_offset;
} Ake_Vt;

typedef struct {
    Ake_comp_unit* cu;
    Zinc_string value;
    Zinc_string module_text;
    Zinc_string function_text;
    void* return_address;
    long long return_size;
    bool debug;
    bool dry_run;
} Ake_code_gen_result;

typedef void* (*Ake_code_gen_init_interface)();
typedef bool (*Ake_code_gen_jit_interface)(void* cg_obj, void* jd, Ake_ast* n, Ake_code_gen_result* result);
typedef void (*Ake_code_gen_destroy_interface)(void* cg_jit);

void Ake_code_gen_result_init(Ake_code_gen_result* result);
void Ake_code_gen_result_destroy(Ake_code_gen_result* result);
void* Ake_code_gen_init(void* cg_obj, Ake_Vt* cg_vtable);
bool Ake_code_gen_jit(void* cg_obj, Ake_Vt* cg_vtable, void* jd, Ake_ast* n, Ake_code_gen_result* result);
void Ake_code_gen_destroy(void* cg_obj, Ake_Vt* cg_vtable, void* cg_jit);

#endif