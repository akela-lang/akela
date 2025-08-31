#include "ast.h"
#include "type.h"
#include <assert.h>
#include "type_slots.h"

void Ake_print_types(Ake_TypeSlots* slots);
void Ake_ast_cent_print(Ake_Ast* n, size_t level, bool is_property, Ake_TypeSlots* slots);
void Ake_indent_print(size_t level);
char* Ake_ast_cent_name(Ake_AstKind type);
void Ake_type_cent_print(Ake_Type* type, size_t level, bool is_property);
char* Ake_type_def_cent_name(Ake_TypeKind kind);
char* Ake_type_param_cent_name(Ake_TypeParamKind kind);

void Ake_tree_print(Ake_Ast* n, Zinc_string* initial_line)
{
    printf("%s\n", Zinc_string_c_str(initial_line));

    Ake_TypeSlots slots;
    Ake_TypeSlotsInit(&slots);

    Ake_TypeSlotsScan(&slots, n);
    Ake_print_types(&slots);
    Ake_ast_cent_print(n, 0, false, &slots);

    Ake_TypeSlotsDestroy(&slots);
 }

void Ake_print_types(Ake_TypeSlots* slots)
{
    size_t i = 0;
    Ake_TypeNode* node = slots->list.head;
    while (node) {
        printf("const type%zu = ", i);
        Ake_type_cent_print(node->type, 0, true);
        node = node->next;
        i++;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_ast_cent_print(Ake_Ast* n, size_t level, bool is_property, Ake_TypeSlots* slots)
{
    if (n) {
        if (!is_property) {
            Ake_indent_print(level);
        }
        printf("%s {\n", Ake_ast_cent_name(n->kind));

        level++;

        if (n->kind == AKE_AST_ID && n->data.id.value.size > 0) {
            Ake_indent_print(level);
            printf(".value = \"%s\"\n", Zinc_string_c_str(&n->data.id.value));
        } else if (n->struct_value.size > 0) {
            Ake_indent_print(level);
            printf(".value = \"%s\"\n", Zinc_string_c_str(&n->struct_value));
        } else if (n->kind == AKE_AST_NUMBER && n->data.number.value.size > 0) {
            Ake_indent_print(level);
            printf(".value = \"%s\"\n", Zinc_string_c_str(&n->data.number.value));
        } else if (n->kind == AKE_AST_STRING && n->data.string.value.size > 0) {
            Ake_indent_print(level);
            printf(".value = \"%s\"\n", Zinc_string_c_str(&n->data.string.value));
        } else if (n->boolean_value.size > 0) {
            Ake_indent_print(level);
            printf(".value = \"%s\"\n", Zinc_string_c_str(&n->boolean_value));
        }

        if (n->type) {
            Ake_indent_print(level);
            size_t slot = Ake_TypeSlotsGetSlot(slots, n->type);
            printf(".type = type%zu\n", slot);
        }

        switch (n->kind) {
            case AKE_AST_SIGN:
                Ake_indent_print(level);
                printf(".op = ");
                Ake_ast_cent_print(n->data.sign.op, level, true, slots);

                Ake_indent_print(level);
                printf(".right = ");
                Ake_ast_cent_print(n->data.sign.right, level, true, slots);
                break;
            case AKE_AST_ID:
                break;
            case AKE_AST_ASSIGN:
                if (n->data.assign.left) {
                    Ake_indent_print(level);
                    printf(".left = ");
                    Ake_ast_cent_print(n->data.assign.left, level, true, slots);
                }
                if (n->data.assign.right) {
                    Ake_indent_print(level);
                    printf(".right = ");
                    Ake_ast_cent_print(n->data.assign.right, level, true, slots);
                }
                break;
            case AKE_AST_PLUS:
                if (n->data.plus.left) {
                    Ake_indent_print(level);
                    printf(".left = ");
                    Ake_ast_cent_print(n->data.plus.left, level, true, slots);
                }
                if (n->data.plus.right) {
                    Ake_indent_print(level);
                    printf(".right = ");
                    Ake_ast_cent_print(n->data.plus.right, level, true, slots);
                }
                break;
            case AKE_AST_MINUS:
                if (n->data.minus.left) {
                    Ake_indent_print(level);
                    printf(".left = ");
                    Ake_ast_cent_print(n->data.minus.left, level, true, slots);
                }
                if (n->data.minus.right) {
                    Ake_indent_print(level);
                    printf(".right = ");
                    Ake_ast_cent_print(n->data.minus.right, level, true, slots);
                }
                break;
            case AKE_AST_MULT:
                if (n->data.mult.left) {
                    Ake_indent_print(level);
                    printf(".left = ");
                    Ake_ast_cent_print(n->data.mult.left, level, true, slots);
                }
                if (n->data.mult.right) {
                    Ake_indent_print(level);
                    printf(".right = ");
                    Ake_ast_cent_print(n->data.mult.right, level, true, slots);
                }
                break;
            case AKE_AST_DIVIDE:
                if (n->data.divide.left) {
                    Ake_indent_print(level);
                    printf(".left = ");
                    Ake_ast_cent_print(n->data.divide.left, level, true, slots);
                }
                if (n->data.divide.right) {
                    Ake_indent_print(level);
                    printf(".right = ");
                    Ake_ast_cent_print(n->data.divide.right, level, true, slots);
                }
                break;
            case AKE_AST_STMTS:
                level++;
                Ake_Ast* p = n->data.stmts.list.head;
                while (p) {
                    Ake_ast_cent_print(p, level, false, slots);
                    p = p->next;
                }
                level--;
            default:
                break;
        }

        Ake_Ast* p = n->head;
        while (p) {
            Ake_ast_cent_print(p, level, false, slots);
            p = p->next;
        }

        level--;

        Ake_indent_print(level);
        printf("}\n");
    } else {
        printf("null\n");
    }
}

void Ake_indent_print(size_t level)
{
    for (size_t i = 0; i < level; i++) {
        printf("  ");
    }
}

char* Ake_ast_cent_name(Ake_AstKind type)
{
    if (type == AKE_AST_ID) {
        return "Ast::Id";
    }

    if (type == AKE_AST_SIGN) {
        return "Ast::Sign";
    }

    if (type == AKE_AST_NUMBER) {
        return "Ast::Number";
    }

    if (type == AKE_AST_STRING) {
        return "Ast::String";
    }

    if (type == AKE_AST_ASSIGN) {
        return "Ast::Assign";
    }

    if (type == AKE_AST_PLUS) {
        return "Ast::Plus";
    }

    if (type == AKE_AST_MINUS) {
        return "Ast::Minus";
    }

    if (type == AKE_AST_MULT) {
        return "Ast::Mult";
    }

    if (type == AKE_AST_DIVIDE) {
        return "Ast::Divide";
    }

    if (type == AKE_AST_STMTS) {
        return "Ast::Stmts";
    }

    if (type == Ake_ast_type_function) {
        return "Ast::Function";
    }

    if (type == Ake_ast_type_dseq) {
        return "Ast::Dseq";
    }

    if (type == Ake_ast_type_dret) {
        return "Ast::Dret";
    }

    if (type == Ake_ast_type_call) {
        return "Ast::Call";
    }

    if (type == Ake_ast_type_cseq) {
        return "Ast::Cseq";
    }

    if (type == Ake_ast_type_if) {
        return "Ast::If";
    }

    if (type == Ake_ast_type_conditional_branch) {
        return "Ast::ConditionalBranch";
    }

    if (type == Ake_ast_type_default_branch) {
        return "Ast::DefaultBranch";
    }

    if (type == Ake_ast_type_equality) {
        return "Ast::Equality";
    }

    if (type == Ake_ast_type_not_equal) {
        return "Ast::NotEqual";
    }

    if (type == Ake_ast_type_less_than) {
        return "Ast::LessThan";
    }

    if (type == Ake_ast_type_less_than_or_equal) {
        return "Ast::LessThanOrEqual";
    }

    if (type == Ake_ast_type_greater_than) {
        return "Ast::GreaterThan";
    }

    if (type == Ake_ast_type_greater_than_or_equal) {
        return "Ast::GreaterThanOrEqual";
    }

    if (type == Ake_ast_type_not) {
        return "Ast::Not";
    }

    if (type == Ake_ast_type_and) {
        return "Ast::And";
    }

    if (type == Ake_ast_type_or) {
        return "Ast::Or";
    }

    if (type == Ake_ast_type_while) {
        return "Ast::While";
    }

    if (type == Ake_ast_type_for_range) {
        return "Ast::ForRange";
    }

    if (type == Ake_ast_type_for_iteration) {
        return "Ast::ForIteration";
    }

    if (type == Ake_ast_type_declaration) {
        return "Ast::Declaration";
    }

    if (type == Ake_ast_type_array_literal) {
        return "Ast::ArrayLiteral";
    }

    if (type == Ake_ast_type_array_subscript) {
        return "Ast::ArraySubscript";
    }

    if (type == Ake_ast_type_boolean) {
        return "Ast::Boolean";
    }

    if (type == Ake_ast_type_parenthesis) {
        return "Ast::Parenthesis";
    }

    if (type == Ake_ast_type_type) {
        return "Ast::Type";
    }

    if (type == Ake_ast_type_power) {
        return "Ast::Power";
    }

    if (type == Ake_ast_type_type_pool) {
        return "Ast::TypePool";
    }

    if (type == Ake_ast_type_dot) {
        return "Ast::Dot";
    }

    if (type == Ake_ast_type_struct) {
        return "Ast::Struct";
    }

    if (type == Ake_ast_type_return) {
        return "Ast::Return";
    }

    if (type == Ake_ast_type_prototype) {
        return "Ast::Prototype";
    }

    if (type == Ake_ast_type_extern) {
        return "Ast::Extern";
    }

    if (type == Ake_ast_type_struct_literal) {
        return "Ast::StructLiteral";
    }

    if (type == Ake_ast_type_struct_literal_field) {
        return "Ast::StructLiteralField";
    }

    if (type == Ake_ast_type_ellipsis) {
        return "Ast::Ellipsis";
    }

    if (type == Ake_ast_type_impl) {
        return "Ast::Impl";
    }

    if (type == Ake_ast_type_const) {
        return "Ast::Const";
    }

    if (type == Ake_ast_type_var) {
        return "Ast::Var";
    }

    assert(false && "unknown type");
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_type_cent_print(Ake_Type* type, size_t level, bool is_property)
{
    if (!is_property) {
        Ake_indent_print(level);
    }
    printf("%s {\n", Ake_type_def_cent_name(type->kind));

    level++;

    if (type->name.size > 0) {
        Ake_indent_print(level);
        printf(".name = \"%s\"\n", Zinc_string_c_str(&type->name));
    }

    switch (type->kind) {
        case AKE_TYPE_INTEGER:
            Ake_indent_print(level);
            printf(".bit_count = %d\n", type->data.integer.bit_count);
            break;
        case AKE_TYPE_NATURAL:
            Ake_indent_print(level);
            printf(".bit_count = %d\n", type->data.natural.bit_count);
            break;
        case AKE_TYPE_REAL:
            Ake_indent_print(level);
            printf(".bit_count = %d\n", type->data.real.bit_count);
            break;
        case AKE_TYPE_BOOLEAN:
            break;
        case AKE_TYPE_STRUCT:
            Ake_TypeField* field = type->data.fields.head;
            while (field) {
                Ake_indent_print(level);
                printf("TypeField {\n");
                level++;
                Ake_indent_print(level);
                printf(".name = \"%s\"\n", Zinc_string_c_str(&field->name));
                Ake_indent_print(level);
                printf(".type = ");
                Ake_type_cent_print(field->type, level, true);
                level--;
                Ake_indent_print(level);
                printf("}\n");
                field = field->next;
            }
            break;
        case AKE_TYPE_ARRAY:
            Ake_indent_print(level);
            printf(".is_const = %s\n", type->data.array.is_const ? "true" : "false");
            Ake_indent_print(level);
            printf(".dim = %zu\n", type->data.array.dim);
            Ake_indent_print(level);
            printf(".type = ");
            Ake_type_cent_print(type->data.array.type, level, true);
            break;
        case AKE_TYPE_SLICE:
            Ake_indent_print(level);
            printf(".type = ");
            Ake_type_cent_print(type->data.slice.type, level, true);
        case AKE_TYPE_POINTER:
            Ake_indent_print(level);
            printf(".type = ");
            Ake_type_cent_print(type->data.pointer.type, level, true);
        case AKE_TYPE_FUNCTION:

            if (type->data.function.input_head) {
                Ake_indent_print(level);
                printf(".input = Input {\n");
                level++;
                Ake_TypeParam* tp = type->data.function.input_head;
                while (tp) {
                    Ake_indent_print(level);
                    printf("%s {\n", Ake_type_param_cent_name(tp->kind));
                    level++;
                    Ake_indent_print(level);
                    printf(".name = \"%s\"\n", Zinc_string_c_str(&tp->name));
                    Ake_indent_print(level);
                    printf(".type = ");
                    Ake_type_cent_print(tp->type, level, true);
                    level--;
                    Ake_indent_print(level);
                    printf("}\n");
                    tp = tp->next;
                }
                level--;
                Ake_indent_print(level);
                printf("}\n");
            }

            if (type->data.function.output) {
                Ake_indent_print(level);
                printf(".output = ");
                Ake_type_cent_print(type->data.function.output, level, true);
            }
            break;
        default:
            assert(false && "invalid kind");
    }

    level--;

    Ake_indent_print(level);
    printf("}\n");
}

char* Ake_type_def_cent_name(Ake_TypeKind kind)
{
    if (kind == AKE_TYPE_NONE) {
        return "Type::None";
    }

    if (kind == AKE_TYPE_INTEGER) {
        return "Type::Integer";
    }

    if (kind == AKE_TYPE_NATURAL) {
        return "Type::Natural";
    }

    if (kind == AKE_TYPE_REAL) {
        return "Type::Real";
    }

    if (kind == AKE_TYPE_BOOLEAN) {
        return "Type::Boolean";
    }

    if (kind == AKE_TYPE_STRUCT) {
        return "Type::Struct";
    }

    if (kind == AKE_TYPE_ARRAY) {
        return "Type::Array";
    }

    if (kind == AKE_TYPE_SLICE) {
        return "Type::Slice";
    }

    if (kind == AKE_TYPE_POINTER) {
        return "Type::Pointer";
    }

    if (kind == AKE_TYPE_FUNCTION) {
        return "Type::Function";
    }

    assert(false && "unknown kind");
}

char* Ake_type_param_cent_name(Ake_TypeParamKind kind)
{
    if (kind == AKE_TYPE_PARAM_REGULAR) {
        return "TypeParam::Regular";
    }

    if (kind == AKE_TYPE_PARAM_ELLIPSIS) {
        return "TypeParam::Ellipsis";
    }

    assert(false && "unknown kind");
}