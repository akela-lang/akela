#ifndef AKELA_LLVM_FUNCTION_H
#define AKELA_LLVM_FUNCTION_H

#include "akela_llvm_tools.h"

llvm::Value* CodeGenLLVMExtern(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMFunction(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMCall(JITData* jd, Ast_node* n);

#endif