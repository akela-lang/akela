#include "akela_llvm.h"
#include "akela_llvm_jit.h"
#include <cstddef>


Code_gen_vtable Code_gen_llvm_vtable = {
        .jit_offset = offsetof(Code_gen_llvm, jit),
};

void Code_gen_llvm_init(Code_gen_llvm* cg, struct error_list* el)
{
    cg->el = el;
    cg->jit = (Code_gen_interface) Akela_llvm::Jit;
    cg->debug = false;
}

void Code_gen_llvm_create(Code_gen_llvm** cg, struct error_list* el)
{
    *cg = new Code_gen_llvm();
    Code_gen_llvm_init(*cg, el);
}

void Code_gen_llvm_destroy(Code_gen_llvm* cg)
{
    delete cg;
}
