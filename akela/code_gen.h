#ifndef AKELA_CODE_GEN_H
#define AKELA_CODE_GEN_H

#include "ast.h"

typedef struct {
    u_int8_t jit_offset;
} CodeGenVTable;

typedef struct {
    struct buffer value;
    struct buffer text;
} CodeGenResult;

typedef bool (*CodeGenInterface)(void* cg_obj, struct ast_node* n, CodeGenResult* result);

void CodeGenResultInit(CodeGenResult* result);
void CodeGenResultDestroy(CodeGenResult* result);
bool CodeGenJIT(void* cg_obj, CodeGenVTable* cg_vtable, struct ast_node* n, CodeGenResult* result);

#endif