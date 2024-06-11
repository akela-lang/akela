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
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#define TOPLEVEL_NAME "__toplevel"
#define MODULE_NAME "Akela JIT"

using namespace llvm;
using namespace llvm::orc;

typedef struct {
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<Module> TheModule;
    std::unique_ptr<IRBuilder<>> Builder;
    ExitOnError ExitOnErr;
    std::unique_ptr<KaleidoscopeJIT> TheJIT;
    Function* toplevel;
} JITData;

void JITDataInit(JITData* jd)
{
    jd->TheJIT = jd->ExitOnErr(KaleidoscopeJIT::Create());
    jd->TheContext = std::make_unique<LLVMContext>();
    jd->TheModule = std::make_unique<Module>(MODULE_NAME, *jd->TheContext);
    jd->TheModule->setDataLayout(jd->TheJIT->getDataLayout());
    jd->Builder = std::make_unique<IRBuilder<>>(*jd->TheContext);
    jd->toplevel = nullptr;
}

void CreateTopLevel(JITData* jd)
{
    std::vector<Type*> param_types = std::vector<Type*>();
    Type* element_type = Type::getInt8Ty(*jd->TheContext);
    ArrayType* array_type = ArrayType::get(element_type, 6);
    PointerType* pt = array_type->getPointerTo();
    Type* ret_type = pt;
    FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
    Function *f = Function::Create(func_type, Function::ExternalLinkage, TOPLEVEL_NAME, *jd->TheModule);
    BasicBlock* entry = BasicBlock::Create(*jd->TheContext, "entry", f);
    jd->Builder->SetInsertPoint(entry);

    Value* value = jd->Builder->CreateGlobalStringPtr("hello", ".str");
    jd->Builder->CreateRet(value);
}

void Run(JITData* jd, std::string* s)
{
    auto ExprSymbol = jd->ExitOnErr(jd->TheJIT->lookup(TOPLEVEL_NAME));
    char* (*fp)() = ExprSymbol.getAddress().toPtr <char*(*)()>();
    char* p = fp();
    printf("%s\n", p);
}

int main()
{
    bool valid = true;

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    JITData jd;
    JITDataInit(&jd);

    CreateTopLevel(&jd);

    if (verifyModule(*jd.TheModule, &errs())) {
        valid = false;
    }
    printf("\n");
    jd.TheModule->print(outs(), nullptr);

    if (valid) {
        auto rt = jd.TheJIT->getMainJITDylib().createResourceTracker();
        auto tsm = ThreadSafeModule(std::move(jd.TheModule), std::move(jd.TheContext));
        jd.ExitOnErr(jd.TheJIT->addModule(std::move(tsm), rt));
        std::string s;
        Run(&jd, &s);
        std::cout << s << std::endl;
        jd.ExitOnErr(rt->remove());
    }

    return 0;
}