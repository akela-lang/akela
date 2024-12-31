#ifndef AKELA_LLVM_FUNCTION_H
#define AKELA_LLVM_FUNCTION_H

#include "akela_llvm_tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_extern(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_function(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_call(Jit_data* jd, Ake_ast* n);
}

#endif