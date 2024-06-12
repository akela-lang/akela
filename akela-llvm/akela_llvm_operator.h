#ifndef AKELA_LLVM_OPERATOR_H
#define AKELA_LLVM_OPERATOR_H

#include "akela_llvm_tools.h"

llvm::Value* CodeGenLLVMAdd(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMSub(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMSign(JITData* jd, Ast_node* n);

#endif
