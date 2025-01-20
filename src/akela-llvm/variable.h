#ifndef AKELA_LLVM_VARIABLE_H
#define AKELA_LLVM_VARIABLE_H

#include "tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_variable_dec(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_assign(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_identifier(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_array_literal(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_subscript(Jit_data* jd, Ake_ast* n);
}

#endif