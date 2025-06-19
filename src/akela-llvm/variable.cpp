#include "tools.h"

using namespace llvm;

namespace Akela_llvm {
    Value* Assign_lhs_rhs(Jit_data* jd, Ake_ast* lhs, Ake_ast* rhs);
    Value* Assign_lhs_rhs_value(Jit_data* jd, Ake_ast* lhs, Ake_ast* rhs, Value* rhs_value);
    void Array_literal_element(Jit_data* jd, Ake_ast* n, Value* ptr);

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_variable_dec(Jit_data* jd, Ake_ast* n)
    {
        Ake_ast* lseq = Ast_node_get(n, 0);
        Ake_ast* type_node = Ast_node_get(n, 1);
        Ake_ast* rseq = Ast_node_get(n, 2);
        Ake_type_use* tu = type_node->tu;

        Ake_ast *lhs = Ast_node_get(lseq, 0);
        Ake_ast *rhs = nullptr;
        if (rseq) {
            rhs = Ast_node_get(rseq, 0);
        }
        while (lhs) {
            Ake_Environment* env = Ake_get_current_env(type_node);
            Ake_symbol* sym = Ake_EnvironmentGet(env, &lhs->value, type_node->loc.start);
            assert(sym);
            if (tu->type == Ake_type_use_function) {
                if (rhs) {
                    FunctionType *func_type = Get_function_type(jd, tu);
                    //PointerType *pt = func_type->getPointerTo();
                    PointerType *pt = PointerType::get(func_type, 0);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(pt, nullptr, lhs->value.buf);
                    sym->reference = lhs_value;
                    Value* rhs_value = Dispatch(jd, rhs);
                    jd->Builder->CreateStore(rhs_value, lhs_value);
                } else {
                    if (tu->is_array) {
                        Type *t = Get_type(jd, tu);
                        Zinc_string_finish(&lhs->value);
                        AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                        sym->value = lhs_value;
                    } else {
                        Type *t = Get_type_pointer(jd, tu);
                        Zinc_string_finish(&lhs->value);
                        AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                        sym->reference = lhs_value;
                    }
                }
            } else if (tu->is_array) {
                if (rhs) {
                    Type *t = Get_type(jd, tu);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    rhs->tu->lhs_allocation = lhs_value;
                    if (rhs->type == Ake_ast_type_array_literal) {
                        Value *rhs_value = Dispatch(jd, rhs);
                    } else {
                        Value *rhs_value = Dispatch(jd, rhs);
                        Array_copy(jd, tu, rhs->tu, lhs_value, rhs_value);
                    }
                    sym->value = lhs_value;
                } else {
                    Type *t = Get_type(jd, tu);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    sym->value = lhs_value;
                }
            } else if (tu->td->kind == AKE_TYPE_DEF_STRUCT) {
                if (rhs) {
                    Type *t = Get_type(jd, tu);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    rhs->tu->lhs_allocation = lhs_value;
                    Value *rhs_value = Dispatch(jd, rhs);
                    sym->value = lhs_value;
                } else {
                    Type *t = Get_type(jd, tu);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst *lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    sym->value = lhs_value;
                }
            } else {
                if (rhs) {
                    Type* t = Get_type(jd, tu);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    sym->reference = lhs_value;
                    Value *rhs_value = Dispatch(jd, rhs);
                    jd->Builder->CreateStore(rhs_value, lhs_value);
                } else {
                    Type* t = Get_type(jd, tu);
                    Zinc_string_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    sym->reference = lhs_value;                }
            }
            if (rhs) {
                rhs = rhs->next;
            }
            lhs = lhs->next;
        }

        return nullptr;
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_assign(Jit_data* jd, Ake_ast* n)
    {
        Ake_ast* rhs = n->tail;
        Ake_ast* lhs = rhs->prev;
        if (lhs->type == Ake_ast_type_eseq && rhs->type == Ake_ast_type_eseq) {
            Ake_ast* lhs2 = lhs->head;
            Ake_ast* rhs2 = rhs->head;
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
    Value* Assign_lhs_rhs(Jit_data* jd, Ake_ast* lhs, Ake_ast* rhs) {
        Value* rhs_value = Dispatch(jd, rhs);
        return Assign_lhs_rhs_value(jd, lhs, rhs, rhs_value);
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Assign_lhs_rhs_value(Jit_data* jd, Ake_ast* lhs, Ake_ast* rhs, Value* rhs_value)
    {
        Ake_Environment* env = Ake_get_current_env(lhs);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &lhs->value, lhs->loc.end);
        if (lhs->tu->type == Ake_type_use_function) {
            if (lhs->type == Ake_ast_type_id) {
                AllocaInst *lhs_value;
                if (sym->reference) {
                    lhs_value = (AllocaInst *) sym->reference;
                } else {
                    sym->value = nullptr;
                    FunctionType *func_type = Get_function_type(jd, rhs->tu);
                    //PointerType *pt = func_type->getPointerTo();
                    PointerType *pt = PointerType::get(func_type, 0);
                    Zinc_string_finish(&rhs->value);
                    lhs_value = jd->Builder->CreateAlloca(pt, nullptr, rhs->value.buf);
                    sym->reference = lhs_value;
                }
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else if (lhs->type == Ake_ast_type_array_subscript) {
                lhs->tu->context = Ake_type_context_ptr;
                Value* lhs_value = Dispatch(jd, lhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                assert(false);
            }
        } else if (lhs->tu->is_array) {
            lhs->tu->context = Ake_type_context_ptr;
            Value* lhs_value = Dispatch(jd, lhs);
            Array_copy(jd, lhs->tu, rhs->tu, lhs_value, rhs_value);
        } else {
            if (lhs->type == Ake_ast_type_id) {
                AllocaInst* lhs_value;
                lhs_value = (AllocaInst*)sym->reference;
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                lhs->tu->context = Ake_type_context_ptr;
                Value* lhs_value = Dispatch(jd, lhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            }
        }

        return rhs_value;
    }

    Value* Handle_identifier(Jit_data* jd, Ake_ast* n)
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
    Value* Handle_array_literal(Jit_data* jd, Ake_ast* n)
    {
        assert(n->tu->is_array);
        std::vector<size_t> index;
        Type *t = Get_type(jd, n->tu);
        Value* ptr;
        if (n->tu->lhs_allocation) {
            ptr = (Value*)n->tu->lhs_allocation;
        } else {
            ptr = jd->Builder->CreateAlloca(t, nullptr, "arrayliteraltmp");
        }
        Array_literal_element(jd, n, ptr);
        return ptr;
    }

        /* NOLINTNEXTLINE(misc-no-recursion) */
    void Array_literal_element(Jit_data* jd, Ake_ast* n, Value* ptr)
    {
        if (n->tu->is_array) {
            Type* t = Get_type(jd, n->tu);
            size_t i = 0;
            std::vector<Value*> list;
            list.push_back(
                    ConstantInt::get(Type::getInt64Ty(*jd->TheContext),
                                            APInt(64, 0, false)));
            list.push_back(
                    ConstantInt::get(Type::getInt64Ty(*jd->TheContext),
                                     APInt(64, 0, false)));
            Ake_ast* p = n->head;
            while (p) {
                Value* ptr2 = jd->Builder->CreateInBoundsGEP(t, ptr, list, "arrayelementtmp");
                Array_literal_element(jd, p, ptr2);

                ptr = ptr2;
                i++;
                p = p->next;
                if (!p) {
                    break;
                }
                t = Get_type(jd, p->tu);
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
    Value* Handle_subscript(Jit_data* jd, Ake_ast* n)
    {
        Type* element_type = Get_type(jd, n->tu);
        if (n->tu->type == Ake_type_use_function) {
            //element_type = element_type->getPointerTo();
            element_type = PointerType::get(element_type, 0);
        }

        Ake_ast* array = n->head;
        assert(array->tu->is_array);
        array->tu->context = Ake_type_context_ptr;
        Value* array_value = Dispatch(jd, array);
        assert(array_value);

        Ake_ast* subscript = array->next;
        Value* subscript_value = Dispatch(jd, subscript);

        assert(array->tu->dim.count >= 1);
        size_t size = *(size_t*)ZINC_VECTOR_PTR(&array->tu->dim, 0);
        Type* size_type = Type::getInt64Ty(*jd->TheContext);
        Value* size_value = ConstantInt::get(size_type, APInt(64, size, false));

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

        if (n->tu->is_array) {
            return element_ptr;
        } else {
            if (n->tu->context == Ake_type_context_ptr) {
                return element_ptr;
            } else {
                return jd->Builder->CreateLoad(element_type, element_ptr, "elementtmp");
            }
        }
    }
}