#ifndef AKELA_LLVM_STRUCT_H
#define AKELA_LLVM_STRUCT_H

#include "akela_llvm_tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_struct(Jit_data* jd, Cob_ast* n);
    llvm::Value* Handle_dot(Jit_data* jd, Cob_ast* n);
    llvm::Value* Handle_struct_literal(Jit_data* jd, Cob_ast* n);
}

#endif