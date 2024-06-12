#ifndef AKELA_LLVM_TOOLS_H
#define AKELA_LLVM_TOOLS_H

#include "../include/KaleidoscopeJIT.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "akela/code_gen.h"
#include "akela_llvm.h"
#include "akela/type_def.h"
#include "zinc/error.h"
#include "zinc/list.h"
#include "zinc/vector.h"
#include <cassert>
#include <iostream>

#define TOPLEVEL_NAME "__toplevel"
#define MODULE_NAME "Akela JIT"

typedef struct Code_gen_context{
    bool in_lhs;
} Code_gen_context;

typedef struct {
    struct error_list* el;
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    llvm::ExitOnError ExitOnErr;
    std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
    llvm::Function* toplevel;
    Code_gen_context context;
} JITData;

void JITDataInit(JITData* jd, struct error_list* el);
llvm::FunctionType* CodeGenLLVMFunctionType(JITData* jd, struct Ast_node* tu);
llvm::Type* CodeGenLLVMGetTypeScalar(JITData* jd, struct Ast_node* tu);
llvm::Type* CodeGenLLVMGetType(JITData* jd, struct Ast_node* tu);
llvm::Type* CodeGenLLVMReturnType(JITData* jd, struct Ast_node* tu);
void CodeGenLLVMRun(JITData* jd, struct Ast_node* n, struct buffer* bf);
llvm::BasicBlock* CodeGenLLVMGetLastBlock(JITData* jd, llvm::Function* f);
llvm::Value* CodeGenLLVMDispatch(JITData* jd, struct Ast_node* n);

#endif