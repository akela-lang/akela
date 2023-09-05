#ifndef CODE_GEN_TOOLS_H
#define CODE_GEN_TOOLS_H

#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include "zinc/error.h"
#include "zinc/hash.h"

struct cg_data {
    LLVMBuilderRef builder;
    LLVMContextRef context;
    struct error_list el;
    struct hash_table named_values;
};

#define NAMED_VALUES_HASH_SIZE 32

void cg_data_init(struct cg_data *cgd, LLVMBuilderRef builder, LLVMContextRef context);
void cg_data_destroy(struct cg_data* cgd);

#endif