#ifndef AKELA_CODE_GEN_H
#define AKELA_CODE_GEN_H

#include "ast.h"

typedef struct {
    u_int8_t jit_offset;
} CodeGenVTable;

typedef void (*CodeGenInterface)(void* cg_obj, struct ast_node* n, struct buffer* bf);

void CodeGenJIT(void* cg_obj, CodeGenVTable* cg_vtable, struct ast_node* n, struct buffer* bf);

#endif