#include "akela_llvm_tools.h"

using namespace llvm;

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMAdd(JITData* jd, Ast_node* n)
{
    Ast_node* a = Ast_node_get(n, 0);
    Value* lhs = CodeGenLLVMDispatch(jd, a);

    Ast_node* b = Ast_node_get(n, 1);
    Value* rhs = CodeGenLLVMDispatch(jd, b);

    return jd->Builder->CreateAdd(lhs, rhs, "addtmp");
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMSub(JITData* jd, Ast_node* n)
{
    Ast_node* a = Ast_node_get(n, 0);
    Value* lhs = CodeGenLLVMDispatch(jd, a);

    Ast_node* b = Ast_node_get(n, 1);
    Value* rhs = CodeGenLLVMDispatch(jd, b);

    return jd->Builder->CreateSub(lhs, rhs, "subtmp");
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Value* CodeGenLLVMSign(JITData* jd, Ast_node* n)
{
    Ast_node* op = Ast_node_get(n, 0);
    Ast_node* number = Ast_node_get(n, 1);
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