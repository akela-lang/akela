#ifndef AKELA_LLVM_LITERAL_H
#define AKELA_LLVM_LITERAL_H

#include "tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_number(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_boolean(Jit_data* jd, Ake_ast* n);
    llvm::Value* Handle_string(Jit_data* jd, Ake_ast* n);
}

#endif