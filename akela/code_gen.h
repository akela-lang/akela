#ifndef AKELA_CODE_GEN_H
#define AKELA_CODE_GEN_H

#include "akela/ast.h"
#include "symbol_table.h"
#include "comp_unit.h"

typedef struct {
    u_int8_t jit_offset;
} CodeGenVTable;

typedef struct {
    struct comp_unit* cu;
    struct buffer value;
    struct buffer text;
    bool debug;
} CodeGenResult;

typedef bool (*CodeGenInterface)(void* cg_obj, struct ast_node* n, CodeGenResult* result);

void CodeGenResultInit(CodeGenResult* result);
void CodeGenResultDestroy(CodeGenResult* result);
bool CodeGenJIT(void* cg_obj, CodeGenVTable* cg_vtable, struct ast_node* n, CodeGenResult* result);

#endif