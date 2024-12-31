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
#include "akela/symbol.h"

#define TOPLEVEL_NAME "__toplevel"
#define MODULE_NAME "Akela JIT"

namespace Akela_llvm {
    typedef struct {
        struct error_list* el;
        std::unique_ptr<llvm::LLVMContext> TheContext;
        std::unique_ptr<llvm::Module> TheModule;
        std::unique_ptr<llvm::IRBuilder<>> Builder;
        llvm::ExitOnError ExitOnErr;
        std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
        llvm::Function* toplevel;
        llvm::Function* abort_function;
        llvm::Function* printf_function;
        llvm::Function* exit_function;
        std::vector<llvm::Function*> current_function;
    } Jit_data;

    void Jit_data_init(Jit_data* jd, struct error_list* el);
    llvm::FunctionType* Get_function_type(Jit_data* jd, Type_use* tu);
    llvm::Type* Get_scalar_type(Jit_data * jd, Type_use* tu);
    llvm::Type* Get_type(Jit_data * jd, Type_use* tu);
    llvm::Type* Get_type_pointer(Jit_data *jd, Type_use *tu);
    llvm::Type* Get_return_type(Jit_data * jd, Type_use * tu);
    void Run(Jit_data* jd, Ake_ast* n, struct buffer* bf);
    llvm::BasicBlock* Get_last_block(Jit_data * jd, llvm::Function * f);
    llvm::Value* Dispatch(Jit_data* jd, Ake_ast* n);
    void Array_copy(
            Jit_data* jd,
            Type_use* lhs_tu,
            Type_use* rhs_tu,
            llvm::Value* lhs_ptr,
            llvm::Value* rhs_ptr);
}

#endif