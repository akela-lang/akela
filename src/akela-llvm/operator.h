#ifndef AKELA_LLVM_OPERATOR_H
#define AKELA_LLVM_OPERATOR_H

#include "tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_mult(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_add(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_sub(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_sign(Jit_data* jd, Ake_Ast* n);
}

#endif
