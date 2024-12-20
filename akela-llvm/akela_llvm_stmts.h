#ifndef AKELA_LLVM_STMTS_H
#define AKELA_LLVM_STMTS_H

#include "akela_llvm_tools.h"

namespace Akela_llvm {
    llvm::Value* Handle_stmts(Jit_data* jd, Cob_ast* n);
    llvm::Value* Handle_if(Jit_data* jd, Cob_ast* n);
}

#endif