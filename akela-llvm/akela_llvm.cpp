#include "akela_llvm_tools.h"
#include "akela_llvm_literal.h"
#include "akela_llvm_function.h"

using namespace llvm;
using namespace llvm::orc;

CodeGenVTable CodeGenLLVMVTable = {
        .jit_offset = offsetof(CodeGenLLVM, jit),
};

bool CodeGenLLVMJIT(CodeGenLLVM* cg, struct ast_node* n, CodeGenResult* result);

void CodeGenLLVMInit(CodeGenLLVM* cg, struct error_list* el)
{
    cg->el = el;
    cg->jit = (CodeGenInterface) CodeGenLLVMJIT;
    cg->debug = false;
}

void CodeGenLLVMCreate(CodeGenLLVM** cg, struct error_list* el)
{
    *cg = new CodeGenLLVM();
    CodeGenLLVMInit(*cg, el);
}

void CodeGenLLVMDestroy(CodeGenLLVM* cg)
{
    delete cg;
}

bool CodeGenLLVMJIT(CodeGenLLVM* cg, struct ast_node* n, CodeGenResult* result)
{
    bool valid = true;

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    JITData jd;
    JITDataInit(&jd, cg->el);

    std::vector<Type*> param_types = std::vector<Type*>();
    Type* ret_type = CodeGenLLVMReturnType(&jd, n->tu);
    FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
    Function *f = Function::Create(func_type, Function::ExternalLinkage, TOPLEVEL_NAME, *jd.TheModule);
    jd.toplevel = f;

    std::string error_str;
    raw_string_ostream error_os(error_str);
    if (verifyFunction(*f, &error_os)) {
        struct location loc = {};
        location_init(&loc);
        error_list_set(cg->el, &loc, "%s", error_str.c_str());
    }

    BasicBlock* entry = BasicBlock::Create(*jd.TheContext, "entry", f);
    jd.Builder->SetInsertPoint(entry);
    Value* value = CodeGenLLVMDispatch(&jd, n);

    BasicBlock* last_block = nullptr;
    Function::iterator blocks = jd.toplevel->end();
    if (blocks != jd.toplevel->begin()) {
        last_block = &*--blocks;
    }
    jd.Builder->SetInsertPoint(last_block);

    if (n->tu) {
        jd.Builder->CreateRet(value);
    } else {
        jd.Builder->CreateRetVoid();
    }

    if (cg->el->head) {
        valid = false;
    }

    std::string str;
    raw_string_ostream os(str);
    if  (verifyModule(*jd.TheModule, &os)) {
        struct location loc = {};
        location_init(&loc);
        error_list_set(cg->el, &loc, "%s", str.c_str());
        valid = false;
    }

    std::string str2;
    raw_string_ostream os2(str2);
    jd.TheModule->print(os2, nullptr);
    buffer_add_format(&result->text, "%s", str2.c_str());

    if (result->debug) {
        printf("%s\n\n", result->text.buf);
    }

    if (valid) {
        auto rt = jd.TheJIT->getMainJITDylib().createResourceTracker();
        auto tsm = ThreadSafeModule(std::move(jd.TheModule), std::move(jd.TheContext));
        jd.ExitOnErr(jd.TheJIT->addModule(std::move(tsm), rt));
        CodeGenLLVMRun(&jd, n, &result->value);
        jd.ExitOnErr(rt->remove());
    }

    buffer_finish(&result->value);
    buffer_finish(&result->text);

    return true;
}