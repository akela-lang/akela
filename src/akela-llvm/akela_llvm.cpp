#include "akela_llvm.h"
#include "akela_llvm_jit.h"
#include <cstddef>
#include "zinc/buffer_list.h"


Ake_code_gen_vtable Code_gen_llvm_vtable = {
        .jit_offset = offsetof(Code_gen_llvm, jit),
};

void Code_gen_llvm_init(Code_gen_llvm* cg, struct error_list* el, struct buffer_list* extern_list)
{
    cg->el = el;
    cg->extern_list = extern_list;
    cg->jit = (Ake_code_gen_interface) Akela_llvm::Jit;
    cg->debug = false;
}

void Code_gen_llvm_create(Code_gen_llvm** cg, struct error_list* el, struct buffer_list* extern_list)
{
    *cg = new Code_gen_llvm();
    Code_gen_llvm_init(*cg, el, extern_list);
}

void Code_gen_llvm_destroy(Code_gen_llvm* cg)
{
    delete cg;
}
