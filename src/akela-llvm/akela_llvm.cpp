#include "akela_llvm.h"
#include "akela_llvm_jit.h"
#include <cstddef>
#include "zinc/string_list.h"


Code_gen_vtable Akela_llvm_vtable = {
        .jit_offset = offsetof(Akela_llvm_cg, jit),
};

void Akela_llvm_cg_init(Akela_llvm_cg* cg, Zinc_error_list* el, Zinc_string_list* extern_list)
{
    cg->el = el;
    cg->extern_list = extern_list;
    cg->jit = reinterpret_cast<Ake_code_gen_interface>(Akela_llvm::Jit);
    cg->debug = false;
}

void Akela_llvm_cg_create(Akela_llvm_cg** cg, Zinc_error_list* el, Zinc_string_list* extern_list)
{
    *cg = new Akela_llvm_cg();
    Akela_llvm_cg_init(*cg, el, extern_list);
}

void Akela_llvm_cg_destroy(Akela_llvm_cg* cg)
{
    delete cg;
}
