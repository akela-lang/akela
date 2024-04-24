#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#include "ast.h"
#include "code_gen.h"
#include "code_gen_llvm.h"
#include <stddef.h>
#include "type_def.h"
#include <assert.h>
#include <string.h>
#include "zinc/memory.h"
#include "zinc/list.h"

bool CodeGenLLVMJIT(CodeGenLLVM* cg, struct ast_node* n, struct buffer* bf);
LLVMValueRef CodeGenLLVMDispatch(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMStmts(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMIf(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMAssign(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMVar(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMAdd(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMSub(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMID(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMNumber(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMString(CodeGenLLVM* cg, struct ast_node* n);
LLVMValueRef CodeGenLLVMBoolean(CodeGenLLVM* cg, struct ast_node* n);;

CodeGenVTable CodeGenLLVMVTable = {
        .jit_offset = offsetof(CodeGenLLVM, jit),
};

void CodeGenLLVMInit(CodeGenLLVM* cg, struct error_list* el, struct symbol_table* st)
{
    cg->el = el;
    cg->st = st;
    cg->builder = NULL;
    cg->context = NULL;
    cg->main = NULL;
    cg->jit = (CodeGenInterface)CodeGenLLVMJIT;
}

void CodeGenLLVMCreate(CodeGenLLVM** cg, struct error_list* el, struct symbol_table* st)
{
    malloc_safe((void**)cg, sizeof(CodeGenLLVM));
    CodeGenLLVMInit(*cg, el, st);
}

LLVMTypeRef get_llvm_type(struct ast_node* n, struct ast_node* tu)
{
    if (n) {
        tu = n->tu;
    }
    if (tu) {
        if (tu->type == ast_type_type) {
            struct type_def* td = tu->td;
            if (td) {
                if (td->type == type_integer) {
                    if (td->bit_count == 32) {
                        return LLVMInt32Type();
                    } else if (td->bit_count == 64) {
                        return LLVMInt64Type();
                    }
                } else if (td->type == type_float) {
                    if (td->bit_count == 32) {
                        return LLVMFloatType();
                    } else if (td->bit_count == 64) {
                        return LLVMDoubleType();
                    }
                } else if (td->type == type_string) {
                    if (n && n->type == ast_type_string) {
                        buffer_finish(&n->value);
                        unsigned int len = n->value.size + 1;
                        return LLVMPointerType(LLVMInt8Type(), 0);
                    } else {
                        return LLVMPointerType(LLVMInt8Type(), 0);
                    }
                } else if (td->type == type_boolean) {
                    return LLVMInt1Type();
                }
            }
        }
    }
    return LLVMVoidType();
}

enum result serialize(LLVMGenericValueRef v, struct ast_node* n, struct buffer* bf)
{
    enum result r = result_ok;
    LLVMTypeRef t = get_llvm_type(n, NULL);
    bool found = false;

    struct ast_node* tu = n->tu;
    if (tu) {
        if (tu->type == ast_type_type) {
            struct type_def* td = tu->td;
            if (td) {
                if (td->type == type_integer) {
                    if (td->bit_count == 32) {
                        buffer_add_format(bf, "%ld", LLVMGenericValueToInt(v, true));
                        found = true;
                    } else if (td->bit_count == 64) {
                        buffer_add_format(bf, "%ld", LLVMGenericValueToInt(v, true));
                        found = true;
                    }
                } else if (td->type == type_float) {
                    if (td->bit_count == 32) {
                        buffer_add_format(bf, "%f", LLVMGenericValueToFloat(t, v));
                        found = true;
                    } else if (td->bit_count == 64) {
                        buffer_add_format(bf, "%lf", LLVMGenericValueToFloat(t, v));
                        found = true;
                    }

                } else if (td->type == type_string) {
                    buffer_add_format(bf, "%s", LLVMGenericValueToPointer(v));
                    found = true;
                } else if (td->type == type_boolean) {
                    unsigned long long v2 = LLVMGenericValueToInt(v, false);
                    if (v2 == 1) {
                        buffer_add_format(bf, "%s", "true");
                        found = true;
                    } else if (v2 == 0) {
                        buffer_add_format(bf, "%s", "false");
                        found = true;
                    }
                }
            }
        }
    } else {
        /* void */
        found = true;
    }

    if (!found) {
        r = set_error("unknown type");
    }

    return r;
}

bool CodeGenLLVMJIT(CodeGenLLVM* cg, struct ast_node* n, struct buffer* bf)
{
    bool valid = true;

    LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");
    LLVMContextRef context = LLVMContextCreate();
    cg->context = context;

    LLVMTypeRef ret_type = get_llvm_type(n, NULL);
    LLVMTypeRef fun_type = LLVMFunctionType(ret_type, NULL, 0, 0);
    LLVMValueRef _main = LLVMAddFunction(mod, "toplevel", fun_type);
    cg->main = _main;

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(_main, "entry");

    LLVMBuilderRef builder = LLVMCreateBuilder();
    cg->builder = builder;
    LLVMPositionBuilderAtEnd(builder, entry);

    LLVMValueRef tmp = CodeGenLLVMDispatch(cg, n);
    if (n->tu) {
        LLVMBuildRet(builder, tmp);
    } else {
        LLVMBuildRet(builder, NULL);
    }

    char *error = NULL;
    LLVMBool broken = LLVMVerifyModule(mod, LLVMReturnStatusAction, &error);
    if (broken || (error && strlen(error) > 0)) {
        char* mod_str = LLVMPrintModuleToString(mod);
        struct location loc;
        location_init(&loc);
        error_list_set(cg->el, &loc, "%s", error);
        error_list_set(cg->el, &loc, "module:\n%s", mod_str);
        valid = false;
        LLVMDisposeMessage(error);

        error = NULL;
        if (LLVMPrintModuleToFile(mod, "toplevel.ll", &error)) {
            error_list_set(cg->el, &loc, "%s", error);
            LLVMDisposeMessage(error);
        }

        return valid;
    }
    LLVMDisposeMessage(error);

    LLVMExecutionEngineRef engine;
    error = NULL;
    LLVMLinkInMCJIT();
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    if (LLVMCreateExecutionEngineForModule(&engine, mod, &error) != 0) {
        fprintf(stderr, "failed to create execution engine\n");
        abort();
    }
    if (error) {
        fprintf(stderr, "error: %s\n", error);
        LLVMDisposeMessage(error);
        exit(EXIT_FAILURE);
    }

    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(mod, "toplevel.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }

    LLVMGenericValueRef v = LLVMRunFunction(engine, _main, 0, NULL);
    enum result r = serialize(v, n, bf);
    LLVMDisposeGenericValue(v);
    if (r == result_error) {
        struct location loc;
        location_init(&loc);
        valid = error_list_set(cg->el, &loc, "%s", error_message);
    }

    LLVMContextDispose(context);
    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);

    return valid;
}

LLVMValueRef CodeGenLLVMDispatch(CodeGenLLVM* cg, struct ast_node* n)
{
    if (n->type == ast_type_stmts) {
        return CodeGenLLVMStmts(cg, n);
    } else if (n->type == ast_type_if) {
        return CodeGenLLVMIf(cg, n);
    } else if (n->type == ast_type_assign) {
        return CodeGenLLVMAssign(cg, n);
    } else if (n->type == ast_type_var) {
        return CodeGenLLVMVar(cg, n);
    } else if (n->type == ast_type_plus) {
        return CodeGenLLVMAdd(cg, n);
    } else if (n->type == ast_type_minus) {
        return CodeGenLLVMSub(cg, n);
    } else if (n->type == ast_type_id) {
        return CodeGenLLVMID(cg, n);
    } else if (n->type == ast_type_number) {
        return CodeGenLLVMNumber(cg, n);
    } else if (n->type == ast_type_string) {
        return CodeGenLLVMString(cg, n);
    } else if (n->type == ast_type_boolean) {
        return CodeGenLLVMBoolean(cg, n);
    } else {
        char* names[ast_type_count];
        ast_set_names(names);
        fprintf(stderr, "code gen: unknown expression: %d\n", n->type);
        ast_node_print(n, names, false);
        exit(1);
    }
}

LLVMValueRef CodeGenLLVMStmts(CodeGenLLVM* cg, struct ast_node* n)
{
    LLVMValueRef last_v = NULL;
    struct ast_node* last_n = NULL;
    struct ast_node* stmt = ast_node_get(n, 0);
    while (stmt) {
        last_v = CodeGenLLVMDispatch(cg, stmt);
        last_n = stmt;
        stmt = stmt->next;
    }
    return last_v;
}

LLVMValueRef CodeGenLLVMVar(CodeGenLLVM* cg, struct ast_node* n)
{
    struct ast_node* lseq = ast_node_get(n, 0);
    struct ast_node* tu = ast_node_get(n, 1);
    struct ast_node* rseq = ast_node_get(n, 2);

    struct ast_node *lp = ast_node_get(lseq, 0);
    struct ast_node *rp = NULL;
    if (rseq) {
        rp = ast_node_get(rseq, 0);
    }
    while (lp) {
        LLVMTypeRef t = get_llvm_type(NULL, tu);
        buffer_finish(&lp->value);
        LLVMValueRef lhs =  LLVMBuildAlloca(cg->builder, t, lp->value.buf);
        assert(lp->sym);
        lp->sym->allocation = lhs;
        if (rp) {
            LLVMValueRef rhs = CodeGenLLVMDispatch(cg, rp);
            LLVMBuildStore(cg->builder, rhs, lhs);
            rp = rp->next;
        }
        lp = lp->next;
    }

    return NULL;
}

LLVMValueRef CodeGenLLVMAssign(CodeGenLLVM* cg, struct ast_node* n)
{
    struct ast_node* p = n->tail;
    LLVMValueRef rhs = CodeGenLLVMDispatch(cg, p);
    p = p->prev;
    LLVMValueRef lhs;
    while (p) {
        lhs = p->sym->allocation;
        LLVMBuildStore(cg->builder, rhs, lhs);
        p = p->prev;
    }
    return rhs;
}

LLVMValueRef CodeGenLLVMIf(CodeGenLLVM* cg, struct ast_node* n)
{
    bool has_else = false;

    LLVMTypeRef type = NULL;
    LLVMValueRef ptr = NULL;
    if (n->tu) {
        type = get_llvm_type(n, n->tu);
    }
    if (type) {
        ptr = LLVMBuildAlloca(cg->builder, type, "ifresult");
    }

    struct list l;
    list_init(&l);

    LLVMBasicBlockRef cond_block = NULL;
    LLVMBasicBlockRef then_block = NULL;
    LLVMBasicBlockRef next_block = NULL;
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
                    cond_block = LLVMAppendBasicBlockInContext(cg->context, cg->main, "condtmp");
                }
            }
            then_block = LLVMAppendBasicBlockInContext(cg->context, cg->main, "thentmp");
            next_block = LLVMAppendBasicBlockInContext(cg->context, cg->main, "nexttmp");

            if (cond_block) {
                LLVMPositionBuilderAtEnd(cg->builder, cond_block);
            }
            LLVMValueRef cond_value = CodeGenLLVMDispatch(cg, cond);
            LLVMValueRef branch_value = LLVMBuildCondBr(cg->builder, cond_value, then_block, next_block);

            LLVMPositionBuilderAtEnd(cg->builder, then_block);
            LLVMValueRef body_value = CodeGenLLVMStmts(cg, body);
            if (type) {
                LLVMBuildStore(cg->builder, body_value, ptr);
            }
            list_add_item(&l, then_block);      /* branch to end after end_block is created */

            LLVMPositionBuilderAtEnd(cg->builder, next_block);

        } else if (branch->type == ast_type_default_branch) {
            struct ast_node* body = ast_node_get(branch, 0);

            LLVMValueRef body_value = CodeGenLLVMStmts(cg, body);
            if (type) {
                LLVMBuildStore(cg->builder, body_value, ptr);
            }
            list_add_item(&l, next_block);      /* branch to end after end_block is created */
            has_else = true;

        } else {
            assert(false);
        }

        i++;
    }

    LLVMBasicBlockRef end_block = NULL;
    if (has_else) {
        end_block = LLVMAppendBasicBlockInContext(cg->context, cg->main, "endiftmp");
    } else {
        end_block = next_block;
    }
    struct list_node* ln = l.head;
    while (ln) {
        LLVMBasicBlockRef p = ln->item;
        LLVMPositionBuilderAtEnd(cg->builder, p);
        LLVMBuildBr(cg->builder, end_block);
        ln = ln->next;
    }

    LLVMPositionBuilderAtEnd(cg->builder, end_block);
    LLVMValueRef value = NULL;
    if (type) {
        value = LLVMBuildLoad2(cg->builder, type, ptr, "branchresulttmp");
    }

    list_destroy(&l, NULL);

    return value;
}

LLVMValueRef CodeGenLLVMAdd(CodeGenLLVM* cg, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    LLVMValueRef lhs = CodeGenLLVMDispatch(cg, a);

    struct ast_node* b = ast_node_get(n, 1);
    LLVMValueRef rhs = CodeGenLLVMDispatch(cg, b);

    return LLVMBuildAdd(cg->builder, lhs, rhs, "addtmp");
}

LLVMValueRef CodeGenLLVMSub(CodeGenLLVM* cg, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    LLVMValueRef lhs = CodeGenLLVMDispatch(cg, a);

    struct ast_node* b = ast_node_get(n, 1);
    LLVMValueRef rhs = CodeGenLLVMDispatch(cg, b);

    return LLVMBuildSub(cg->builder, lhs, rhs, "subtmp");
}

LLVMValueRef CodeGenLLVMID(CodeGenLLVM* cg, struct ast_node* n)
{
    buffer_finish(&n->value);
    struct symbol* sym = n->sym;
    assert(sym);
    LLVMValueRef v = sym->allocation;
    LLVMTypeRef t = LLVMGetAllocatedType(v);
    return LLVMBuildLoad2(cg->builder, t, v, n->value.buf);
}

LLVMValueRef CodeGenLLVMNumber(CodeGenLLVM* cg, struct ast_node* n)
{
    struct ast_node* tu = n->tu;
    struct type_def *td = tu->td;
    if (td->type == type_integer) {
        long v;
        if (td->bit_count == 32) {
            LLVMTypeRef t = LLVMInt32Type();
            buffer_finish(&n->value);
            v = strtol(n->value.buf, NULL, 10);
            return LLVMConstInt(t, v, false);
        } else if (td->bit_count == 64) {
            LLVMTypeRef t = LLVMInt64Type();
            buffer_finish(&n->value);
            v = strtoll(n->value.buf, NULL, 10);
            return LLVMConstInt(t, v, false);
        }
    } else if (td->type == type_float) {
        double v;
        if (td->bit_count == 32) {
            LLVMTypeRef t = LLVMDoubleType();
            buffer_finish(&n->value);
            v = strtod(n->value.buf, NULL);
            return LLVMConstReal(t, v);
        } else if (td->bit_count == 64) {
            LLVMTypeRef t = LLVMDoubleType();
            buffer_finish(&n->value);
            v = strtod(n->value.buf, NULL);
            return LLVMConstReal(t, v);
        }
    }
    assert(false);
}

LLVMValueRef CodeGenLLVMString(CodeGenLLVM* cg, struct ast_node* n)
{
    buffer_finish(&n->value);
    return LLVMBuildGlobalStringPtr(cg->builder, n->value.buf, ".str");
}

LLVMValueRef CodeGenLLVMBoolean(CodeGenLLVM* cg, struct ast_node* n)
{
    if (buffer_compare_str(&n->value, "true")) {
        return LLVMConstInt(LLVMInt1Type(), 1, false);
    } else if (buffer_compare_str(&n->value, "false")) {
        return LLVMConstInt(LLVMInt1Type(), 0, false);
    }
    assert(false);
}