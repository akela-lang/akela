#include "tools.h"

using namespace llvm;

namespace Akela_llvm {
    Value* Assign_lhs_rhs(Jit_data* jd, Ake_Ast* lhs, Ake_Ast* rhs);
    Value* Assign_lhs_rhs_value(Jit_data* jd, Ake_Ast* lhs, Ake_Ast* rhs, Value* rhs_value);
    void Array_literal_element(Jit_data* jd, Ake_Ast* n, Value* ptr);

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_variable_dec(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* lhs = Ast_node_get(n, 0);
        Ake_Ast* type_node = Ast_node_get(n, 1);
        Ake_Ast* rhs = Ast_node_get(n, 2);
        Ake_Type* type = type_node->type;
        Ake_Environment* env = Ake_get_current_env(type_node);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &lhs->value, type_node->loc.start);
        assert(sym);
        if (type->kind == AKE_TYPE_FUNCTION) {
            if (rhs) {
                FunctionType *func_type = Get_function_type(jd, type);
                //PointerType *pt = func_type->getPointerTo();
                PointerType *pt = PointerType::get(func_type, 0);
                Zinc_string_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(pt, nullptr, lhs->value.buf);
                sym->reference = lhs_value;
                Value* rhs_value = Dispatch(jd, rhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                if (IsArray(type->kind)) {
                    Type *t = Get_type(jd, type);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    sym->value = lhs_value;
                } else {
                    Type *t = Get_type_pointer(jd, type);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    sym->reference = lhs_value;
                }
            }
        } else if (IsArray(type->kind)) {
            if (rhs) {
                Type *t = Get_type(jd, type);
                Zinc_string_finish(&lhs->value);
                AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                sym->value = lhs_value;
                if (rhs->kind == Ake_ast_type_array_literal) {
                    Value *rhs_value = Dispatch(jd, rhs);
                } else {
                    Value *rhs_value = Dispatch(jd, rhs);
                    Array_copy(jd, type, rhs->type, lhs_value, rhs_value);
                }
            } else {
                Type *t = Get_type(jd, type);
                Zinc_string_finish(&lhs->value);
                AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                sym->value = lhs_value;
            }
        } else if (type->kind == AKE_TYPE_STRUCT) {
            if (rhs) {
                Type *t = Get_type(jd, type);
                Zinc_string_finish(&lhs->value);
                AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                sym->value = lhs_value;
                Value *rhs_value = Dispatch(jd, rhs);
            } else {
                Type *t = Get_type(jd, type);
                Zinc_string_finish(&lhs->value);
                AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                sym->value = lhs_value;
            }
        } else {
            if (rhs) {
                Type* t = Get_type(jd, type);
                Zinc_string_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                sym->reference = lhs_value;
                Value *rhs_value = Dispatch(jd, rhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                Type* t = Get_type(jd, type);
                Zinc_string_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                sym->reference = lhs_value;                }
        }

        return nullptr;
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_assign(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* rhs = n->tail;
        Ake_Ast* lhs = rhs->prev;
        if (lhs->kind == Ake_ast_type_eseq && rhs->kind == Ake_ast_type_eseq) {
            Ake_Ast* lhs2 = lhs->head;
            Ake_Ast* rhs2 = rhs->head;
            while (lhs2 && rhs2) {
                Assign_lhs_rhs(jd, lhs2, rhs2);
                lhs2 = lhs2->next;
                rhs2 = rhs2->next;
            }
            return nullptr;
        } else {
            Value* rhs_value = Dispatch(jd, rhs);
            while (lhs) {
                Assign_lhs_rhs_value(jd, lhs, rhs, rhs_value);
                lhs = lhs->prev;
            }
            return rhs_value;
        }
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Assign_lhs_rhs(Jit_data* jd, Ake_Ast* lhs, Ake_Ast* rhs) {
        Value* rhs_value = Dispatch(jd, rhs);
        return Assign_lhs_rhs_value(jd, lhs, rhs, rhs_value);
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Assign_lhs_rhs_value(Jit_data* jd, Ake_Ast* lhs, Ake_Ast* rhs, Value* rhs_value)
    {
        Ake_Environment* env = Ake_get_current_env(lhs);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &lhs->value, lhs->loc.end);
        if (lhs->type->kind == AKE_TYPE_FUNCTION) {
            if (lhs->kind == Ake_ast_type_id) {
                AllocaInst *lhs_value;
                if (sym->reference) {
                    lhs_value = (AllocaInst *) sym->reference;
                } else {
                    sym->value = nullptr;
                    FunctionType *func_type = Get_function_type(jd, rhs->type);
                    //PointerType *pt = func_type->getPointerTo();
                    PointerType *pt = PointerType::get(func_type, 0);
                    Zinc_string_finish(&rhs->value);
                    lhs_value = jd->Builder->CreateAlloca(pt, nullptr, rhs->value.buf);
                    sym->reference = lhs_value;
                }
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else if (lhs->kind == Ake_ast_type_array_subscript) {
                lhs->type->context = Ake_type_context_ptr;
                Value* lhs_value = Dispatch(jd, lhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                assert(false);
            }
        } else if (IsArray(lhs->type->kind)) {
            lhs->type->context = Ake_type_context_ptr;
            Value* lhs_value = Dispatch(jd, lhs);
            Array_copy(jd, lhs->type, rhs->type, lhs_value, rhs_value);
        } else {
            if (lhs->kind == Ake_ast_type_id) {
                AllocaInst* lhs_value;
                lhs_value = (AllocaInst*)sym->reference;
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                lhs->type->context = Ake_type_context_ptr;
                Value* lhs_value = Dispatch(jd, lhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            }
        }

        return rhs_value;
    }

    Value* Handle_identifier(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Environment* env = Ake_get_current_env(n);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &n->value, n->loc.end);
        if (sym->value) {
            return (Value*)sym->value;
        } else if (sym->reference) {
            auto v = (AllocaInst*)sym->reference;
            Type* t = v->getAllocatedType();
            Zinc_string_finish(&n->value);
            return jd->Builder->CreateLoad(t, v);
        } else {
            assert(false);
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_array_literal(Jit_data* jd, Ake_Ast* n)
    {
        assert(IsArray(n->type->kind));
        std::vector<size_t> index;
        Type *t = Get_type(jd, n->type);
        Value* ptr;
        if (n->parent->kind == Ake_ast_type_const || n->parent->kind == Ake_ast_type_var) {
            Ake_Environment* env = Ake_get_current_env(n->parent);
            Ake_Ast* lhs =  Ast_node_get(n->parent, 0);
            Ake_Ast* type_node = Ast_node_get(n->parent, 1);
            Ake_symbol* sym = Ake_EnvironmentGet(env, &lhs->value, type_node->loc.start);
            ptr = (Value*)sym->value;
        } else {
            ptr = jd->Builder->CreateAlloca(t, nullptr, "arrayliteraltmp");
        }
        Array_literal_element(jd, n, ptr);
        return ptr;
    }

        /* NOLINTNEXTLINE(misc-no-recursion) */
    void Array_literal_element(Jit_data* jd, Ake_Ast* n, Value* ptr)
    {
        if (IsArray(n->type->kind)) {
            Type* t = Get_type(jd, n->type);
            size_t i = 0;
            std::vector<Value*> list;
            list.push_back(
                    ConstantInt::get(Type::getInt64Ty(*jd->TheContext),
                                            APInt(64, 0, false)));
            list.push_back(
                    ConstantInt::get(Type::getInt64Ty(*jd->TheContext),
                                     APInt(64, 0, false)));
            Ake_Ast* p = n->head;
            while (p) {
                Value* ptr2 = jd->Builder->CreateInBoundsGEP(t, ptr, list, "arrayelementtmp");
                Array_literal_element(jd, p, ptr2);

                ptr = ptr2;
                i++;
                p = p->next;
                if (!p) {
                    break;
                }
                t = Get_type(jd, p->type);
                list.clear();
                list.push_back(
                        ConstantInt::get(Type::getInt64Ty(*jd->TheContext),
                                         APInt(64, 1, false)));
            }
        } else {
            Value* value = Dispatch(jd, n);
            jd->Builder->CreateStore(value, ptr);
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_subscript(Jit_data* jd, Ake_Ast* n)
    {
        Type* element_type = Get_type(jd, n->type);
        if (n->type->kind == AKE_TYPE_FUNCTION) {
            //element_type = element_type->getPointerTo();
            element_type = PointerType::get(element_type, 0);
        }

        Ake_Ast* array = n->head;
        assert(IsArray(array->type->kind));
        array->type->context = Ake_type_context_ptr;
        Value* array_value = Dispatch(jd, array);
        assert(array_value);

        Ake_Ast* subscript = array->next;
        Value* subscript_value = Dispatch(jd, subscript);
        size_t dim = 0;
        if (array->type->kind == AKE_TYPE_ARRAY) {
            dim = array->type->data.array.dim;
        } else if (array->type->kind == AKE_TYPE_ARRAY_CONST) {
            dim = array->type->data.array_const.dim;
        } else {
            assert(false && "expected an array");
        }

        Type* size_type = Type::getInt64Ty(*jd->TheContext);
        Value* size_value = ConstantInt::get(size_type, APInt(64, dim, false));

        Value* cond = jd->Builder->CreateICmpULT(subscript_value, size_value);

        BasicBlock* abort_block = BasicBlock::Create(*jd->TheContext, "aborttmp", jd->current_function.back());
        BasicBlock* continue_block = BasicBlock::Create(*jd->TheContext, "continuetmp", jd->current_function.back());

        jd->Builder->CreateCondBr(cond, continue_block, abort_block);

        jd->Builder->SetInsertPoint(abort_block);

        std::vector<Value*> printf_args;
        Value* printf_string = jd->Builder->CreateGlobalString("invalid subscript index", ".str");
        printf_args.push_back(printf_string);
        jd->Builder->CreateCall(jd->printf_function, printf_args);

        std::vector<Value*> exit_args;
        Type* status_type = Type::getInt32Ty(*jd->TheContext);
        Value* status_value = ConstantInt::get(status_type, APInt(32, 1, false));
        exit_args.push_back(status_value);
        jd->Builder->CreateCall(jd->exit_function, exit_args);
        jd->Builder->CreateBr(continue_block);

        jd->Builder->SetInsertPoint(continue_block);

        std::vector<Value*> list;
        list.push_back(subscript_value);
        Value* element_ptr = jd->Builder->CreateInBoundsGEP(element_type, array_value, list, "subscripttmp");

        if (IsArray(n->type->kind)) {
            return element_ptr;
        } else {
            if (n->type->context == Ake_type_context_ptr) {
                return element_ptr;
            } else {
                return jd->Builder->CreateLoad(element_type, element_ptr, "elementtmp");
            }
        }
    }
}