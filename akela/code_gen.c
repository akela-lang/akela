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
#include "zinc/result.h"

LLVMValueRef cg_expr(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_stmts(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_var(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_add(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_sub(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_id(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_number(struct cg_data* cgd, struct ast_node* n);
LLVMValueRef cg_string(struct cg_data* cgd, struct ast_node* n);

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
                        return LLVMArrayType(LLVMInt8Type(), len);
                    } else {
                        //return LLVMArrayType(LLVMInt8Type(), 7);
                        return LLVMPointerType(LLVMInt8Type(), 0);
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
                        return LLVMConstReal(LLVMDoubleType(), v);
                    }

                } else if (td->type == type_string) {
                    return LLVMConstString(n->value.buf, n->value.size, false);
                }
            }
        }
    }

    fprintf(stderr, "Constant not implemented");
    exit(1);
}

enum result serialize(LLVMExecutionEngineRef engine, struct ast_node* n, struct buffer* bf)
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
                    long v;
                    if (td->bit_count == 32) {
                        int32_t (*main_func)() = (int32_t (*)()) LLVMGetFunctionAddress(engine, "main");
                        buffer_add_format(bf, "%d", main_func());
                        found = true;
                    } else if (td->bit_count == 64) {
                        int64_t (*main_func)() = (int64_t (*)()) LLVMGetFunctionAddress(engine, "main");
                        buffer_add_format(bf, "%ld", main_func());
                        found = true;
                    }
                } else if (td->type == type_float) {
                    double v;
                    if (td->bit_count == 32) {
                        float (*main_func)() = (float (*)())LLVMGetFunctionAddress(engine, "main");
                        buffer_add_format(bf, "%f", main_func());
                        found = true;
                    } else if (td->bit_count == 64) {
                        double (*main_func)() = (double (*)())LLVMGetFunctionAddress(engine, "main");
                        buffer_add_format(bf, "%lf", main_func());
                        found = true;
                    }

                } else if (td->type == type_string) {
                    char* (*main_func)() = (char* (*)())LLVMGetFunctionAddress(engine, "main");
                    buffer_add_format(bf, "%s", main_func());
                    found = true;
                }
            }
        }
    }

    if (!found) {
        r = set_error("unknown type");
    }

    return r;
}

enum result serialize2(LLVMGenericValueRef v, struct ast_node* n, struct buffer* bf)
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
                }
            }
        }
    }

    if (!found) {
        r = set_error("unknown type");
    }

    return r;
}

void cg_jit(struct comp_unit* cu, struct buffer* bf, bool output_bc, bool output_ll)
{
    LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");
    LLVMContextRef context = LLVMContextCreate();

    LLVMTypeRef ret_type = get_llvm_type(cu->root, NULL);
    LLVMTypeRef fun_type = LLVMFunctionType(ret_type, NULL, 0, 0);
    LLVMValueRef _main = LLVMAddFunction(mod, "main", fun_type);

    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(_main, "entry");

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

    /*
    enum result r = serialize(engine, cu->root, bf);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
    }
     */
    //LLVMGenericValueRef v = LLVMRunFunction(engine, _main, 0, NULL);

    // Write out bitcode to file
    if (LLVMWriteBitcodeToFile(mod, "main.bc") != 0) {
        fprintf(stderr, "error writing bitcode to file, skipping\n");
    }

    error = NULL;
    if (LLVMPrintModuleToFile(mod, "main.ll", &error)) {
        fprintf(stderr, "%s\n", error);
        LLVMDisposeMessage(error);
    }

    LLVMGenericValueRef v = LLVMRunFunction(engine, _main, 0, NULL);
    enum result r = serialize2(v, cu->root, bf);
    if (r == result_error) {
        fprintf(stderr, "%s\n", error_message);
    }
    //printf("%lld\n", LLVMGenericValueToInt(v, true));


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
    } else if (n->type == ast_type_string) {
        return cg_string(cgd, n);
    } else {
        fprintf(stderr, "unknown expression: %d\n", n->type);
        exit(1);
    }
}

LLVMValueRef cg_stmts(struct cg_data* cgd, struct ast_node* n)
{
    LLVMValueRef last_v = NULL;
    struct ast_node* last_n = NULL;
    struct ast_node* stmt = ast_node_get(n, 0);
    while (stmt) {
        last_v = cg_expr(cgd, stmt);
        last_n = stmt;
        stmt = stmt->next;
    }
    if (LLVMIsConstantString(last_v)) {
        //last = LLVMBuildGEP2(cgd->builder, LLVMPointerType(LLVMInt8Type(), 0), last, 0, 0, "abc");
        //last = LLVMConstInBoundsGEP2(LLVMPointerType(LLVMInt8Type(), 0), last, 0, 0);
        //last = LLVMBuildInBoundsGEP2(cgd->builder, LLVMArrayType(LLVMInt8Type(), 7), last, 0, 0, "abc");
        //last = LLVMConstString("abc", 3, false);
        //LLVMSetGlobalConstant(last, true);
        //LLVMGetAsString(last, );
        buffer_finish(&last_n->value);
        last_v = LLVMBuildGlobalString(cgd->builder, last_n->value.buf, "str");
        LLVMTypeRef last_t = LLVMArrayType(LLVMInt8Type(), last_n->value.size+1);
        LLVMValueRef zeroIndex = LLVMConstInt( LLVMInt64Type(), 0, true );
        LLVMValueRef indexes[2] = { zeroIndex, zeroIndex };
        //last_v = LLVMBuildInBoundsGEP2(cgd->builder, last_t, last_v, indexes, 2, "gep");
        last_v = LLVMConstInBoundsGEP2(last_t, last_v, indexes, 2);
    }
    return last_v;
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

            LLVMTypeRef t = get_llvm_type(NULL, tu);
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

LLVMValueRef cg_string(struct cg_data* cgd, struct ast_node* n)
{
    return get_llvm_constant(n);
}
