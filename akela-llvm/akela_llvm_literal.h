#ifndef AKELA_LLVM_LITERAL_H
#define AKELA_LLVM_LITERAL_H

#include "akela_llvm_tools.h"

llvm::Value* CodeGenLLVMNumber(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMBoolean(JITData* jd, Ast_node* n);
llvm::Value* CodeGenLLVMString(JITData* jd, Ast_node* n);

#endif