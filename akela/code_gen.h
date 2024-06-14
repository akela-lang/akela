#ifndef AKELA_CODE_GEN_H
#define AKELA_CODE_GEN_H

#include "akela/ast.h"
#include "symbol_table.h"
#include "comp_unit.h"

typedef struct {
    u_int8_t jit_offset;
} Code_gen_vtable;

typedef struct {
    struct comp_unit* cu;
    struct buffer value;
    struct buffer text;
    bool debug;
} Code_gen_result;

typedef bool (*Code_gen_interface)(void* cg_obj, Ast_node* n, Code_gen_result* result);

void Code_gen_result_init(Code_gen_result* result);
void Code_gen_result_destroy(Code_gen_result* result);
bool Code_gen_jit(void* cg_obj, Code_gen_vtable* cg_vtable, Ast_node* n, Code_gen_result* result);

#endif