#ifndef AKELA_LLVM_STMTS_H
#define AKELA_LLVM_STMTS_H

#include "tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_stmts(Jit_data* jd, Ake_Ast* n);
    llvm::Value* Handle_if(Jit_data* jd, Ake_Ast* n);
}

#endif