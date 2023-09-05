#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#include <stdio.h>
#include <stdlib.h>
#include "comp_unit.h"
#include "type_def.h"
#include "code_gen_tools.h"
#include "zinc/hash.h"
#include "zinc/memory.h"

LLVMValueRef cg_expr(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_stmts(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_var(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_add(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_sub(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_id(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_number(struct cg_data* cgd, struct ast_node* n);

LLVMTypeRef get_llvm_type(struct ast_node* tu)
{
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
                }
            }
        }
    }
    return LLVMVoidType();
}

LLVMValueRef get_llvm_constant(struct ast_node* n)
{
    struct ast_node* tu = n->tu;
    if (tu) {
        if (tu->type == ast_type_type) {
            struct type_def* td = tu->td;
            if (td) {
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
                }
            }
        }
    }

    fprintf(stderr, "Constant not implemented");
    exit(1);
}

void code_gen(struct comp_unit* cu)
{
    LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");
    LLVMContextRef context = LLVMContextCreate();

    LLVMTypeRef param_types[] = { LLVMInt32Type(), LLVMInt32Type() };
    LLVMTypeRef ret_type = get_llvm_type(cu->root->tu);
    LLVMTypeRef fun_type = LLVMFunctionType(ret_type, param_types, 2, 0);
    LLVMValueRef sum = LLVMAddFunction(mod, "sum", fun_type);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(sum, "entry");

    LLVMBuilderRef builder = LLVMCreateBuilder();
    LLVMPositionBuilderAtEnd(builder, entry);

    struct cg_data cgd;
    cg_data_init(&cgd, builder, context);
    LLVMValueRef tmp = cg_expr(&cgd, cu->root);
    LLVMBuildRet(builder, tmp);

    char *error = NULL;
    LLVMVerifyModule(mod, LLVMReturnStatusAction, &error);
    if (error) {
        fprintf(stderr, "%s\n", error);
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

    int x = 1;
    int y = 2;

    int (*sum_func)(int, int) = (int (*)(int, int))LLVMGetFunctionAddress(engine, "sum");
    printf("%d\n", sum_func(x, y));

    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(mod, "sum.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }

    error = NULL;
    if (LLVMPrintModuleToFile(mod, "sum.ll", &error)) {
        fprintf(stderr, "%s\n", error);
        LLVMDisposeMessage(error);
    }

    LLVMContextDispose(context);
    LLVMDisposeBuilder(builder);
    LLVMDisposeExecutionEngine(engine);
    cg_data_destroy(&cgd);
}

LLVMValueRef cg_expr(struct cg_data* cgd, struct ast_node* n)
{
    if (n->type == ast_type_stmts) {
        return cg_stmts(cgd, n);
    } else if (n->type == ast_type_var) {
        return cg_var(cgd, n);
    } else if (n->type == ast_type_plus) {
        return cg_add(cgd, n);
    } else if (n->type == ast_type_minus) {
        return cg_sub(cgd, n);
    } else if (n->type == ast_type_id) {
        return cg_id(cgd, n);
    } else if (n->type == ast_type_number) {
        return cg_number(cgd, n);
    } else {
        fprintf(stderr, "unknown expression: %d\n", n->type);
        exit(1);
    }
    return NULL;
}

LLVMValueRef cg_stmts(struct cg_data* cgd, struct ast_node* n)
{
    LLVMValueRef last = NULL;
    struct ast_node* stmt = ast_node_get(n, 0);
    while (stmt) {
        last = cg_expr(cgd, stmt);
        stmt = stmt->next;
    }
    return last;
}

LLVMValueRef cg_var(struct cg_data* cgd, struct ast_node* n)
{
    struct ast_node* lseq = ast_node_get(n, 0);
    struct ast_node* tu = ast_node_get(n, 1);
    struct ast_node* rseq = ast_node_get(n, 2);

    struct ast_node *lval = ast_node_get(lseq, 0);
    struct ast_node *rval = ast_node_get(rseq, 0);
    while (lval) {
        if (rval) {
            LLVMValueRef rhs = cg_expr(cgd, rval);

            LLVMTypeRef t = get_llvm_type(tu);
            struct buffer* bf = &lval->value;
            buffer_finish(bf);
            LLVMValueRef lhs =  LLVMBuildAlloca(cgd->builder, t, bf->buf);
            LLVMBuildStore(cgd->builder, rhs, lhs);
            hash_table_add(&cgd->named_values, bf, lhs);
            rval = rval->next;
        }
        lval = lval->next;
    }

    return NULL;
}

LLVMValueRef cg_add(struct cg_data* cgd, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    LLVMValueRef lhs = cg_expr(cgd, a);

    struct ast_node* b = ast_node_get(n, 1);
    LLVMValueRef rhs = cg_expr(cgd, b);

    return LLVMBuildAdd(cgd->builder, lhs, rhs, "addtmp");
}

LLVMValueRef cg_sub(struct cg_data* cgd, struct ast_node* n)
{
    struct ast_node* a = ast_node_get(n, 0);
    LLVMValueRef lhs = cg_expr(cgd, a);

    struct ast_node* b = ast_node_get(n, 1);
    LLVMValueRef rhs = cg_expr(cgd, b);

    return LLVMBuildSub(cgd->builder, lhs, rhs, "subtmp");
}

LLVMValueRef cg_id(struct cg_data* cgd, struct ast_node* n)
{
    struct buffer* bf = &n->value;
    buffer_finish(bf);
    LLVMValueRef v = hash_table_get(&cgd->named_values, bf);
    LLVMTypeRef t = LLVMGetAllocatedType(v);
    return LLVMBuildLoad2(cgd->builder, t, v, bf->buf);
}

LLVMValueRef cg_number(struct cg_data* cgd, struct ast_node* n)
{
    return get_llvm_constant(n);
}
