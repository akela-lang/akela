#include "code_gen.h"

bool CodeGenJIT(void* cg_obj, CodeGenVTable* cg_vtable, struct ast_node* n, struct buffer* bf)
{
    if (cg_obj && cg_vtable) {
        CodeGenInterface* code_gen_jit = cg_obj + cg_vtable->jit_offset;
        return (*code_gen_jit)(cg_obj, n, bf);
    }
    return true;
}