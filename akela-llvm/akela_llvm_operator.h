#ifndef AKELA_LLVM_OPERATOR_H
#define AKELA_LLVM_OPERATOR_H

#include "akela_llvm_tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_add(Jit_data* jd, Ast_node* n);
    llvm::Value* Handle_sub(Jit_data* jd, Ast_node* n);
    llvm::Value* Handle_sign(Jit_data* jd, Ast_node* n);
}

#endif
