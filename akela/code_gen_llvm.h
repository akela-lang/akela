#ifndef AKELA_CODE_GEN_LLVM_H
#define AKELA_CODE_GEN_LLVM_H

#include "code_gen.h"
#include "zinc/input_unicode.h"
#include <stdbool.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include "zinc/buffer.h"
#include "parse_tools.h"
#include "comp_unit.h"
#include "zinc/buffer_list.h"

#define NAMED_VALUES_HASH_SIZE 32

typedef struct {
    struct symbol_table* st;
    LLVMBuilderRef builder;
    LLVMContextRef context;
    LLVMValueRef main;
    CodeGenInterface jit;
} CodeGenLLVM;

extern CodeGenVTable CodeGenLLVMVTable;

void CodeGenLLVMCreate(CodeGenLLVM** cg, struct symbol_table* st);

#endif