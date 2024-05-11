#include "../include/KaleidoscopeJIT.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "code_gen.h"
#include "code_gen_llvm2.h"
#include "akela/type_def.h"
#include "zinc/error.h"
#include "zinc/list.h"
#include "zinc/vector.h"
#include <cassert>
#include <iostream>
#include "parse_types.h"

#define TOPLEVEL_NAME "__toplevel"
#define MODULE_NAME "Akela JIT"

using namespace llvm;
using namespace llvm::orc;

typedef struct Code_gen_context{
    bool in_lhs;
} Code_gen_context;

typedef struct {
    struct error_list* el;
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<Module> TheModule;
    std::unique_ptr<IRBuilder<>> Builder;
    ExitOnError ExitOnErr;
    std::unique_ptr<KaleidoscopeJIT> TheJIT;
    Function* toplevel;
    Code_gen_context context;
} JITData;

CodeGenVTable CodeGenLLVM2VTable = {
        .jit_offset = offsetof(CodeGenLLVM2, jit),
};

Type* CodeGenLLVM2GetType(JITData* jd, struct ast_node* tu);
bool CodeGenLLVM2JIT(CodeGenLLVM2* cg, struct ast_node* n, CodeGenResult* result);
Value* CodeGenLLVM2Dispatch(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Stmts(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Extern(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2If(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Var(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Function(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Assign(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Add(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Sub(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Call(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2ID(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2ArrayLiteral(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Subscript(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Number(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Boolean(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2String(JITData* jd, struct ast_node* n);
Value* CodeGenLLVM2Sign(JITData* jd, struct ast_node* n);

void CodeGenLLVM2Init(CodeGenLLVM2* cg, struct error_list* el)
{
    cg->el = el;
    cg->jit = (CodeGenInterface)CodeGenLLVM2JIT;
    cg->debug = false;
}

void CodeGenLLVM2Create(CodeGenLLVM2** cg, struct error_list* el)
{
    *cg = new CodeGenLLVM2();
    CodeGenLLVM2Init(*cg, el);
}

void CodeGenLLVM2Destroy(CodeGenLLVM2* cg)
{
    delete cg;
}

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
FunctionType* CodeGenLLVM2FunctionType(JITData* jd, struct ast_node* tu)
{
    struct ast_node *input = nullptr;
    struct ast_node *output = nullptr;
    get_function_children(tu, &input, &output);

    std::vector<Type *> param_types = std::vector<Type *>();
    size_t input_count = ast_node_count_children(input);
    if (input_count > 0) {
        for (size_t i = 0; i < input_count; i++) {
            struct ast_node *dec = ast_node_get(input, i);
            Type *dec_type = CodeGenLLVM2GetType(jd, dec);
            param_types.push_back(dec_type);
        }
    }

    Type *ret_type = nullptr;
    if (output) {
        struct ast_node *ret = ast_node_get(output, 0);
        ret_type = CodeGenLLVM2GetType(jd, ret);
    } else {
        ret_type = Type::getVoidTy(*jd->TheContext);
    }

    return FunctionType::get(ret_type, param_types, false);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Type* CodeGenLLVM2GetTypeScalar(JITData* jd, struct ast_node* tu)
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
            t =  Type::getInt32Ty(*jd->TheContext);
        }
        return t;
    } else if (td->type == type_float) {
        if (td->bit_count == 64) {
            return Type::getDoubleTy(*jd->TheContext);
        } else if (td->bit_count == 32) {
            return Type::getFloatTy(*jd->TheContext);
        }
    } else if (td->type == type_string) {
        return Type::getInt8Ty(*jd->TheContext)->getPointerTo();
    } else if (td->type == type_boolean) {
        return Type::getInt1Ty(*jd->TheContext);
    } else if (td->type == type_function) {
        return CodeGenLLVM2FunctionType(jd, tu);
    } else {
        assert(false);
    }

    return nullptr;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Type* CodeGenLLVM2GetType(JITData* jd, struct ast_node* tu) {
    Type *t = CodeGenLLVM2GetTypeScalar(jd, tu);
    if (tu && tu->to.is_array) {
        size_t i = tu->to.dim.count - 1;
        while (true) {
            size_t len = *(size_t *) VECTOR_PTR(&tu->to.dim, i);
            t = ArrayType::get(t, len);
            if (i == 0) break;
            i--;
        }
    }

    return t;
}

Type* CodeGenLLVM2ReturnType(JITData* jd, struct ast_node* tu)
{
    if (tu && tu->td && tu->td->type == type_function) {
        FunctionType* func_type = CodeGenLLVM2FunctionType(jd, tu);
        return static_cast<Type*>(func_type->getPointerTo());
    } else {
        return CodeGenLLVM2GetType(jd, tu);
    }
}

void CodeGenLLVM2Run(JITData* jd, struct ast_node* n, struct buffer* bf)
{
    auto ExprSymbol = jd->ExitOnErr(jd->TheJIT->lookup(TOPLEVEL_NAME));
    if (n->tu) {
        enum type type = n->tu->td->type;
        bool is_array = n->tu->to.is_array;
        if (type == type_integer) {
            if (is_array) {
                long* (*fp)() = ExprSymbol.getAddress().toPtr<long*(*)()>();
                long* p = fp();
                Vector* dim_vector = &n->tu->to.dim;
                size_t count = 1;
                for (int i = 0; i < dim_vector->count; i++) {
                    size_t dim = *(size_t *) VECTOR_PTR(dim_vector, i);
                    count *= dim;
                }
                buffer_add_char(bf, '[');
                for (int i = 0; i < count; i++) {
                    if (i >= 1) {
                        buffer_add_char(bf, ',');
                    }
                    buffer_add_format(bf, "%ld", *p++);
                }
                buffer_add_char(bf, ']');
            } else {
                if (n->tu->td->bit_count == 64) {
                    long (*fp)() = ExprSymbol.getAddress().toPtr<long(*)()>();
                    long v = fp();
                    buffer_add_format(bf, "%d", v);
                } else if (n->tu->td->bit_count == 32) {
                    int (*fp)() = ExprSymbol.getAddress().toPtr<int(*)()>();
                    int v = fp();
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
        } else if (type == type_string) {
            char *(*fp)() = ExprSymbol.getAddress().toPtr<char *(*)()>();
            char *v = fp();
            buffer_add_format(bf, "\"%s\"", v);
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

BasicBlock* CodeGenLLVM2GetLastBlock(JITData* jd, Function* f)
{
    BasicBlock* last_block = nullptr;
    Function::iterator blocks = f->end();
    if (blocks != f->begin()) {
        last_block = &*--blocks;
    }
    return last_block;
}

bool CodeGenLLVM2JIT(CodeGenLLVM2* cg, struct ast_node* n, CodeGenResult* result)
{
    bool valid = true;

    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    JITData jd;
    JITDataInit(&jd, cg->el);

    std::vector<Type*> param_types = std::vector<Type*>();
    Type* ret_type = CodeGenLLVM2ReturnType(&jd, n->tu);
    FunctionType *func_type = FunctionType::get(ret_type, param_types, false);
    Function *f = Function::Create(func_type, Function::ExternalLinkage, TOPLEVEL_NAME, *jd.TheModule);
    jd.toplevel = f;

    std::string error_str;
    raw_string_ostream error_os(error_str);
    if (verifyFunction(*f, &error_os)) {
        struct location loc = {};
        location_init(&loc);
        error_list_set(cg->el, &loc, "%s", error_str.c_str());
    }

    BasicBlock* entry = BasicBlock::Create(*jd.TheContext, "entry", f);
    jd.Builder->SetInsertPoint(entry);
    Value* value = CodeGenLLVM2Dispatch(&jd, n);

    BasicBlock* last_block = nullptr;
    Function::iterator blocks = jd.toplevel->end();
    if (blocks != jd.toplevel->begin()) {
        last_block = &*--blocks;
    }
    jd.Builder->SetInsertPoint(last_block);

    if (n->tu) {
        jd.Builder->CreateRet(value);
    } else {
        jd.Builder->CreateRetVoid();
    }

    if (cg->el->head) {
        valid = false;
    }

    std::string str;
    raw_string_ostream os(str);
    if  (verifyModule(*jd.TheModule, &os)) {
        struct location loc = {};
        location_init(&loc);
        error_list_set(cg->el, &loc, "%s", str.c_str());
        valid = false;
    }

    std::string str2;
    raw_string_ostream os2(str2);
    jd.TheModule->print(os2, nullptr);
    buffer_add_format(&result->text, "%s", str2.c_str());

    if (cg->debug) {
        printf("%s\n\n", result->text.buf);
    }

    if (valid) {
        auto rt = jd.TheJIT->getMainJITDylib().createResourceTracker();
        auto tsm = ThreadSafeModule(std::move(jd.TheModule), std::move(jd.TheContext));
        jd.ExitOnErr(jd.TheJIT->addModule(std::move(tsm), rt));
        CodeGenLLVM2Run(&jd, n, &result->value);
        jd.ExitOnErr(rt->remove());
    }

    buffer_finish(&result->value);
    buffer_finish(&result->text);

    return true;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2Dispatch(JITData* jd, struct ast_node* n)
{
    if (n->type == ast_type_stmts) {
        return CodeGenLLVM2Stmts(jd, n);
    } else if (n->type == ast_type_extern) {
        return CodeGenLLVM2Extern(jd, n);
    } else if (n->type == ast_type_if) {
        return CodeGenLLVM2If(jd, n);
    } else if (n->type == ast_type_let) {
        return CodeGenLLVM2Var(jd, n);
    } else if (n->type == ast_type_function) {
        return CodeGenLLVM2Function(jd, n);
    } else if (n->type == ast_type_anonymous_function) {
        return CodeGenLLVM2Function(jd, n);
    } else if (n->type == ast_type_assign) {
        return CodeGenLLVM2Assign(jd, n);
    } else if (n->type == ast_type_plus) {
        return CodeGenLLVM2Add(jd, n);
    } else if (n->type == ast_type_minus) {
        return CodeGenLLVM2Sub(jd, n);
    } else if (n->type == ast_type_call) {
        return CodeGenLLVM2Call(jd, n);
    } else if (n->type == ast_type_id) {
        return CodeGenLLVM2ID(jd, n);
    } else if (n->type == ast_type_array_literal) {
        return CodeGenLLVM2ArrayLiteral(jd, n);
    } else if (n->type == ast_type_array_subscript) {
        return CodeGenLLVM2Subscript(jd, n);
    } else if (n->type == ast_type_sign) {
        return CodeGenLLVM2Sign(jd, n);
    } else if (n->type == ast_type_number) {
        return CodeGenLLVM2Number(jd, n);
    } else if (n->type == ast_type_boolean) {
        return CodeGenLLVM2Boolean(jd, n);
    } else if (n->type == ast_type_string) {
        return CodeGenLLVM2String(jd, n);
    } else {
        char* names[ast_type_count];
        ast_set_names(names);
        printf("code gen: unhandled ast node type: %s", names[n->type]);
        exit(1);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2Stmts(JITData* jd, struct ast_node* n)
{
    Value* last_v = nullptr;
    struct ast_node* last_n = nullptr;
    struct ast_node* stmt = ast_node_get(n, 0);
    while (stmt) {
        last_v = CodeGenLLVM2Dispatch(jd, stmt);
        last_n = stmt;
        stmt = stmt->next;
    }
    return last_v;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2If(JITData* jd, struct ast_node* n)
{
    bool has_else = false;

    Type* type = nullptr;
    Value* ptr = nullptr;
    if (n->tu) {
        type = CodeGenLLVM2GetType(jd, n->tu);
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
            Value* cond_value = CodeGenLLVM2Dispatch(jd, cond);
            Value* branch_value = jd->Builder->CreateCondBr(cond_value, then_block, next_block);

            jd->Builder->SetInsertPoint(then_block);
            Value* body_value = CodeGenLLVM2Stmts(jd, body);
            if (type) {
                jd->Builder->CreateStore(body_value, ptr);
            }
            list_add_item(&l, then_block);      /* branch to end after end_block is created */

            jd->Builder->SetInsertPoint(next_block);

        } else if (branch->type == ast_type_default_branch) {
            struct ast_node* body = ast_node_get(branch, 0);

            Value* body_value = CodeGenLLVM2Stmts(jd, body);
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
Value* CodeGenLLVM2Var(JITData* jd, struct ast_node* n)
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
                FunctionType *func_type = CodeGenLLVM2FunctionType(jd, tu);
                PointerType *pt = func_type->getPointerTo();
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(pt, nullptr, lhs->value.buf);
                lhs->sym->reference = lhs_value;
                Value* rhs_value = CodeGenLLVM2Dispatch(jd, rhs);
                jd->Builder->CreateStore(rhs_value, lhs_value);
            }
        } else if (tu->to.is_array) {
            if (rhs) {
                Type* t = CodeGenLLVM2GetType(jd, tu);
                t = t->getPointerTo();
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                Value *rhs_value = CodeGenLLVM2Dispatch(jd, rhs);
                lhs->sym->reference = lhs_value;
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                Type* t = CodeGenLLVM2GetType(jd, tu);
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                lhs->sym->value = lhs_value;
            }
        } else {
            if (rhs) {
                Type* t = CodeGenLLVM2GetType(jd, tu);
                buffer_finish(&lhs->value);
                AllocaInst* lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                lhs->sym->reference = lhs_value;
                Value *rhs_value = CodeGenLLVM2Dispatch(jd, rhs);
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
Value* CodeGenLLVM2Assign(JITData* jd, struct ast_node* n)
{
    struct ast_node* rhs = n->tail;
    Value* rhs_value = CodeGenLLVM2Dispatch(jd, rhs);
    struct ast_node* lhs = rhs->prev;
    while (lhs) {
        if (lhs->tu->td->type == type_function) {
            if (lhs->type == ast_type_id) {
                AllocaInst* lhs_value;
                if (lhs->sym->reference) {
                    lhs_value = (AllocaInst*)lhs->sym->reference;
                } else {
                    lhs->sym->value = nullptr;
                    FunctionType *func_type = CodeGenLLVM2FunctionType(jd, rhs->tu);
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
                AllocaInst* lhs_value;
                if (lhs->sym->reference) {
                    lhs_value = (AllocaInst*)lhs->sym->reference;
                } else {
                    lhs->sym->value = nullptr;
                    Type* t = CodeGenLLVM2GetType(jd, lhs->tu);
                    t = t->getPointerTo();
                    buffer_finish(&lhs->value);
                    lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    lhs->sym->reference = lhs_value;
                }
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                assert(false);
            }
        } else {
            if (lhs->type == ast_type_id) {
                AllocaInst* lhs_value;
                if (lhs->sym->reference) {
                    lhs_value = (AllocaInst*)lhs->sym->reference;
                } else {
                    Type* t = CodeGenLLVM2GetType(jd, lhs->tu);
                    buffer_finish(&lhs->value);
                    lhs_value = jd->Builder->CreateAlloca(t, nullptr, lhs->value.buf);
                    lhs->sym->reference = lhs_value;
                }
                jd->Builder->CreateStore(rhs_value, lhs_value);
            } else {
                jd->context.in_lhs = true;
                Value* lhs_value = CodeGenLLVM2Dispatch(jd, lhs);
                jd->context.in_lhs = false;
                jd->Builder->CreateStore(rhs_value, lhs_value);
            }
        }
        lhs = lhs->prev;
    }
    return rhs_value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2Function(JITData* jd, struct ast_node* n)
{
    FunctionType* func_type = CodeGenLLVM2FunctionType(jd, n->tu);
    struct ast_node *proto = ast_node_get(n, 0);
    struct ast_node *id = ast_node_get(proto, 0);
    buffer_finish(&id->value);
    Function* f = Function::Create(func_type,
                                   GlobalValue::ExternalLinkage,
                                   id->value.buf,
                                   *jd->TheModule);
    BasicBlock* body_block = BasicBlock::Create(*jd->TheContext, "body", f);
    jd->Builder->SetInsertPoint(body_block);

    struct ast_node* dseq = ast_node_get(proto, 1);
    struct ast_node* dec = dseq->head;
    int i = 0;
    while (dec) {
        struct ast_node* dec_id = ast_node_get(dec, 0);
        struct ast_node* dec_type = ast_node_get(dec, 1);
        buffer_finish(&dec_id->value);
        Type* t = CodeGenLLVM2GetType(jd, dec_type);
        Value* lhs = jd->Builder->CreateAlloca(t,
                                           nullptr,
                                           dec_id->value.buf);
        jd->Builder->CreateStore(&f->arg_begin()[i], lhs);
        dec_id->sym->reference = lhs;
        dec = dec->next;
        i++;
    }

    struct ast_node* body = ast_node_get(n, 1);
    Value* ret_value = CodeGenLLVM2Dispatch(jd, body);
    if (body->tu) {
        jd->Builder->CreateRet(ret_value);
    } else {
        jd->Builder->CreateRetVoid();
    }

    BasicBlock* last_block = CodeGenLLVM2GetLastBlock(jd, jd->toplevel);
    jd->Builder->SetInsertPoint(last_block);

    n->sym->value = f;

    return f;
}

Value* CodeGenLLVM2Extern(JITData* jd, struct ast_node* n)
{
    FunctionType* func_type = CodeGenLLVM2FunctionType(jd, n->tu);
    struct ast_node *proto = ast_node_get(n, 0);
    struct ast_node *id = ast_node_get(proto, 0);
    buffer_finish(&id->value);
    Function* f = Function::Create(func_type, GlobalValue::ExternalLinkage, id->value.buf, *jd->TheModule);

    BasicBlock* last_block = CodeGenLLVM2GetLastBlock(jd, jd->toplevel);
    jd->Builder->SetInsertPoint(last_block);

    n->sym->value = f;

    return f;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2Add(JITData* jd, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    Value* lhs = CodeGenLLVM2Dispatch(jd, a);

    struct ast_node* b = ast_node_get(n, 1);
    Value* rhs = CodeGenLLVM2Dispatch(jd, b);

    return jd->Builder->CreateAdd(lhs, rhs, "addtmp");
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2Sub(JITData* jd, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    Value* lhs = CodeGenLLVM2Dispatch(jd, a);

    struct ast_node* b = ast_node_get(n, 1);
    Value* rhs = CodeGenLLVM2Dispatch(jd, b);

    return jd->Builder->CreateSub(lhs, rhs, "subtmp");
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2Call(JITData* jd, struct ast_node* n)
{
    struct ast_node* callee = ast_node_get(n, 0);
    struct ast_node* cseq = ast_node_get(n, 1);

    assert(callee && callee->tu && callee->tu->td && callee->tu->td->type == type_function);
    Value* callee_value = CodeGenLLVM2Dispatch(jd, callee);

    std::vector<Value*> arg_list;
    struct ast_node* arg = cseq->head;
    while (arg) {
        Value* value = CodeGenLLVM2Dispatch(jd, arg);
        arg_list.push_back(value);
        arg = arg->next;
    }

    FunctionType *t = CodeGenLLVM2FunctionType(jd, callee->tu);
    return jd->Builder->CreateCall(t, callee_value, arg_list);
}

Value* CodeGenLLVM2ID(JITData* jd, struct ast_node* n)
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
            Type *t = CodeGenLLVM2GetType(jd, n->tu);
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
                    Type* t = CodeGenLLVM2GetType(jd, p->tu);
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
                Value* value = CodeGenLLVM2Dispatch(jd, n);
                jd->Builder->CreateStore(value, ptr);
            }
        }
    };
};

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2ArrayLiteral(JITData* jd, struct ast_node* n)
{
    assert(n->tu->to.is_array);
    Code_gen_llvm::Array_literal array_literal;
    return array_literal.code_gen(jd, n);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVM2Subscript(JITData* jd, struct ast_node* n)
{
    Type* element_type = CodeGenLLVM2GetType(jd, n->tu);

    struct ast_node* array = n->head;
    assert(array->tu->to.is_array);
    Value* array_value = CodeGenLLVM2Dispatch(jd, array);
    assert(array_value);

    struct ast_node* subscript = array->next;
    Value* subscript_value = CodeGenLLVM2Dispatch(jd, subscript);
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
Value* CodeGenLLVM2Sign(JITData* jd, struct ast_node* n)
{
    struct ast_node* op = ast_node_get(n, 0);
    struct ast_node* number = ast_node_get(n, 1);
    Value* number_value = CodeGenLLVM2Dispatch(jd, number);

    if (op->type == ast_type_plus) {
        return number_value;
    }

    assert(op->type == ast_type_minus);

    Type* number_type = CodeGenLLVM2GetType(jd, number->tu);

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

Value* CodeGenLLVM2Number(JITData* jd, struct ast_node* n)
{
    struct ast_node* tu = n->tu;
    struct type_def *td = tu->td;
    if (td->type == type_integer) {
        Type* t = CodeGenLLVM2GetType(jd, n->tu);
        buffer_finish(&n->value);
        long v = strtol(n->value.buf, nullptr, 10);
        return ConstantInt::get(t, APInt(td->bit_count, v, n->tu->td->is_signed));
    } else if (td->type == type_float) {
        buffer_finish(&n->value);
        double v = strtod(n->value.buf, nullptr);
        return ConstantFP::get(*jd->TheContext, APFloat(v));
    }
    assert(false);
}

Value* CodeGenLLVM2Boolean(JITData* jd, struct ast_node* n)
{
    if (buffer_compare_str(&n->value, "true")) {
        Type* t = Type::getInt1Ty(*jd->TheContext);
        return ConstantInt::get(t, APInt(1, 1, true));
    } else if (buffer_compare_str(&n->value, "false")) {
        Type* t = Type::getInt1Ty(*jd->TheContext);
        return ConstantInt::get(t, APInt(1, 0, true));
    }
    assert(false && "invalid boolean identifier");
}

Value* CodeGenLLVM2String(JITData* jd, struct ast_node* n)
{
    buffer_finish(&n->value);
    return jd->Builder->CreateGlobalStringPtr(n->value.buf, ".str");
}