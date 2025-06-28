#ifndef AKELA_LLVM_JIT_H
#define AKELA_LLVM_JIT_H

#include "tools.h"

namespace Akela_llvm {
        bool Jit(Akela_llvm_cg* cg, Jit_data* jd, Ake_Ast* n, Ake_code_gen_result* result);
        Jit_data* Init();
        void Destroy(Jit_data* jd);
}

#endif