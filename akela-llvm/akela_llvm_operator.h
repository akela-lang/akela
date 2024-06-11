#ifndef AKELA_LLVM_OPERATOR_H
#define AKELA_LLVM_OPERATOR_H

#include "akela_llvm_tools.h"

llvm::Value* CodeGenLLVMAdd(JITData* jd, struct ast_node* n);
llvm::Value* CodeGenLLVMSub(JITData* jd, struct ast_node* n);
llvm::Value* CodeGenLLVMSign(JITData* jd, struct ast_node* n);

#endif
