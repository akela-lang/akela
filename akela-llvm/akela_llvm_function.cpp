#include "akela_llvm_tools.h"
#include "akela_llvm.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_extern(Jit_data* jd, Cob_ast* n)
    {
        FunctionType* func_type = Get_function_type(jd, n->tu);
        Cob_ast *proto = Ast_node_get(n, 0);
        Cob_ast *id = Ast_node_get(proto, 0);
        buffer_finish(&id->value);
        Function* f = Function::Create(func_type, GlobalValue::ExternalLinkage, id->value.buf, *jd->TheModule);

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);

        n->sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_function(Jit_data* jd, Cob_ast* n)
    {
        FunctionType* func_type = Get_function_type(jd, n->tu);
        Cob_ast *proto = Ast_node_get(n, 0);
        Cob_ast *id = Ast_node_get(proto, 0);
        buffer_finish(&id->value);
        Function* f = Function::Create(func_type,
                                       GlobalValue::ExternalLinkage,
                                       id->value.buf,
                                       *jd->TheModule);
        BasicBlock* body_block = BasicBlock::Create(*jd->TheContext, "body", f);
        jd->Builder->SetInsertPoint(body_block);

        Cob_ast* dseq = Ast_node_get(proto, 1);
        Cob_ast* dec = dseq->head;
        int i = 0;
        while (dec) {
            Cob_ast* dec_id = Ast_node_get(dec, 0);
            Cob_ast* dec_type = Ast_node_get(dec, 1);
            Value* dec_value = &f->arg_begin()[i];

            buffer_finish(&dec_id->value);
            Type* t = Get_type_pointer(jd, dec_type->tu);
            Value* lhs = jd->Builder->CreateAlloca(t,
                                                   nullptr,
                                                   dec_id->value.buf);
            jd->Builder->CreateStore(dec_value, lhs);
            dec_id->sym->reference = lhs;

            dec = dec->next;
            i++;
        }

        jd->current_function.push_back(f);
        Cob_ast* body = Ast_node_get(n, 1);
        Value* ret_value = Dispatch(jd, body);
        if (body->tu) {
            jd->Builder->CreateRet(ret_value);
        } else {
            jd->Builder->CreateRetVoid();
        }
        jd->current_function.pop_back();

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);

        n->sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_call(Jit_data* jd, Cob_ast* n)
    {
        Cob_ast* callee = Ast_node_get(n, 0);
        Cob_ast* cseq = Ast_node_get(n, 1);

        assert(callee && callee->tu && callee->tu->td && callee->tu->td->type == type_function);
        Value* callee_value = Dispatch(jd, callee);

        std::vector<Value*> arg_list;
        Cob_ast* arg = cseq->head;
        while (arg) {
            Value* value = Dispatch(jd, arg);
            arg_list.push_back(value);
            arg = arg->next;
        }

        FunctionType *t = Get_function_type(jd, callee->tu);
        return jd->Builder->CreateCall(t, callee_value, arg_list);
    }
}