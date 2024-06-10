#ifndef AKELA_CODE_GEN_LLVM2_H
#define AKELA_CODE_GEN_LLVM2_H

#include "code_gen.h"

typedef struct {
    struct error_list* el;
    CodeGenInterface jit;
    bool debug;
} CodeGenLLVM2;

extern CodeGenVTable CodeGenLLVM2VTable;

#ifdef __cplusplus
extern "C" {
#endif

void CodeGenLLVMCreate(CodeGenLLVM2** cg, struct error_list* el);
void CodeGenLLVMDestroy(CodeGenLLVM2* cg);

#ifdef __cplusplus
}
#endif

#endif