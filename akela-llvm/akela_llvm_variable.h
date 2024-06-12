#ifndef AKELA_LLVM_VARIABLE_H
#define AKELA_LLVM_VARIABLE_H

#include "akela_llvm_tools.h"

llvm::Value* CodeGenLLVMVar(JITData* jd, struct Ast_node* n);
llvm::Value* CodeGenLLVMAssign(JITData* jd, struct Ast_node* n);
llvm::Value* CodeGenLLVMID(JITData* jd, struct Ast_node* n);
llvm::Value* CodeGenLLVMArrayLiteral(JITData* jd, struct Ast_node* n);
llvm::Value* CodeGenLLVMSubscript(JITData* jd, struct Ast_node* n);

#endif