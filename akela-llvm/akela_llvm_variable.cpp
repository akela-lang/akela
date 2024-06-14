#include "akela_llvm_tools.h"

using namespace llvm;

namespace Akela_llvm {
    Value* Assign_lhs_rhs(Jit_data* jd, Ast_node* lhs, Ast_node* rhs);
    Value* Assign_lhs_rhs_value(Jit_data* jd, Ast_node* lhs, Ast_node* rhs, Value* rhs_value);
    void Array_literal_element(Jit_data* jd, Ast_node* n, Value* ptr);

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_let(Jit_data* jd, Ast_node* n)
    {
        Ast_node* lseq = Ast_node_get(n, 0);
        Ast_node* tu = Ast_node_get(n, 1);
        Ast_node* rseq = Ast_node_get(n, 2);

        Ast_node *lhs = Ast_node_get(lseq, 0);
        Ast_node *rhs = nullptr;
        if (rseq) {
            rhs = Ast_node_get(rseq, 0);
        }
        while (lhs) {
            assert(lhs->sym);
            if (tu->td->type == type_function) {
                if (rhs) {
                    FunctionType *func_type = Get_function_type(jd, tu);
                    PointerType *pt = func_type->getPointerTo();
                    buffer_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(pt, nullptr, lhs->value.buf);
                    lhs->sym->reference = lhs_value;
                    Value* rhs_value = Dispatch(jd, rhs);
                    jd->Builder->CreateStore(rhs_value, lhs_value);
                }
            } else if (tu->to.is_array) {
                if (rhs) {
                    Type* t = Get_type(jd, tu);
                    t = t->getPointerTo();
                    buffer_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    Value *rhs_value = Dispatch(jd, rhs);
                    lhs->sym->reference = lhs_value;
                    jd->Builder->CreateStore(rhs_value, lhs_value);
                } else {
                    Type* t = Get_type(jd, tu);
                    buffer_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    lhs->sym->value = lhs_value;
                }
            } else {
                if (rhs) {
                    Type* t = Get_type(jd, tu);
                    buffer_finish(&lhs->value);
                    AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    lhs->sym->reference = lhs_value;
                    Value *rhs_value = Dispatch(jd, rhs);
                    jd->Builder->CreateStore(rhs_value, lhs_value);
                }
            }
            if (rhs) {
                rhs = rhs->next;
            }
            lhs = lhs->next;
        }

        return nullptr;
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_assign(Jit_data* jd, Ast_node* n)
    {
        Ast_node* rhs = n->tail;
        Ast_node* lhs = rhs->prev;
        if (lhs->type == ast_type_eseq && rhs->type == ast_type_eseq) {
            Ast_node* lhs2 = lhs->head;
            Ast_node* rhs2 = rhs->head;
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
    Value* Assign_lhs_rhs(Jit_data* jd, Ast_node* lhs, Ast_node* rhs) {
        Value* rhs_value = Dispatch(jd, rhs);
        return Assign_lhs_rhs_value(jd, lhs, rhs, rhs_value);
    }

/* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Assign_lhs_rhs_value(Jit_data* jd, Ast_node* lhs, Ast_node* rhs, Value* rhs_value)
    {
        if (lhs->tu->td->type == type_function) {
            if (lhs->type == ast_type_id) {
                AllocaInst* lhs_value;
                if (lhs->sym->reference) {
                    lhs_value = (AllocaInst*)lhs->sym->reference;
                } else {
                    lhs->sym->value = nullptr;
                    FunctionType *func_type = Get_function_type(jd, rhs->tu);
                    PointerType *pt = func_type->getPointerTo();
                    buffer_finish(&rhs->value);
                    lhs_value = jd->Builder->CreateAlloca(pt, nullptr, rhs->value.buf);
                    lhs->sym->reference = lhs_value;
                }
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                assert(false);
            }
        } else if (lhs->tu->to.is_array) {
            if (lhs->type == ast_type_id) {
                AllocaInst *lhs_value;
                if (lhs->sym->reference) {
                    lhs_value = (AllocaInst *) lhs->sym->reference;
                } else {
                    lhs->sym->value = nullptr;
                    Type *t = Get_type(jd, lhs->tu);
                    t = t->getPointerTo();
                    buffer_finish(&lhs->value);
                    lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    lhs->sym->reference = lhs_value;
                }
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else if (lhs->type == ast_type_eseq) {
                Ast_node* p = lhs->head;
                while (p) {
                    p = p->next;
                }
            } else {
                assert(false);
            }
        } else {
            if (lhs->type == ast_type_id) {
                AllocaInst* lhs_value;
                if (lhs->sym->reference) {
                    lhs_value = (AllocaInst*)lhs->sym->reference;
                } else {
                    Type* t = Get_type(jd, lhs->tu);
                    buffer_finish(&lhs->value);
                    lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    lhs->sym->reference = lhs_value;
                }
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                jd->context.in_lhs = true;
                Value* lhs_value = Dispatch(jd, lhs);
                jd->context.in_lhs = false;
                jd->Builder->CreateStore(rhs_value, lhs_value);
            }
        }

        return rhs_value;
    }

    Value* Handle_identifier(Jit_data* jd, Ast_node* n)
    {
        buffer_finish(&n->value);
        struct symbol* sym = n->sym;
        if (sym->value) {
            return (Value*)sym->value;
        } else if (sym->reference) {
            auto v = (AllocaInst*)sym->reference;
            Type* t = v->getAllocatedType();
            buffer_finish(&n->value);
            return jd->Builder->CreateLoad(t, v, n->value.buf);
        } else {
            assert(false);
        }
    }

        /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_array_literal(Jit_data* jd, Ast_node* n)
    {
        assert(n->tu->to.is_array);
        std::vector<size_t> index;
        Type *t = Get_type(jd, n->tu);
        Value* ptr = jd->Builder->CreateAlloca(t, nullptr, "arrayliteraltmp");
        Array_literal_element(jd, n, ptr);
        return ptr;
    }

        /* NOLINTNEXTLINE(misc-no-recursion) */
    void Array_literal_element(Jit_data* jd, Ast_node* n, Value* ptr)
    {
        if (n->tu->to.is_array) {
            size_t i = 0;
            Ast_node* p = n->head;
            while (p) {
                Type* t = Get_type(jd, p->tu);
                std::vector<Value*> list;
                list.push_back(
                        ConstantInt::get(Type::getInt64Ty(*jd->TheContext),
                                         APInt(64, i, false)));
                Value* ptr2 = jd->Builder->CreateInBoundsGEP(t, ptr, list, "arrayelementtmp");
                Array_literal_element(jd, p, ptr2);
                i++;
                p = p->next;
            }
        } else {
            Value* value = Dispatch(jd, n);
            jd->Builder->CreateStore(value, ptr);
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_subscript(Jit_data* jd, Ast_node* n)
    {
        Type* element_type = Get_type(jd, n->tu);

        Ast_node* array = n->head;
        assert(array->tu->to.is_array);
        Value* array_value = Dispatch(jd, array);
        assert(array_value);

        Ast_node* subscript = array->next;
        Value* subscript_value = Dispatch(jd, subscript);
        std::vector<Value*> list;
        list.push_back(subscript_value);
        Value* element_ptr = jd->Builder->CreateInBoundsGEP(element_type, array_value, list, "subscripttmp");

        if (n->tu->to.is_array) {
            return element_ptr;
        } else {
            if (jd->context.in_lhs) {
                return element_ptr;
            } else {
                return jd->Builder->CreateLoad(element_type, element_ptr, "elementtmp");
            }
        }
    }
}