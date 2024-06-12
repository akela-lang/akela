#ifndef AKELA_LLVM_VARIABLE_H
#define AKELA_LLVM_VARIABLE_H

#include "akela_llvm_tools.h"

llvm::Value* CodeGenLLVMVar(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMAssign(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMID(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMArrayLiteral(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMSubscript(JITData* jd, Ast_node* n);

#endif