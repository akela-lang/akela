#ifndef AKELA_LLVM_H
#define AKELA_LLVM_H

#include "akela/code_gen.h"

typedef struct Akela_llvm_cg {
    Zinc_error_list* el;
    Zinc_string_list* extern_list;
    Ake_code_gen_interface jit;
    bool debug;
    bool add_abort;
    bool add_printf;
    bool add_exit;
} Akela_llvm_cg;

extern Code_gen_vtable Akela_llvm_vtable;

#ifdef __cplusplus
extern "C" {
#endif

void Akela_llvm_cg_create(Akela_llvm_cg** cg, Zinc_error_list* el, Zinc_string_list* extern_list);
void Akela_llvm_cg_destroy(Akela_llvm_cg* cg);

#ifdef __cplusplus
}
#endif

#endif