#include "akela/parse_types.h"
#include "akela_llvm_tools.h"
#include "akela_llvm_function.h"
#include "akela_llvm_literal.h"
#include "akela_llvm_variable.h"

using namespace llvm;
using namespace llvm::orc;

Value* CodeGenLLVMStmts(JITData* jd, struct ast_node* n);
Value* CodeGenLLVMIf(JITData* jd, struct ast_node* n);
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