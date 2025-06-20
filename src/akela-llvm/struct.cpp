#include "tools.h"
#include "akela/type.h"

using namespace llvm;

namespace Akela_llvm {
    Value* Handle_struct_literal_element(Jit_data* jd, Value* ptr, Ake_ast* n);

    StructType* GetStructTypeFromNode(Jit_data* jd, Ake_ast* n)
    {
        std::vector<Type*> type_list;
        Zinc_string_finish(&n->value);
        struct Ake_ast* element_dec = n->head;
        while (element_dec) {
            Ake_ast* element_type_node = Ast_node_get(element_dec, 1);
            Ake_type_use* element_tu = element_type_node->tu;
            Type* element_type = Get_type(jd, element_tu);
            if (element_tu->type == Ake_type_use_function) {
                //element_type = element_type->getPointerTo();
                element_type = PointerType::get(element_type, 0);
            }
            type_list.push_back(element_type);
            element_dec = element_dec->next;
        }
        return StructType::create(*jd->TheContext, type_list, n->value.buf);
    }

    StructType* GetStructTypeFromType(Jit_data* jd, Ake_TypeDef* td)
    {
        std::vector<Type*> type_list;
        assert(td->kind == AKE_TYPE_DEF_STRUCT);
        Ake_TypeField* tf = td->data.fields.head;
        while (tf) {
            assert(tf->td->kind == AKE_TYPE_DEF_OLD);
            Type* element_type = Get_type(jd, tf->td->data.old);
            if (tf->td->data.old->type == Ake_type_use_function) {
                //element_type = element_type->getPointerTo();
                element_type = PointerType::get(element_type, 0);
            }
            type_list.push_back(element_type);
            tf = tf->next;
        }

        return StructType::create(*jd->TheContext, type_list, Zinc_string_c_str(&td->name));
    }

    Value* Handle_struct(Jit_data* jd, Ake_ast* n)
    {
        StructType* struct_type = GetStructTypeFromNode(jd, n);
        Ake_Environment* env = Ake_get_current_env(n);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &n->value, n->loc.start);
        sym->td->data.fields.backend_type = struct_type;
        return nullptr;
    }

    Value* Handle_dot(Jit_data* jd, Ake_ast* n)
    {
        Ake_ast* left = Ast_node_get(n, 0);
        Ake_type_use* left_tu = left->tu;
        assert(left_tu);
        Ake_TypeDef* left_td = left_tu->td;
        assert(left_td);
        left->tu->context = Ake_type_context_ptr;
        Value* struct_value = Dispatch(jd, left);

        Ake_ast* right = Ast_node_get(n, 1);
        Ake_TypeField* tf = left_td->data.fields.head;
        auto struct_type = (StructType*)left_td->data.fields.backend_type;
        size_t i = 0;
        Ake_type_use* dec_tu = nullptr;
        bool found = false;
        while (tf) {
            assert(tf->td->kind == AKE_TYPE_DEF_OLD);
            dec_tu = tf->td->data.old;
            if (Zinc_string_compare(&tf->name, &right->value)) {
                found = true;
                break;
            }
            tf = tf->next;
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
        assert(n->tu->td->kind == AKE_TYPE_DEF_STRUCT);
        assert(n->tu->td->data.fields.backend_type);
        auto t = (StructType*)GetStructTypeFromType(jd, n->tu->td);
        Zinc_string bf;
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
            if (expr->tu->td && expr->tu->td->kind == AKE_TYPE_DEF_STRUCT) {
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
        auto t = (StructType*)n->tu->td->data.fields.backend_type;

        size_t i = 0;
        Ake_ast* field = n->head;
        while (field) {
            Ake_ast* id = Ast_node_get(field, 0);
            Ake_ast* expr = Ast_node_get(field, 1);

            Zinc_string_finish(&id->value);
            Value* gep_value = jd->Builder->CreateStructGEP(t, ptr, i, id->value.buf);
            if (expr->tu->td->kind == AKE_TYPE_DEF_STRUCT) {
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