#include "cg.h"
#include "jit.h"
#include <cstddef>
#include "zinc/string_list.h"

Ake_Vt Akela_llvm_vtable = {
    .init_offset = offsetof(Akela_llvm_cg, init),
    .jit_offset = offsetof(Akela_llvm_cg, jit),
    .destroy_offset = offsetof(Akela_llvm_cg, destroy),
};

void Akela_llvm_cg_init(Akela_llvm_cg* cg, Zinc_error_list* el, Zinc_string_list* extern_list)
{
    cg->el = el;
    cg->extern_list = extern_list;
    cg->init = reinterpret_cast<Ake_code_gen_init_interface>(Akela_llvm::Init);
    cg->jit = reinterpret_cast<Ake_code_gen_jit_interface>(Akela_llvm::Jit);
    cg->destroy = reinterpret_cast<Ake_code_gen_destroy_interface>(Akela_llvm::Destroy);
    cg->debug = false;
    cg->add_abort = false;
    cg->add_printf = false;
    cg->add_exit = false;
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
