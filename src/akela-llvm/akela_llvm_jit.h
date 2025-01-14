#ifndef AKELA_LLVM_JIT_H
#define AKELA_LLVM_JIT_H

namespace Akela_llvm {
        bool Jit(Code_gen_llvm* cg, Ake_ast* n, Ake_code_gen_result* result);
}

#endif