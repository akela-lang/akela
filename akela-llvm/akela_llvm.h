#ifndef AKELA_LLVM_H
#define AKELA_LLVM_H

#include "akela/code_gen.h"

typedef struct Code_gen_llvm {
    struct error_list* el;
    struct buffer_list* extern_list;
    Code_gen_interface jit;
    bool debug;
} Code_gen_llvm;

extern Code_gen_vtable Code_gen_llvm_vtable;

#ifdef __cplusplus
extern "C" {
#endif

void Code_gen_llvm_create(Code_gen_llvm** cg, struct error_list* el, struct buffer_list* extern_list);
void Code_gen_llvm_destroy(Code_gen_llvm* cg);

#ifdef __cplusplus
}
#endif

#endif