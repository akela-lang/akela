#include "akela_llvm_tools.h"

using namespace llvm;

Value* Code_gen_llvm_assign_lhs_rhs(JITData* jd, Ast_node* lhs, Ast_node* rhs);
Value* Code_gen_llvm_assign_lhs_rhs_value(JITData* jd, Ast_node* lhs, Ast_node* rhs, Value* rhs_value);

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMVar(JITData* jd, Ast_node* n)
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
                FunctionType *func_type = CodeGenLLVMFunctionType(jd, tu);
                PointerType *pt = func_type->getPointerTo();
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(pt, nullptr, lhs->value.buf);
                lhs->sym->reference = lhs_value;
                Value* rhs_value = CodeGenLLVMDispatch(jd, rhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            }
        } else if (tu->to.is_array) {
            if (rhs) {
                Type* t = CodeGenLLVMGetType(jd, tu);
                t = t->getPointerTo();
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                Value *rhs_value = CodeGenLLVMDispatch(jd, rhs);
                lhs->sym->reference = lhs_value;
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                Type* t = CodeGenLLVMGetType(jd, tu);
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                lhs->sym->value = lhs_value;
            }
        } else {
            if (rhs) {
                Type* t = CodeGenLLVMGetType(jd, tu);
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                lhs->sym->reference = lhs_value;
                Value *rhs_value = CodeGenLLVMDispatch(jd, rhs);
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
Value* CodeGenLLVMAssign(JITData* jd, Ast_node* n)
{
    Ast_node* rhs = n->tail;
    Ast_node* lhs = rhs->prev;
    if (lhs->type == ast_type_eseq && rhs->type == ast_type_eseq) {
        Ast_node* lhs2 = lhs->head;
        Ast_node* rhs2 = rhs->head;
        while (lhs2 && rhs2) {
            Code_gen_llvm_assign_lhs_rhs(jd, lhs2, rhs2);
            lhs2 = lhs2->next;
            rhs2 = rhs2->next;
        }
        return nullptr;
    } else {
        Value* rhs_value = CodeGenLLVMDispatch(jd, rhs);
        while (lhs) {
            Code_gen_llvm_assign_lhs_rhs_value(jd, lhs, rhs, rhs_value);
            lhs = lhs->prev;
        }
        return rhs_value;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* Code_gen_llvm_assign_lhs_rhs(JITData* jd, Ast_node* lhs, Ast_node* rhs) {
    Value* rhs_value = CodeGenLLVMDispatch(jd, rhs);
    return Code_gen_llvm_assign_lhs_rhs_value(jd, lhs, rhs, rhs_value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* Code_gen_llvm_assign_lhs_rhs_value(JITData* jd, Ast_node* lhs, Ast_node* rhs, Value* rhs_value)
{
    if (lhs->tu->td->type == type_function) {
        if (lhs->type == ast_type_id) {
            AllocaInst* lhs_value;
            if (lhs->sym->reference) {
                lhs_value = (AllocaInst*)lhs->sym->reference;
            } else {
                lhs->sym->value = nullptr;
                FunctionType *func_type = CodeGenLLVMFunctionType(jd, rhs->tu);
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
                Type *t = CodeGenLLVMGetType(jd, lhs->tu);
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
                Type* t = CodeGenLLVMGetType(jd, lhs->tu);
                buffer_finish(&lhs->value);
                lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                lhs->sym->reference = lhs_value;
            }
            jd->Builder->CreateStore(rhs_value, lhs_value);
        } else {
            jd->context.in_lhs = true;
            Value* lhs_value = CodeGenLLVMDispatch(jd, lhs);
            jd->context.in_lhs = false;
            jd->Builder->CreateStore(rhs_value, lhs_value);
        }
    }

    return rhs_value;
}

Value* CodeGenLLVMID(JITData* jd, Ast_node* n)
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

namespace Code_gen_llvm {
    class Array_literal {
    public:

        /* NOLINTNEXTLINE(misc-no-recursion) */
        Value* code_gen(JITData* jd, Ast_node* n)
        {
            std::vector<size_t> index;
            Type *t = CodeGenLLVMGetType(jd, n->tu);
            Value* ptr = jd->Builder->CreateAlloca(t, nullptr, "arrayliteraltmp");
            code_gen_element(jd, n, ptr);
            return ptr;
        }

    private:

        /* NOLINTNEXTLINE(misc-no-recursion) */
        void code_gen_element(JITData* jd,
                              Ast_node* n,
                              Value* ptr)
        {
            if (n->tu->to.is_array) {
                size_t i = 0;
                Ast_node* p = n->head;
                while (p) {
                    Type* t = CodeGenLLVMGetType(jd, p->tu);
                    std::vector<Value*> list;
                    list.push_back(
                            ConstantInt::get(Type::getInt64Ty(*jd->TheContext),
                                             APInt(64, i, false)));
                    Value* ptr2 = jd->Builder->CreateInBoundsGEP(t, ptr, list, "arrayelementtmp");
                    code_gen_element(jd, p, ptr2);
                    i++;
                    p = p->next;
                }
            } else {
                Value* value = CodeGenLLVMDispatch(jd, n);
                jd->Builder->CreateStore(value, ptr);
            }
        }
    };
};

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMArrayLiteral(JITData* jd, Ast_node* n)
{
    assert(n->tu->to.is_array);
    Code_gen_llvm::Array_literal array_literal;
    return array_literal.code_gen(jd, n);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMSubscript(JITData* jd, Ast_node* n)
{
    Type* element_type = CodeGenLLVMGetType(jd, n->tu);

    Ast_node* array = n->head;
    assert(array->tu->to.is_array);
    Value* array_value = CodeGenLLVMDispatch(jd, array);
    assert(array_value);

    Ast_node* subscript = array->next;
    Value* subscript_value = CodeGenLLVMDispatch(jd, subscript);
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
