#ifndef AKELA_LLVM_STRUCT_H
#define AKELA_LLVM_STRUCT_H

#include "tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_struct(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_dot(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_struct_literal(Jit_data* jd, Ake_ast* n);
    llvm::StructType* GetStructTypeFromType(Jit_data* jd, Ake_TypeDef* td);
}

#endif