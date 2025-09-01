#include "tools.h"

using namespace llvm;

namespace Akela_llvm {
    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_stmts(Jit_data* jd, Ake_Ast* n)
    {
        Value* last_v = nullptr;
        Ake_Ast* last_n = nullptr;
        Ake_Ast* stmt = n->data.stmts.list.head;
        Ake_begin_environment(&jd->st);
        while (stmt) {
            last_v = Dispatch(jd, stmt);
            last_n = stmt;
            stmt = stmt->next;
        }
        Ake_end_environment(&jd->st);
        return last_v;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_if(Jit_data* jd, Ake_Ast* n)
    {
        bool has_else = false;

        Type* type = nullptr;
        Value* ptr = nullptr;
        if (n->type) {
            type = Get_type(jd, n->type);
        }
        if (type) {
            ptr = jd->Builder->CreateAlloca(type, nullptr, "ifresult");
        }

        struct Zinc_list l{};
        Zinc_list_init(&l);

        BasicBlock* cond_block = nullptr;
        BasicBlock* then_block = nullptr;
        BasicBlock* next_block = nullptr;
        Ake_Ast* branch = n->data._if_.branches.head;
        size_t i = 0;
        while (branch) {
            if (branch->kind == AKE_AST_COND_BRANCH) {
                Ake_Ast* cond = branch->data.cond_branch.cond;
                Ake_Ast* body = branch->data.cond_branch.body;

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
                Value* cond_value = Dispatch(jd, cond);
                Value* branch_value = jd->Builder->CreateCondBr(cond_value, then_block, next_block);

                jd->Builder->SetInsertPoint(then_block);
                Value* body_value = Handle_stmts(jd, body);
                if (type) {
                    jd->Builder->CreateStore(body_value, ptr);
                }
                Zinc_list_add_item(&l, then_block);      /* branch to end after end_block is created */

                jd->Builder->SetInsertPoint(next_block);

            } else if (branch->kind == Ake_ast_type_default_branch) {
                Ake_Ast* body = Ake_AstGet(branch, 0);

                Value* body_value = Handle_stmts(jd, body);
                if (type) {
                    jd->Builder->CreateStore(body_value, ptr);
                }
                Zinc_list_add_item(&l, next_block);      /* branch to end after end_block is created */
                has_else = true;

            } else {
                assert(false);
            }

            branch = branch->next;
            i++;
        }

        BasicBlock* end_block = nullptr;
        if (has_else) {
            end_block = BasicBlock::Create(*jd->TheContext, "endiftmp", jd->toplevel);
        } else {
            end_block = next_block;
        }
        Zinc_list_node* ln = l.head;
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

        Zinc_list_destroy(&l, nullptr);

        return value;
    }
}