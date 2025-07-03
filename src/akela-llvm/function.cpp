#include <akela/parse_types.h>
#include "tools.h"
#include "cg.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_extern(Jit_data* jd, Ake_Ast* n)
    {
        FunctionType* func_type = Get_function_type(jd, n->type);
        Ake_Ast *proto = Ake_ast_get(n, 0);
        Ake_Ast *id = Ake_ast_get(proto, 0);
        Zinc_string_finish(&id->value);
        Function* f = Function::Create(func_type, GlobalValue::ExternalLinkage, id->value.buf, *jd->TheModule);

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);

        Ake_Environment* env = Ake_get_current_env(n);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &id->value, n->loc.end);
        sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_function(Jit_data* jd, Ake_Ast* n)
    {
        FunctionType* func_type = Get_function_type(jd, n->type);
        Ake_Ast *proto = Ake_ast_get(n, 0);
        Ake_Ast *id = Ake_ast_get(proto, 0);
        Zinc_string_finish(&id->value);
        Function* f = Function::Create(func_type,
                                       GlobalValue::ExternalLinkage,
                                       id->value.buf,
                                       *jd->TheModule);
        BasicBlock* body_block = BasicBlock::Create(*jd->TheContext, "body", f);
        jd->Builder->SetInsertPoint(body_block);

        Ake_Ast* dseq = Ake_ast_get(proto, 1);
        Ake_Ast* dec = dseq->head;
        int i = 0;
        while (dec) {
            Ake_Ast* dec_id = Ake_ast_get(dec, 0);
            Ake_Ast* dec_type = Ake_ast_get(dec, 1);
            Value* dec_value = &f->arg_begin()[i];

            Zinc_string_finish(&dec_id->value);
            Type* t = Get_type_pointer(jd, dec_type->type);
            Value* lhs = jd->Builder->CreateAlloca(t,
                                                   nullptr,
                                                   dec_id->value.buf);
            jd->Builder->CreateStore(dec_value, lhs);

            Ake_Environment* env = Ake_get_current_env(n);
            Ake_symbol* sym = Ake_EnvironmentGet(env, &dec_id->value, dec->loc.end);
            sym->reference = lhs;

            dec = dec->next;
            i++;
        }

        jd->current_function.push_back(f);
        Ake_Ast* body = Ake_ast_get(n, 1);
        Value* ret_value = Dispatch(jd, body);
        if (body->type) {
            jd->Builder->CreateRet(ret_value);
        } else {
            jd->Builder->CreateRetVoid();
        }
        jd->current_function.pop_back();

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);

        Ake_Environment* env = Ake_get_current_env(n);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &id->value, n->loc.start);
        sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_call(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* callee = Ake_ast_get(n, 0);
        Ake_Ast* cseq = Ake_ast_get(n, 1);

        assert(callee && callee->type && callee->type->kind == AKE_TYPE_FUNCTION);
        Value* callee_value = Dispatch(jd, callee);

        std::vector<Value*> arg_list;
        Ake_Ast* arg = cseq->head;
        Ake_Type* func_type = callee->type;

        Ake_TypeParam* tp = func_type->data.function.input_head;
        while (tp) {
            Ake_Type* param_tu = tp->type;
            Ake_Type* arg_tu = arg->type;
            if (arg->kind == Ake_ast_type_number) {
                if ((arg_tu->kind == AKE_TYPE_INTEGER && arg_tu->data.integer.bit_count == 32)
                    && (param_tu->kind == AKE_TYPE_NATURAL && param_tu->data.natural.bit_count == 64)) {
                    arg_tu->kind = AKE_TYPE_NATURAL;
                    arg_tu->data.natural.bit_count = 64;
                }
            }

            Value* value = Dispatch(jd, arg);
            arg_list.push_back(value);
            arg = arg->next;
            tp = tp->next;
        }

        FunctionType *t = Get_function_type(jd, callee->type);
        return jd->Builder->CreateCall(t, callee_value, arg_list);
    }
}