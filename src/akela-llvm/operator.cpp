#include "tools.h"

using namespace llvm;

namespace Akela_llvm {
    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_mult(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* a = n->data.mult.left;
        Value* lhs = Dispatch(jd, a);

        Ake_Ast* b = n->data.mult.right;
        Value* rhs = Dispatch(jd, b);

        if (a->type->kind == AKE_TYPE_REAL || b->type->kind == AKE_TYPE_REAL) {
            return jd->Builder->CreateFMul(lhs, rhs, "multmp");
        } else {
            return jd->Builder->CreateMul(lhs, rhs, "multmp");
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_add(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* a = n->data.plus.left;
        Value* lhs = Dispatch(jd, a);

        Ake_Ast* b = n->data.plus.right;
        Value* rhs = Dispatch(jd, b);

        if (a->type->kind == AKE_TYPE_REAL || b->type->kind == AKE_TYPE_REAL) {
            return jd->Builder->CreateFAdd(lhs, rhs, "addtmp");
        } else {
            return jd->Builder->CreateAdd(lhs, rhs, "addtmp");
        }
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_sub(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* a = n->data.minus.left;
        Value* lhs = Dispatch(jd, a);

        Ake_Ast* b = n->data.minus.right;
        Value* rhs = Dispatch(jd, b);

        return jd->Builder->CreateSub(lhs, rhs, "subtmp");
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_sign(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* op = n->data.sign.op;
        Ake_Ast* number = n->data.sign.right;
        Value* number_value = Dispatch(jd, number);

        if (op->kind == AKE_AST_PLUS) {
            return number_value;
        }

        assert(op->kind == AKE_AST_MINUS);

        Type* number_type = Get_type(jd, number->type);

        Value* zero_value;
        if (n->type->kind == AKE_TYPE_INTEGER) {
            zero_value = ConstantInt::get(
                number_type,
                APInt(number->type->data.integer.bit_count,
                    0,
                    true));
        } else if (n->type->kind == AKE_TYPE_NATURAL) {
            zero_value = ConstantInt::get(
                number_type,
                APInt(number->type->data.natural.bit_count,
                    0,
                    false));
        } else if (n->type->kind == AKE_TYPE_REAL) {
            zero_value = ConstantFP::get(number_type, APFloat(0.0));
        } else {
            assert(false);
        }
        Value* value = jd->Builder->CreateSub(zero_value, number_value, "negatetmp");
        return value;
    }

    Value* Handle_and(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* lhs = n->data._and_.left;
        Ake_Ast* rhs = n->data._and_.right;

        Value* lhs_value = Dispatch(jd, lhs);
        BasicBlock* curBB = jd->Builder->GetInsertBlock();
        BasicBlock* rhsBB = BasicBlock::Create(*jd->TheContext, "and.rhs", curBB->getParent());
        BasicBlock* mergeBB = BasicBlock::Create(*jd->TheContext, "and.end", curBB->getParent());

        jd->Builder->CreateCondBr(lhs_value, rhsBB, mergeBB);

        jd->Builder->SetInsertPoint(rhsBB);
        Value* rhs_value = Dispatch(jd, rhs);
        jd->Builder->CreateBr(mergeBB);
        rhsBB = jd->Builder->GetInsertBlock();

        jd->Builder->SetInsertPoint(mergeBB);
        Type* t = Get_type(jd, n->type);
        PHINode* phi = jd->Builder->CreatePHI(t, 2, "and.tmp");
        phi->addIncoming(jd->Builder->getFalse(), curBB);
        phi->addIncoming(rhs_value, rhsBB);

        return phi;
    }
}

