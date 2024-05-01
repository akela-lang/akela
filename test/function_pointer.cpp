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

void Run(JITData* jd, std::string* s)
{
    auto ExprSymbol = jd->ExitOnErr(jd->TheJIT->lookup(TOPLEVEL_NAME));
    int (*fp)() = ExprSymbol.getAddress().toPtr <int(*)()>();
    int v = fp();
    *s = std::to_string(v);
}

BasicBlock* GetLastBlock(Function* f)
{
    BasicBlock* last_block = nullptr;
    Function::iterator blocks = f->end();
    if (blocks != f->begin()) {
        last_block = &*--blocks;
    }
    return last_block;
}

bool CreateFoo(JITData* jd, Function** foo)
{
    bool valid = true;

    std::vector<Type*> param_types = std::vector<Type*>();
    param_types.push_back(Type::getInt64Ty(*jd->TheContext));
    Type* ret_type = Type::getInt64Ty(*jd->TheContext);
    FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
    Function *f = Function::Create(func_type, Function::ExternalLinkage, "foo", *jd->TheModule);
    if (verifyFunction(*f, &errs())) {
        valid = false;
    }
    BasicBlock* entry = BasicBlock::Create(*jd->TheContext, "entry", f);
    jd->Builder->SetInsertPoint(entry);
    Value* x;
    for (auto &arg: f->args()) {
        x = jd->Builder->CreateAlloca(arg.getType(), nullptr, "x");
        jd->Builder->CreateStore(&arg, x);
    }
    Value* x2 = jd->Builder->CreateLoad(Type::getInt64Ty(*jd->TheContext), x);
    Value* one = ConstantInt::get(Type::getInt64Ty(*jd->TheContext), APInt(64, 1, true));
    Value* result = jd->Builder->CreateAdd(x2, one, "addtmp");
    jd->Builder->CreateRet(result);

    *foo = f;

    return valid;
}

bool CreateTopLevel(JITData* jd, Function* foo)
{
    bool valid = true;

    std::vector<Type*> param_types = std::vector<Type*>();
    Type* ret_type = Type::getInt64Ty(*jd->TheContext);
    FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
    Function *f = Function::Create(func_type, Function::ExternalLinkage, TOPLEVEL_NAME, *jd->TheModule);
    jd->toplevel = f;
    if (verifyFunction(*f, &errs())) {
        valid = false;
    }
    BasicBlock* entry = BasicBlock::Create(*jd->TheContext, "entry", f);
    jd->Builder->SetInsertPoint(entry);
    Value* one = ConstantInt::get(Type::getInt64Ty(*jd->TheContext), APInt(64, 1, true));
    std::vector<Value*> args;
    args.push_back(one);

    PointerType* pt = foo->getFunctionType()->getPointerTo();
    Value* p = jd->Builder->CreateAlloca(pt, nullptr, "p");
    jd->Builder->CreateStore(foo, p);
    Value* temp = jd->Builder->CreateLoad(pt, p);

    Value* result = jd->Builder->CreateCall(foo->getFunctionType(), temp, args);

    jd->Builder->CreateRet(result);

    return valid;
}

int main()
{
    bool valid = true;

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    JITData jd;
    JITDataInit(&jd);

    Function* foo = nullptr;
    valid = CreateFoo(&jd, &foo) && valid;
    valid = CreateTopLevel(&jd, foo) && valid;

    if (verifyModule(*jd.TheModule, &errs())) {
        valid = false;
    }
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