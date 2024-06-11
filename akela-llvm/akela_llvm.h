#ifndef AKELA_CODE_GEN_LLVM2_H
#define AKELA_CODE_GEN_LLVM2_H

#include "akela/code_gen.h"

typedef struct {
    struct error_list* el;
    CodeGenInterface jit;
    bool debug;
} CodeGenLLVM;

extern CodeGenVTable CodeGenLLVMVTable;

#ifdef __cplusplus
extern "C" {
#endif

void CodeGenLLVMCreate(CodeGenLLVM** cg, struct error_list* el);
void CodeGenLLVMDestroy(CodeGenLLVM* cg);

#ifdef __cplusplus
}
#endif

#endif