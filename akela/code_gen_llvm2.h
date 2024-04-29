#ifndef AKELA_CODE_GEN_LLVM2_H
#define AKELA_CODE_GEN_LLVM2_H

#include "code_gen.h"

typedef struct {
    struct error_list* el;
    CodeGenInterface jit;
} CodeGenLLVM2;

extern CodeGenVTable CodeGenLLVM2VTable;

#ifdef __cplusplus
extern "C" {
#endif

void CodeGenLLVM2Create(CodeGenLLVM2** cg, struct error_list* el);
void CodeGenLLVM2Destroy(CodeGenLLVM2* cg);

#ifdef __cplusplus
}
#endif

#endif