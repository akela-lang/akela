#ifndef AKELA_LLVM_VARIABLE_H
#define AKELA_LLVM_VARIABLE_H

#include "tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_let(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_assign(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_identifier(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_array_literal(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_subscript(Jit_data* jd, Ake_Ast* n);
}

#endif