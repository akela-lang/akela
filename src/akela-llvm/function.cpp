#include <akela/parse_types.h>
#include "tools.h"
#include "cg.h"
#include "akela/update_symbol.h"

using namespace llvm;
using namespace llvm::orc;

namespace Akela_llvm {
    Value* Handle_extern(Jit_data* jd, Ake_Ast* n)
    {
        Ake_UpdateSymbolExtern(&jd->st, n);
        FunctionType* func_type = Get_function_type(jd, n->type);
        Ake_Ast *proto = Ake_AstGet(n, 0);
        Ake_Ast *id = proto->data.prototype.id;
        Zinc_string_finish(&id->data.id.value);
        Function* f = Function::Create(func_type, GlobalValue::ExternalLinkage, id->data.id.value.buf, *jd->TheModule);

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);

        Ake_Environment* env = jd->st.top;
        Ake_symbol* sym = Ake_EnvironmentGet(env, &id->data.id.value);
        sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_function(Jit_data* jd, Ake_Ast* n)
    {
        FunctionType* func_type = Get_function_type(jd, n->type);
        Ake_Ast *proto = n->data.function.proto;
        Ake_Ast *id = proto->data.prototype.id;
        Zinc_string_finish(&id->data.id.value);
        Function* f = Function::Create(func_type,
                                       GlobalValue::ExternalLinkage,
                                       id->data.id.value.buf,
                                       *jd->TheModule);
        BasicBlock* body_block = BasicBlock::Create(*jd->TheContext, "body", f);
        jd->Builder->SetInsertPoint(body_block);

        Ake_begin_environment(&jd->st);
        Ake_UpdateSymbolPrototype(&jd->st, n);

        Ake_Ast* dseq = proto->data.prototype.dseq;
        Ake_Ast* dec = dseq->data.dseq.list.head;
        int i = 0;
        while (dec) {
            Ake_Ast* dec_id = dec->data.declaration.id_node;
            Ake_Ast* dec_type = dec->data.declaration.type_node;
            Value* dec_value = &f->arg_begin()[i];

            Zinc_string_finish(&dec_id->data.id.value);
            Type* t = Get_type_pointer(jd, dec_type->type);
            Value* lhs = jd->Builder->CreateAlloca(t,
                                                   nullptr,
                                                   dec_id->data.id.value.buf);
            jd->Builder->CreateStore(dec_value, lhs);

            Ake_Environment* env = jd->st.top;
            Ake_symbol* sym = Ake_EnvironmentGet(env, &dec_id->data.id.value);
            sym->reference = lhs;

            dec = dec->next;
            i++;
        }

        jd->current_function.push_back(f);
        Ake_Ast* body = n->data.function.body;
        Value* ret_value = Dispatch(jd, body);
        if (body->type) {
            jd->Builder->CreateRet(ret_value);
        } else {
            jd->Builder->CreateRetVoid();
        }
        jd->current_function.pop_back();

        BasicBlock* last_block = Get_last_block(jd, jd->toplevel);
        jd->Builder->SetInsertPoint(last_block);
        Ake_end_environment(&jd->st);

        Ake_UpdateSymbolFunction(&jd->st, n);

        Ake_Environment* env = jd->st.top;
        Ake_symbol* sym = Ake_EnvironmentGet(env, &id->data.id.value);
        sym->value = f;

        return f;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_call(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* callee = n->data.call.func;
        Ake_AstList* args = &n->data.call.args;

        assert(callee && callee->type && callee->type->kind == AKE_TYPE_FUNCTION);
        Value* callee_value = Dispatch(jd, callee);

        std::vector<Value*> arg_list;
        Ake_Ast* arg = args->head;
        Ake_Type* func_type = callee->type;

        Ake_TypeParam* tp = func_type->data.function.input_head;
        while (tp) {
            Ake_Type* param_tu = tp->type;
            Ake_Type* arg_tu = arg->type;
            if (arg->kind == AKE_AST_NUMBER) {
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
