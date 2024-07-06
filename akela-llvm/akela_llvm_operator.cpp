#include "akela_llvm_tools.h"

using namespace llvm;

namespace Akela_llvm {
    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_mult(Jit_data* jd, Ast_node* n)
    {
        Ast_node* a = Ast_node_get(n, 0);
        Value* lhs = Dispatch(jd, a);

        Ast_node* b = Ast_node_get(n, 1);
        Value* rhs = Dispatch(jd, b);

        if (a->tu->td->type == type_float || b->tu->td->type == type_float) {
            return jd->Builder->CreateFMul(lhs, rhs, "multmp");
        } else {
            return jd->Builder->CreateMul(lhs, rhs, "multmp");
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_add(Jit_data* jd, Ast_node* n)
    {
        Ast_node* a = Ast_node_get(n, 0);
        Value* lhs = Dispatch(jd, a);

        Ast_node* b = Ast_node_get(n, 1);
        Value* rhs = Dispatch(jd, b);

        if (a->tu->td->type == type_float || b->tu->td->type == type_float) {
            return jd->Builder->CreateFAdd(lhs, rhs, "addtmp");
        } else {
            return jd->Builder->CreateAdd(lhs, rhs, "addtmp");
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_sub(Jit_data* jd, Ast_node* n)
    {
        Ast_node* a = Ast_node_get(n, 0);
        Value* lhs = Dispatch(jd, a);

        Ast_node* b = Ast_node_get(n, 1);
        Value* rhs = Dispatch(jd, b);

        return jd->Builder->CreateSub(lhs, rhs, "subtmp");
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_sign(Jit_data* jd, Ast_node* n)
    {
        Ast_node* op = Ast_node_get(n, 0);
        Ast_node* number = Ast_node_get(n, 1);
        Value* number_value = Dispatch(jd, number);

        if (op->type == Ast_type_plus) {
            return number_value;
        }

        assert(op->type == Ast_type_minus);

        Type* number_type = Get_type(jd, number->tu);

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
}