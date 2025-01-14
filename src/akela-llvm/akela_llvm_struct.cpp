#include "akela_llvm_tools.h"
#include "akela/type_def.h"

using namespace llvm;

namespace Akela_llvm {
    Value* Handle_struct_literal_element(Jit_data* jd, Value* ptr, Ake_ast* n);

    Value* Handle_struct(Jit_data* jd, Ake_ast* n)
    {
        std::vector<Type*> type_list;
        Zinc_string_finish(&n->value);
        struct Ake_ast* element_dec = n->head;
        while (element_dec) {
            Ake_ast* element_type_node = Ast_node_get(element_dec, 1);
            Ake_type_use* element_tu = element_type_node->tu;
            Type* element_type = Get_type(jd, element_tu);
            if (element_tu->td->type == Ake_type_function) {
                element_type = element_type->getPointerTo();
            }
            type_list.push_back(element_type);
            element_dec = element_dec->next;
        }
        StructType* struct_type = StructType::create(*jd->TheContext, type_list, n->value.buf);
        assert(n->sym);
        assert(n->sym->td);
        n->sym->td->composite_type = struct_type;
        return nullptr;
    }

    Value* Handle_dot(Jit_data* jd, Ake_ast* n)
    {
        Ake_ast* left = Ast_node_get(n, 0);
        Ake_type_use* left_tu = left->tu;
        assert(left_tu);
        struct Ake_type_def* left_td = left_tu->td;
        assert(left_td);
        left->tu->context = Ake_type_context_ptr;
        Value* struct_value = Dispatch(jd, left);

        Ake_ast* right = Ast_node_get(n, 1);
        Ake_ast* dec = left_td->composite->head;
        auto struct_type = (StructType*)left_td->composite_type;
        size_t i = 0;
        Ake_ast* dec_id = nullptr;
        Ake_ast* dec_type = nullptr;
        Ake_type_use* dec_tu = nullptr;
        bool found = false;
        while (dec) {
            dec_id = Ast_node_get(dec, 0);
            dec_type = Ast_node_get(dec, 1);
            dec_tu = dec_type->tu;
            if (Zinc_string_compare(&dec_id->value, &right->value)) {
                found = true;
                break;
            }
            dec = dec->next;
            i++;
        }
        assert(found);
        Value* gep_value = jd->Builder->CreateStructGEP(struct_type, struct_value, i);

        if (n->tu->context == Ake_type_context_ptr) {
            return gep_value;
        } else if (dec_tu->is_array) {
            return gep_value;
        } else {
            Type* dot_type = Get_type_pointer(jd, n->tu);
            Zinc_string_finish(&right->value);
            return jd->Builder->CreateLoad(dot_type, gep_value, right->value.buf);
        }
    }

    Value* Handle_struct_literal(Jit_data* jd, Ake_ast* n)
    {
        assert(n && n->tu && n->tu->td);
        assert(n->tu->td->type == Ake_type_struct);
        assert(n->tu->td->composite_type);
        assert(n->tu->td->composite);
        auto t = (StructType*)n->tu->td->composite_type;
        struct Zinc_string bf;
        Zinc_string_init(&bf);
        Zinc_string_copy(&n->tu->td->name, &bf);
        Zinc_string_add_str(&bf, ".tmp");
        Zinc_string_finish(&bf);
        Value* value;
        if (n->tu->lhs_allocation) {
            value = (Value*)n->tu->lhs_allocation;
        } else {
            value = jd->Builder->CreateAlloca(t, nullptr, bf.buf);
        }
        Zinc_string_destroy(&bf);
        size_t i = 0;
        Ake_ast* field = n->head;
        while (field) {
            Ake_ast* id = Ast_node_get(field, 0);
            Ake_ast* expr = Ast_node_get(field, 1);

            Zinc_string_finish(&id->value);
            Value* gep_value = jd->Builder->CreateStructGEP(t, value, i, id->value.buf);
            if (expr->tu->td->type == Ake_type_struct) {
                Handle_struct_literal_element(jd, gep_value, expr);
            } else if (expr->tu->is_array) {
                Value* expr_value = Dispatch(jd, expr);
                Array_copy(jd, expr->tu, expr->tu, gep_value, expr_value);
            } else {
                Value* expr_value = Dispatch(jd, expr);
                jd->Builder->CreateStore(expr_value, gep_value);
            }

            field = field->next;
            i++;
        }

        return value;
    }

    Value* Handle_struct_literal_element(Jit_data* jd, Value* ptr, Ake_ast* n)
    {
        auto t = (StructType*)n->tu->td->composite_type;

        size_t i = 0;
        Ake_ast* field = n->head;
        while (field) {
            Ake_ast* id = Ast_node_get(field, 0);
            Ake_ast* expr = Ast_node_get(field, 1);

            Zinc_string_finish(&id->value);
            Value* gep_value = jd->Builder->CreateStructGEP(t, ptr, i, id->value.buf);
            if (expr->tu->td->type == Ake_type_struct) {
                Handle_struct_literal_element(jd, gep_value, expr);
            } else if (expr->tu->is_array) {
                Value* expr_value = Dispatch(jd, expr);
                Array_copy(jd, expr->tu, expr->tu, gep_value, expr_value);
            } else {
                Value* expr_value = Dispatch(jd, expr);
                jd->Builder->CreateStore(expr_value, gep_value);
            }
            field = field->next;
            i++;
        }

        return ptr;
    }
}