#include "tools.h"
#include "akela/type.h"
#include "akela/update_symbol.h"

using namespace llvm;

namespace Akela_llvm {
    Value* Handle_struct_literal_element(Jit_data* jd, Value* ptr, Ake_Ast* n);

    StructType* GetStructTypeFromNode(Jit_data* jd, Ake_Ast* n)
    {
        std::vector<Type*> type_list;
        Zinc_string_finish(&n->struct_value);
        struct Ake_Ast* element_dec = n->head;
        while (element_dec) {
            Ake_Ast* element_type_node = element_dec->data.declaration.type_node;
            Ake_Type* element_type = element_type_node->type;
            Type* element_type2 = Get_type(jd, element_type);
            if (element_type->kind == AKE_TYPE_FUNCTION) {
                element_type2 = PointerType::get(*jd->TheContext, 0);
            }
            type_list.push_back(element_type2);
            element_dec = element_dec->next;
        }
        return StructType::create(
            *jd->TheContext,
            type_list,
            Zinc_string_c_str(&n->struct_value));
    }

    StructType* GetStructTypeFromType(Jit_data* jd, Ake_Type* td)
    {
        std::vector<Type*> type_list;
        assert(td->kind == AKE_TYPE_STRUCT);
        Ake_TypeField* tf = td->data.fields.head;
        while (tf) {
            Type* element_type = Get_type(jd, tf->type);
            if (tf->type->kind == AKE_TYPE_FUNCTION) {
                element_type = PointerType::get(*jd->TheContext, 0);
            }
            type_list.push_back(element_type);
            tf = tf->next;
        }

        return StructType::create(*jd->TheContext, type_list, Zinc_string_c_str(&td->name));
    }

    Value* Handle_struct(Jit_data* jd, Ake_Ast* n)
    {
        StructType* struct_type = GetStructTypeFromNode(jd, n);
        Ake_Environment* env = jd->st.top;
        Ake_UpdateSymbolStruct(&jd->st, n);
        Ake_symbol* sym = Ake_EnvironmentGet(env, &n->struct_value);
        return nullptr;
    }

    bool DotIsTypeContextPointer(Ake_Ast* n)
    {
        if (n->parent->kind == Ake_ast_type_dot) {
            Ake_Ast* p0 = Ake_AstGet(n->parent, 0);
            if (p0 == n) {
                return true;
            }
        }

        if (n->parent->kind == AKE_AST_ASSIGN) {
            Ake_Ast* p0 = n->parent->data.assign.left;
            if (p0 == n) {
                return true;
            }
        }

        if (n->type->kind == AKE_TYPE_ARRAY) {
            return true;
        }

        return false;
    }

    Value* Handle_dot(Jit_data* jd, Ake_Ast* n)
    {
        Ake_Ast* left = Ake_AstGet(n, 0);
        Ake_Type* left_type = left->type;
        assert(left_type);
        assert(left_type->kind == AKE_TYPE_STRUCT);
        Value* struct_value = Dispatch(jd, left);

        Ake_Ast* right = Ake_AstGet(n, 1);
        Ake_TypeField* tf = left_type->data.fields.head;
        //auto struct_type = (StructType*)left_tu->data.fields.backend_type;
        StructType* struct_type = GetStructTypeFromType(jd, left_type);
        size_t i = 0;
        Ake_Type* dec_tu = nullptr;
        bool found = false;
        while (tf) {
            dec_tu = tf->type;
            if (Zinc_string_compare(&tf->name, &right->data.id.value)) {
                found = true;
                break;
            }
            tf = tf->next;
            i++;
        }
        assert(found);
        Value* gep_value = jd->Builder->CreateStructGEP(struct_type, struct_value, i);

        if (DotIsTypeContextPointer(n)) {
            return gep_value;
        }

        Type* dot_type = Get_type_pointer(jd, n->type);
        Zinc_string_finish(&right->data.id.value);
        return jd->Builder->CreateLoad(dot_type, gep_value, Zinc_string_c_str(&right->data.id.value));
    }

    Value* Handle_struct_literal(Jit_data* jd, Ake_Ast* n)
    {
        assert(n && n->type);
        assert(n->type->kind == AKE_TYPE_STRUCT);
        auto t = (StructType*)GetStructTypeFromType(jd, n->type);
        Zinc_string bf;
        Zinc_string_init(&bf);
        Zinc_string_copy(&n->type->name, &bf);
        Zinc_string_add_str(&bf, ".tmp");
        Zinc_string_finish(&bf);
        Value* value;
        if (n->parent->kind == Ake_ast_type_const || n->parent->kind == Ake_ast_type_var) {
            Ake_Environment* env = jd->st.top;
            Ake_Ast* lhs =  Ake_AstGet(n->parent, 0);
            Ake_Ast* type_node = Ake_AstGet(n->parent, 1);
            Ake_symbol* sym = Ake_EnvironmentGet(env, &lhs->data.id.value);
            value = (Value*)sym->value;
        } else {
            value = jd->Builder->CreateAlloca(t, nullptr, bf.buf);
        }
        Zinc_string_destroy(&bf);
        size_t i = 0;
        Ake_Ast* field = n->head;
        while (field) {
            Ake_Ast* id = Ake_AstGet(field, 0);
            Ake_Ast* expr = Ake_AstGet(field, 1);

            Zinc_string_finish(&id->data.id.value);
            Value* gep_value = jd->Builder->CreateStructGEP(t, value, i, id->data.id.value.buf);
            if (expr->type && expr->type->kind == AKE_TYPE_STRUCT) {
                Handle_struct_literal_element(jd, gep_value, expr);
            } else if (IsArray(expr->type->kind)) {
                Value* expr_value = Dispatch(jd, expr);
                Array_copy(jd, expr->type, expr->type, gep_value, expr_value);
            } else {
                Value* expr_value = Dispatch(jd, expr);
                jd->Builder->CreateStore(expr_value, gep_value);
            }

            field = field->next;
            i++;
        }

        return value;
    }

    /* NOLINTNEXTLINE(misc-no-recursion) */
    Value* Handle_struct_literal_element(Jit_data* jd, Value* ptr, Ake_Ast* n)
    {
        //auto t = (StructType*)n->tu->data.fields.backend_type;
        StructType* t = GetStructTypeFromType(jd, n->type);

        size_t i = 0;
        Ake_Ast* field = n->head;
        while (field) {
            Ake_Ast* id = Ake_AstGet(field, 0);
            Ake_Ast* expr = Ake_AstGet(field, 1);

            Zinc_string_finish(&id->data.id.value);
            Value* gep_value = jd->Builder->CreateStructGEP(t, ptr, i, id->data.id.value.buf);
            if (expr->type->kind == AKE_TYPE_STRUCT) {
                Handle_struct_literal_element(jd, gep_value, expr);
            } else if (IsArray(expr->type->kind)) {
                Value* expr_value = Dispatch(jd, expr);
                Array_copy(jd, expr->type, expr->type, gep_value, expr_value);
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