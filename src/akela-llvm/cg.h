#ifndef AKELA_LLVM_H
#define AKELA_LLVM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "akela/code_gen.h"

typedef struct Akela_llvm_cg {
    Zinc_error_list* el;
    Zinc_string_list* extern_list;
    Ake_code_gen_init_interface init;
    Ake_code_gen_jit_interface jit;
    Ake_code_gen_destroy_interface destroy;
    bool debug;
    bool add_abort;
    bool add_printf;
    bool add_exit;
} Akela_llvm_cg;


extern Ake_Vt Akela_llvm_vtable;


void Akela_llvm_cg_init(Akela_llvm_cg* cg, Zinc_error_list* el, Zinc_string_list* extern_list);
void Akela_llvm_cg_create(Akela_llvm_cg** cg, Zinc_error_list* el, Zinc_string_list* extern_list);
void Akela_llvm_cg_destroy(Akela_llvm_cg* cg);

#ifdef __cplusplus
}
#endif

#endif