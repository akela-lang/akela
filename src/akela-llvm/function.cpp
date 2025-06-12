#include <akela/parse_types.h>

#include "tools.h"
#include "cg.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_extern(Jit_data* jd, Ake_ast* n)
    {
        FunctionType* func_type = Get_function_type(jd, n->tu);
        Ake_ast *proto = Ast_node_get(n, 0);
        Ake_ast *id = Ast_node_get(proto, 0);
        Zinc_string_finish(&id->value);
        Function* f = Function::Create(func_type, GlobalValue::ExternalLinkage, id->value.buf, *jd->TheModule);

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);

        Ake_Environment* env = Ake_get_current_env(n);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &id->value, n->loc.end);
        assert(sym == n->sym);
        sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_function(Jit_data* jd, Ake_ast* n)
    {
        FunctionType* func_type = Get_function_type(jd, n->tu);
        Ake_ast *proto = Ast_node_get(n, 0);
        Ake_ast *id = Ast_node_get(proto, 0);
        Zinc_string_finish(&id->value);
        Function* f = Function::Create(func_type,
                                       GlobalValue::ExternalLinkage,
                                       id->value.buf,
                                       *jd->TheModule);
        BasicBlock* body_block = BasicBlock::Create(*jd->TheContext, "body", f);
        jd->Builder->SetInsertPoint(body_block);

        Ake_ast* dseq = Ast_node_get(proto, 1);
        Ake_ast* dec = dseq->head;
        int i = 0;
        while (dec) {
            Ake_ast* dec_id = Ast_node_get(dec, 0);
            Ake_ast* dec_type = Ast_node_get(dec, 1);
            Value* dec_value = &f->arg_begin()[i];

            Zinc_string_finish(&dec_id->value);
            Type* t = Get_type_pointer(jd, dec_type->tu);
            Value* lhs = jd->Builder->CreateAlloca(t,
                                                   nullptr,
                                                   dec_id->value.buf);
            jd->Builder->CreateStore(dec_value, lhs);

            Ake_Environment* env = Ake_get_current_env(n);
            Ake_symbol* sym = Ake_EnvironmentGet(env, &dec_id->value, dec->loc.end);
            assert(sym == dec_id->sym);
            sym->reference = lhs;

            dec = dec->next;
            i++;
        }

        jd->current_function.push_back(f);
        Ake_ast* body = Ast_node_get(n, 1);
        Value* ret_value = Dispatch(jd, body);
        if (body->tu) {
            jd->Builder->CreateRet(ret_value);
        } else {
            jd->Builder->CreateRetVoid();
        }
        jd->current_function.pop_back();

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);

        Ake_Environment* env = Ake_get_current_env(n);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &id->value, n->loc.start);
        assert(sym == n->sym);
        sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_call(Jit_data* jd, Ake_ast* n)
    {
        Ake_ast* callee = Ast_node_get(n, 0);
        Ake_ast* cseq = Ast_node_get(n, 1);

        assert(callee && callee->tu && callee->tu->td && callee->tu->td->type == Ake_type_function);
        Value* callee_value = Dispatch(jd, callee);

        std::vector<Value*> arg_list;
        Ake_ast* arg = cseq->head;
        Ake_type_use* func_type = callee->tu;
        Ake_type_use* inputs = NULL;
        Ake_type_use* outputs = NULL;
        Ake_get_function_children(func_type, &inputs, &outputs);
        Ake_type_use* param_tu = NULL;
        if (inputs) {
            param_tu = inputs->head;
        }
        while (arg) {
            Ake_type_use* arg_tu = arg->tu;
            if (arg->type == Ake_ast_type_number) {
                if (arg_tu->td->type == Ake_type_integer && param_tu->td->type == Ake_type_integer) {
                    if (arg_tu->td->bit_count == 32 && param_tu->td->bit_count == 64) {
                        arg_tu->td = param_tu->td;
                    }
                }
            }

            Value* value = Dispatch(jd, arg);
            arg_list.push_back(value);
            arg = arg->next;
            param_tu = param_tu->next;
        }

        FunctionType *t = Get_function_type(jd, callee->tu);
        return jd->Builder->CreateCall(t, callee_value, arg_list);
    }
}