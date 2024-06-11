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

using namespace llvm;
using namespace llvm::orc;

#define TOPLEVEL_NAME "__toplevel"
#define MODULE_NAME "Akela JIT"

typedef struct Code_gen_context{
    bool in_lhs;
} Code_gen_context;

typedef struct {
    struct error_list* el;
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<Module> TheModule;
    std::unique_ptr<IRBuilder<>> Builder;
    ExitOnError ExitOnErr;
    std::unique_ptr<KaleidoscopeJIT> TheJIT;
    Function* toplevel;
    Code_gen_context context;
} JITData;

void JITDataInit(JITData* jd, struct error_list* el);
FunctionType* CodeGenLLVMFunctionType(JITData* jd, struct ast_node* tu);
Type* CodeGenLLVMGetTypeScalar(JITData* jd, struct ast_node* tu);
Type* CodeGenLLVMGetType(JITData* jd, struct ast_node* tu);
Type* CodeGenLLVMReturnType(JITData* jd, struct ast_node* tu);
void CodeGenLLVMRun(JITData* jd, struct ast_node* n, struct buffer* bf);
BasicBlock* CodeGenLLVMGetLastBlock(JITData* jd, Function* f);

#endif