#ifndef AKELA_LLVM_STMTS_H
#define AKELA_LLVM_STMTS_H

#include "akela_llvm_tools.h"

llvm::Value* CodeGenLLVMStmts(JITData* jd, struct ast_node* n);
llvm::Value* CodeGenLLVMIf(JITData* jd, struct ast_node* n);

#endif