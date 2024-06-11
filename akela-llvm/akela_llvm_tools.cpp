#include "akela/parse_types.h"
#include "akela_llvm_tools.h"
#include "akela_llvm_function.h"
#include "akela_llvm_literal.h"

using namespace llvm;
using namespace llvm::orc;

Value* CodeGenLLVMStmts(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMIf(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMVar(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMAssign(JITData* jd, struct ast_node* n);
Value* Code_gen_llvm_assign_lhs_rhs(JITData* jd, struct ast_node* lhs, struct ast_node* rhs);
Value* Code_gen_llvm_assign_lhs_rhs_value(JITData* jd, struct ast_node* lhs, struct ast_node* rhs, Value* rhs_value);
Value* CodeGenLLVMAdd(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMSub(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMID(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMArrayLiteral(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMSubscript(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMSign(JITData* jd, struct ast_node* n);

void JITDataInit(JITData* jd, struct error_list* el)
{
    jd->el = el;
    jd->TheJIT = jd->ExitOnErr(KaleidoscopeJIT::Create());
    jd->TheContext = std::make_unique<LLVMContext>();
    jd->TheModule = std::make_unique<Module>(MODULE_NAME, *jd->TheContext);
    jd->TheModule->setDataLayout(jd->TheJIT->getDataLayout());
    jd->Builder = std::make_unique<IRBuilder<>>(*jd->TheContext);
    jd->toplevel = nullptr;
    jd->context.in_lhs = false;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
FunctionType* CodeGenLLVMFunctionType(JITData* jd, struct ast_node* tu)
{
    struct ast_node *input = nullptr;
    struct ast_node *output = nullptr;
    get_function_children(tu, &input, &output);

    std::vector<Type *> param_types = std::vector<Type *>();
    size_t input_count = ast_node_count_children(input);
    if (input_count > 0) {
        for (size_t i = 0; i < input_count; i++) {
            struct ast_node *dec = ast_node_get(input, i);
            Type *dec_type = CodeGenLLVMGetType(jd, dec);
            param_types.push_back(dec_type);
        }
    }

    Type *ret_type = nullptr;
    if (output) {
        struct ast_node *ret = ast_node_get(output, 0);
        ret_type = CodeGenLLVMReturnType(jd, ret);
    } else {
        ret_type = Type::getVoidTy(*jd->TheContext);
    }

    return FunctionType::get(ret_type, param_types, false);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Type* CodeGenLLVMGetTypeScalar(JITData* jd, struct ast_node* tu)
{
    if (!tu) {
        return Type::getVoidTy(*jd->TheContext);
    }

    struct type_def *td = tu->td;

    if (td->type == type_integer) {
        Type* t = nullptr;
        if (td->bit_count == 64) {
            t = Type::getInt64Ty(*jd->TheContext);
        } else if (td->bit_count == 32) {
            t = Type::getInt32Ty(*jd->TheContext);
        } else if (td->bit_count == 8) {
            t = Type::getInt8Ty(*jd->TheContext);
        } else {
            assert(false);
        }
        return t;
    } else if (td->type == type_float) {
        if (td->bit_count == 64) {
            return Type::getDoubleTy(*jd->TheContext);
        } else if (td->bit_count == 32) {
            return Type::getFloatTy(*jd->TheContext);
        }
    } else if (td->type == type_boolean) {
        return Type::getInt1Ty(*jd->TheContext);
    } else if (td->type == type_function) {
        return CodeGenLLVMFunctionType(jd, tu);
    } else {
        assert(false);
    }

    return nullptr;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Type* CodeGenLLVMGetType(JITData* jd, struct ast_node* tu)
{
    Type *t = CodeGenLLVMGetTypeScalar(jd, tu);
    if (tu && tu->to.is_array) {
        size_t i = tu->to.dim.count - 1;
        while (true) {
            auto dim = (Type_dimension*)VECTOR_PTR(&tu->to.dim, i);
            t = ArrayType::get(t, dim->size);
            if (i == 0) break;
            i--;
        }
    }

    return t;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Type* CodeGenLLVMReturnType(JITData* jd, struct ast_node* tu)
{
    if (tu && tu->td && tu->td->type == type_function) {
        FunctionType *func_type = CodeGenLLVMFunctionType(jd, tu);
        return static_cast<Type *>(func_type->getPointerTo());
    } if (tu && tu->to.is_array) {
        return CodeGenLLVMGetType(jd, tu)->getPointerTo();
    } else {
        return CodeGenLLVMGetType(jd, tu);
    }
}

BasicBlock* CodeGenLLVMGetLastBlock(JITData* jd, Function* f)
{
    BasicBlock* last_block = nullptr;
    Function::iterator blocks = f->end();
    if (blocks != f->begin()) {
        last_block = &*--blocks;
    }
    return last_block;
}

void CodeGenLLVMRun(JITData* jd, struct ast_node* n, struct buffer* bf)
{
    auto ExprSymbol = jd->ExitOnErr(jd->TheJIT->lookup(TOPLEVEL_NAME));
    if (n->tu) {
        enum type type = n->tu->td->type;
        bool is_array = n->tu->to.is_array;
        int bit_count = n->tu->td->bit_count;
        if (type == type_integer) {
            if (is_array) {
                if (bit_count == 64) {
                    long* (*fp)() = ExprSymbol.getAddress().toPtr<long*(*)()>();
                    long* p = fp();
                    Vector* dim_vector = &n->tu->to.dim;
                    size_t count = 1;
                    for (int i = 0; i < dim_vector->count; i++) {
                        auto dim = (Type_dimension*)VECTOR_PTR(dim_vector, i);
                        count *= dim->size;
                    }
                    buffer_add_char(bf, '[');
                    for (int i = 0; i < count; i++) {
                        if (i >= 1) {
                            buffer_add_char(bf, ',');
                        }
                        buffer_add_format(bf, "%ld", *p++);
                    }
                    buffer_add_char(bf, ']');
                } else if (bit_count == 32) {
                    int* (*fp)() = ExprSymbol.getAddress().toPtr<int*(*)()>();
                    int* p = fp();
                    Vector* dim_vector = &n->tu->to.dim;
                    size_t count = 1;
                    for (int i = 0; i < dim_vector->count; i++) {
                        auto dim = (Type_dimension*)VECTOR_PTR(dim_vector, i);
                        count *= dim->size;
                    }
                    buffer_add_char(bf, '[');
                    for (int i = 0; i < count; i++) {
                        if (i >= 1) {
                            buffer_add_char(bf, ',');
                        }
                        buffer_add_format(bf, "%d", *p++);
                    }
                    buffer_add_char(bf, ']');
                } else if (bit_count == 8) {
                    char* (*fp)() = ExprSymbol.getAddress().toPtr<char*(*)()>();
                    char* p = fp();
                    buffer_add_format(bf, "%s", p);
                } else {
                    assert(false);
                }
            } else {
                if (n->tu->td->bit_count == 64) {
                    long (*fp)() = ExprSymbol.getAddress().toPtr<long(*)()>();
                    long v = fp();
                    buffer_add_format(bf, "%d", v);
                } else if (n->tu->td->bit_count == 32) {
                    int (*fp)() = ExprSymbol.getAddress().toPtr < int(*)
                    () > ();
                    int v = fp();
                    buffer_add_format(bf, "%d", v);
                } else if (bit_count == 8) {
                    char (*fp)() = ExprSymbol.getAddress().toPtr<char(*)()> ();
                    char v = fp();
                    buffer_add_format(bf, "%d", v);
                } else {
                    assert(false);
                }
            }
        } else if (type == type_float) {
            if (is_array) {
                double* (*fp)() = ExprSymbol.getAddress().toPtr <double*(*)()>();
                double* p = fp();
                buffer_add_format(bf, "%lf", *p);
            } else {
                double (*fp)() = ExprSymbol.getAddress().toPtr <double(*)()>();
                double v = fp();
                buffer_add_format(bf, "%lf", v);
            }
        } else if (type == type_boolean) {
            bool (*fp)() = ExprSymbol.getAddress().toPtr <bool(*)()>();
            bool v = fp();
            if (v) {
                buffer_add_format(bf, "true", v);
            } else {
                buffer_add_format(bf, "false", v);
            }
        } else if (type == type_function) {
            void* (*fp)() = ExprSymbol.getAddress().toPtr<void*(*)()>();
            void* v = fp();
            buffer_add_format(bf, "Function");
        } else {
            struct location loc{};
            location_init(&loc);
            error_list_set(jd->el, &loc, "type not handled");
        }
    } else {
        void (*fp)() = ExprSymbol.getAddress().toPtr <void(*)()>();
        fp();
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMDispatch(JITData* jd, struct ast_node* n)
{
    if (n->type == ast_type_stmts) {
        return CodeGenLLVMStmts(jd, n);
    } else if (n->type == ast_type_extern) {
        return CodeGenLLVMExtern(jd, n);
    } else if (n->type == ast_type_if) {
        return CodeGenLLVMIf(jd, n);
    } else if (n->type == ast_type_let) {
        return CodeGenLLVMVar(jd, n);
    } else if (n->type == ast_type_function || n->type == ast_type_anonymous_function) {
        return CodeGenLLVMFunction(jd, n);
    } else if (n->type == ast_type_assign) {
        return CodeGenLLVMAssign(jd, n);
    } else if (n->type == ast_type_plus) {
        return CodeGenLLVMAdd(jd, n);
    } else if (n->type == ast_type_minus) {
        return CodeGenLLVMSub(jd, n);
    } else if (n->type == ast_type_call) {
        return CodeGenLLVMCall(jd, n);
    } else if (n->type == ast_type_id) {
        return CodeGenLLVMID(jd, n);
    } else if (n->type == ast_type_array_literal) {
        return CodeGenLLVMArrayLiteral(jd, n);
    } else if (n->type == ast_type_array_subscript) {
        return CodeGenLLVMSubscript(jd, n);
    } else if (n->type == ast_type_sign) {
        return CodeGenLLVMSign(jd, n);
    } else if (n->type == ast_type_number) {
        return CodeGenLLVMNumber(jd, n);
    } else if (n->type == ast_type_boolean) {
        return CodeGenLLVMBoolean(jd, n);
    } else if (n->type == ast_type_string) {
        return CodeGenLLVMString(jd, n);
    } else {
        char* names[ast_type_count];
        ast_set_names(names);
        printf("code gen: unhandled ast node type: %s", names[n->type]);
        exit(1);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMStmts(JITData* jd, struct ast_node* n)
{
    Value* last_v = nullptr;
    struct ast_node* last_n = nullptr;
    struct ast_node* stmt = ast_node_get(n, 0);
    while (stmt) {
        last_v = CodeGenLLVMDispatch(jd, stmt);
        last_n = stmt;
        stmt = stmt->next;
    }
    return last_v;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMIf(JITData* jd, struct ast_node* n)
{
    bool has_else = false;

    Type* type = nullptr;
    Value* ptr = nullptr;
    if (n->tu) {
        type = CodeGenLLVMGetType(jd, n->tu);
    }
    if (type) {
        ptr = jd->Builder->CreateAlloca(type, nullptr, "ifresult");
    }

    struct list l{};
    list_init(&l);

    BasicBlock* cond_block = nullptr;
    BasicBlock* then_block = nullptr;
    BasicBlock* next_block = nullptr;
    int i = 0;
    while (true) {
        struct ast_node* branch = ast_node_get(n, i);
        if (!branch) {
            break;
        }

        if (branch->type == ast_type_conditional_branch) {
            struct ast_node* cond = ast_node_get(branch, 0);
            struct ast_node* body = ast_node_get(branch, 1);

            if (next_block) {
                cond_block = next_block;
            } else {
                if (i >= 1) {
                    cond_block = BasicBlock::Create(*jd->TheContext, "condtmp", jd->toplevel);
                }
            }
            then_block = BasicBlock::Create(*jd->TheContext, "thentmp", jd->toplevel);
            next_block = BasicBlock::Create(*jd->TheContext, "nexttmp", jd->toplevel);

            if (cond_block) {
                jd->Builder->SetInsertPoint(cond_block);
            }
            Value* cond_value = CodeGenLLVMDispatch(jd, cond);
            Value* branch_value = jd->Builder->CreateCondBr(cond_value, then_block, next_block);

            jd->Builder->SetInsertPoint(then_block);
            Value* body_value = CodeGenLLVMStmts(jd, body);
            if (type) {
                jd->Builder->CreateStore(body_value, ptr);
            }
            list_add_item(&l, then_block);      /* branch to end after end_block is created */

            jd->Builder->SetInsertPoint(next_block);

        } else if (branch->type == ast_type_default_branch) {
            struct ast_node* body = ast_node_get(branch, 0);

            Value* body_value = CodeGenLLVMStmts(jd, body);
            if (type) {
                jd->Builder->CreateStore(body_value, ptr);
            }
            list_add_item(&l, next_block);      /* branch to end after end_block is created */
            has_else = true;

        } else {
            assert(false);
        }

        i++;
    }

    BasicBlock* end_block = nullptr;
    if (has_else) {
        end_block = BasicBlock::Create(*jd->TheContext, "endiftmp", jd->toplevel);
    } else {
        end_block = next_block;
    }
    struct list_node* ln = l.head;
    while (ln) {
        auto p = (BasicBlock*)ln->item;
        jd->Builder->SetInsertPoint(p);
        jd->Builder->CreateBr(end_block);
        ln = ln->next;
    }

    jd->Builder->SetInsertPoint(end_block);
    Value* value = nullptr;
    if (type) {
        value = jd->Builder->CreateLoad(type, ptr);
    }

    list_destroy(&l, nullptr);

    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMVar(JITData* jd, struct ast_node* n)
{
    struct ast_node* lseq = ast_node_get(n, 0);
    struct ast_node* tu = ast_node_get(n, 1);
    struct ast_node* rseq = ast_node_get(n, 2);

    struct ast_node *lhs = ast_node_get(lseq, 0);
    struct ast_node *rhs = nullptr;
    if (rseq) {
        rhs = ast_node_get(rseq, 0);
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
Value* CodeGenLLVMAssign(JITData* jd, struct ast_node* n)
{
    struct ast_node* rhs = n->tail;
    struct ast_node* lhs = rhs->prev;
    if (lhs->type == ast_type_eseq && rhs->type == ast_type_eseq) {
        struct ast_node* lhs2 = lhs->head;
        struct ast_node* rhs2 = rhs->head;
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
Value* Code_gen_llvm_assign_lhs_rhs(JITData* jd, struct ast_node* lhs, struct ast_node* rhs) {
    Value* rhs_value = CodeGenLLVMDispatch(jd, rhs);
    return Code_gen_llvm_assign_lhs_rhs_value(jd, lhs, rhs, rhs_value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* Code_gen_llvm_assign_lhs_rhs_value(JITData* jd, struct ast_node* lhs, struct ast_node* rhs, Value* rhs_value)
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
            struct ast_node* p = lhs->head;
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

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMAdd(JITData* jd, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    Value* lhs = CodeGenLLVMDispatch(jd, a);

    struct ast_node* b = ast_node_get(n, 1);
    Value* rhs = CodeGenLLVMDispatch(jd, b);

    return jd->Builder->CreateAdd(lhs, rhs, "addtmp");
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMSub(JITData* jd, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    Value* lhs = CodeGenLLVMDispatch(jd, a);

    struct ast_node* b = ast_node_get(n, 1);
    Value* rhs = CodeGenLLVMDispatch(jd, b);

    return jd->Builder->CreateSub(lhs, rhs, "subtmp");
}

Value* CodeGenLLVMID(JITData* jd, struct ast_node* n)
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
        Value* code_gen(JITData* jd, struct ast_node* n)
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
                              struct ast_node* n,
                              Value* ptr)
        {
            if (n->tu->to.is_array) {
                size_t i = 0;
                struct ast_node* p = n->head;
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
Value* CodeGenLLVMArrayLiteral(JITData* jd, struct ast_node* n)
{
    assert(n->tu->to.is_array);
    Code_gen_llvm::Array_literal array_literal;
    return array_literal.code_gen(jd, n);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMSubscript(JITData* jd, struct ast_node* n)
{
    Type* element_type = CodeGenLLVMGetType(jd, n->tu);

    struct ast_node* array = n->head;
    assert(array->tu->to.is_array);
    Value* array_value = CodeGenLLVMDispatch(jd, array);
    assert(array_value);

    struct ast_node* subscript = array->next;
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

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMSign(JITData* jd, struct ast_node* n)
{
    struct ast_node* op = ast_node_get(n, 0);
    struct ast_node* number = ast_node_get(n, 1);
    Value* number_value = CodeGenLLVMDispatch(jd, number);

    if (op->type == ast_type_plus) {
        return number_value;
    }

    assert(op->type == ast_type_minus);

    Type* number_type = CodeGenLLVMGetType(jd, number->tu);

    Value* zero_value;
    if (n->tu->td->type == type_integer) {
        zero_value = ConstantInt::get(number_type,
                                      APInt(number->tu->td->bit_count, 0, number->tu->td->is_signed));
    } else if (n->tu->td->type == type_float) {
        zero_value = ConstantFP::get(number_type, APFloat(0.0));
    } else {
        assert(false);
    }
    Value* value = jd->Builder->CreateSub(zero_value, number_value, "negatetmp");
    return value;
}