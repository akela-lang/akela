#include "akela_llvm_tools.h"
#include "akela_llvm.h"

using namespace llvm;
using namespace llvm::orc;

Value* CodeGenLLVMExtern(JITData* jd, struct ast_node* n)
{
    FunctionType* func_type = CodeGenLLVMFunctionType(jd, n->tu);
    struct ast_node *proto = ast_node_get(n, 0);
    struct ast_node *id = ast_node_get(proto, 0);
    buffer_finish(&id->value);
    Function* f = Function::Create(func_type, GlobalValue::ExternalLinkage, id->value.buf, *jd->TheModule);

    BasicBlock* last_block = CodeGenLLVMGetLastBlock(jd, jd->toplevel);
    jd->Builder->SetInsertPoint(last_block);

    n->sym->value = f;

    return f;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMFunction(JITData* jd, struct ast_node* n)
{
    FunctionType* func_type = CodeGenLLVMFunctionType(jd, n->tu);
    struct ast_node *proto = ast_node_get(n, 0);
    struct ast_node *id = ast_node_get(proto, 0);
    buffer_finish(&id->value);
    Function* f = Function::Create(func_type,
                                   GlobalValue::ExternalLinkage,
                                   id->value.buf,
                                   *jd->TheModule);
    BasicBlock* body_block = BasicBlock::Create(*jd->TheContext, "body", f);
    jd->Builder->SetInsertPoint(body_block);

    struct ast_node* dseq = ast_node_get(proto, 1);
    struct ast_node* dec = dseq->head;
    int i = 0;
    while (dec) {
        struct ast_node* dec_id = ast_node_get(dec, 0);
        struct ast_node* dec_type = ast_node_get(dec, 1);
        buffer_finish(&dec_id->value);
        Type* t = CodeGenLLVMGetType(jd, dec_type);
        Value* lhs = jd->Builder->CreateAlloca(t,
                                               nullptr,
                                               dec_id->value.buf);
        jd->Builder->CreateStore(&f->arg_begin()[i], lhs);
        dec_id->sym->reference = lhs;
        dec = dec->next;
        i++;
    }

    struct ast_node* body = ast_node_get(n, 1);
    Value* ret_value = CodeGenLLVMDispatch(jd, body);
    if (body->tu) {
        jd->Builder->CreateRet(ret_value);
    } else {
        jd->Builder->CreateRetVoid();
    }

    BasicBlock* last_block = CodeGenLLVMGetLastBlock(jd, jd->toplevel);
    jd->Builder->SetInsertPoint(last_block);

    n->sym->value = f;

    return f;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMCall(JITData* jd, struct ast_node* n)
{
    struct ast_node* callee = ast_node_get(n, 0);
    struct ast_node* cseq = ast_node_get(n, 1);

    assert(callee && callee->tu && callee->tu->td && callee->tu->td->type == type_function);
    Value* callee_value = CodeGenLLVMDispatch(jd, callee);

    std::vector<Value*> arg_list;
    struct ast_node* arg = cseq->head;
    while (arg) {
        Value* value = CodeGenLLVMDispatch(jd, arg);
        arg_list.push_back(value);
        arg = arg->next;
    }

    FunctionType *t = CodeGenLLVMFunctionType(jd, callee->tu);
    return jd->Builder->CreateCall(t, callee_value, arg_list);
}