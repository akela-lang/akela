#include "ast.h"
#include "type.h"
#include <assert.h>

void Ake_indent_print(size_t level);
char* Ake_ast_cent_name(Ake_AstKind type);
void Ake_type_def_cent_print(Ake_Type* td, size_t level, bool is_property);
char* Ake_type_def_cent_name(Ake_TypeKind kind);
char* Ake_type_param_cent_name(Ake_TypeParamKind kind);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_ast_cent_print(Ake_Ast* n, size_t level)
{
    if (n) {
        Ake_indent_print(level);
        printf("%s {\n", Ake_ast_cent_name(n->kind));

        level++;

        if (n->value.size > 0) {
            Ake_indent_print(level);
            printf(".value = \"%s\"\n", Zinc_string_c_str(&n->value));
        }

        if (n->type) {
            Ake_indent_print(level);
            printf(".tu = ");
            Ake_type_def_cent_print(n->type, level, true);
        }

        Ake_Ast* p = n->head;
        while (p) {
            Ake_ast_cent_print(p, level);
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
    if (type == Ake_ast_type_id) {
        return "Ast::Id";
    }

    if (type == Ake_ast_type_sign) {
        return "Ast::Sign";
    }

    if (type == Ake_ast_type_number) {
        return "Ast::Number";
    }

    if (type == Ake_ast_type_string) {
        return "Ast::String";
    }

    if (type == Ake_ast_type_assign) {
        return "Ast::Assign";
    }

    if (type == Ake_ast_type_plus) {
        return "Ast::Plus";
    }

    if (type == Ake_ast_type_minus) {
        return "Ast::Minus";
    }

    if (type == Ake_ast_type_mult) {
        return "Ast::Mult";
    }

    if (type == Ake_ast_type_divide) {
        return "Ast::Divide";
    }

    if (type == Ake_ast_type_stmts) {
        return "Ast::Stmts";
    }

    if (type == Ake_ast_type_function) {
        return "Ast::Function";
    }

    if (type == Ake_ast_type_dseq) {
        return "Ast::DSeq";
    }

    if (type == Ake_ast_type_dret) {
        return "Ast::DRet";
    }

    if (type == Ake_ast_type_call) {
        return "Ast::Call";
    }

    if (type == Ake_ast_type_cseq) {
        return "Ast::CSeq";
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

    if (type == Ake_ast_type_eseq) {
        return "Ast::Eseq";
    }

    if (type == Ake_ast_type_let_lseq) {
        return "Ast::LetLseq";
    }

    if (type == Ake_ast_type_let_rseq) {
        return "Ast::LetRseq";
    }

    if (type == Ake_ast_type_error) {
        return "Ast::Error";
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

    if (type == Ake_ast_type_self) {
        return "Ast::Self";
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
void Ake_type_def_cent_print(Ake_Type* td, size_t level, bool is_property)
{
    if (!is_property) {
        Ake_indent_print(level);
    }
    printf("%s {\n", Ake_type_def_cent_name(td->kind));

    level++;

    if (td->name.size > 0) {
        Ake_indent_print(level);
        printf(".name = \"%s\"\n", Zinc_string_c_str(&td->name));
    }

    switch (td->kind) {
        case AKE_TYPE_INTEGER:
            Ake_indent_print(level);
            printf(".bit_count = %d\n", td->data.integer.bit_count);
            break;
        case AKE_TYPE_NATURAL:
            Ake_indent_print(level);
            printf(".bit_count = %d\n", td->data.natural.bit_count);
            break;
        case AKE_TYPE_REAL:
            Ake_indent_print(level);
            printf(".bit_count = %d\n", td->data.real.bit_count);
            break;
        case AKE_TYPE_BOOLEAN:
            break;
        case AKE_TYPE_STRUCT:
            Ake_TypeField* field = td->data.fields.head;
            while (field) {
                Ake_indent_print(level);
                printf("TypeField {\n");
                level++;
                Ake_indent_print(level);
                printf(".name = \"%s\"\n", Zinc_string_c_str(&field->name));
                Ake_indent_print(level);
                printf(".td = ");
                Ake_type_def_cent_print(field->td, level, true);
                level--;
                Ake_indent_print(level);
                printf("}\n");
            }
            break;
        case AKE_TYPE_ARRAY:
            Ake_indent_print(level);
            printf(".dim = %zu\n", td->data.array.dim);
            Ake_indent_print(level);
            printf(".td = ");
            Ake_type_def_cent_print(td->data.array.td, level, true);
            break;
        case AKE_TYPE_ARRAY_CONST:
            Ake_indent_print(level);
            printf(".dim = %zu\n", td->data.array_const.dim);
            Ake_indent_print(level);
            printf(".td = ");
            Ake_type_def_cent_print(td->data.array_const.td, level, true);
        case AKE_TYPE_SLICE:
            Ake_indent_print(level);
            printf(".td = ");
            Ake_type_def_cent_print(td->data.slice.td, level, true);
        case AKE_TYPE_POINTER:
            Ake_indent_print(level);
            printf(".td = ");
            Ake_type_def_cent_print(td->data.pointer.td, level, true);
        case AKE_TYPE_FUNCTION:
            Ake_TypeParam* tp = td->data.function.input_head;
            while (tp) {
                Ake_indent_print(level);
                printf("TypeParam {\n");
                level++;
                Ake_indent_print(level);
                printf(".kind = %s\n", Ake_type_param_cent_name(tp->kind));
                Ake_indent_print(level);
                printf(".name = \"%s\"\n", Zinc_string_c_str(&tp->name));
                Ake_indent_print(level);
                printf(".td = ");
                Ake_type_def_cent_print(tp->td, level, true);
                level--;
                Ake_indent_print(level);
                printf("}\n");
                tp = tp->next;
            }
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
        return "TypeDef::None";
    }

    if (kind == AKE_TYPE_INTEGER) {
        return "TypeDef::Integer";
    }

    if (kind == AKE_TYPE_NATURAL) {
        return "TypeDef::Natural";
    }

    if (kind == AKE_TYPE_REAL) {
        return "TypeDef::Real";
    }

    if (kind == AKE_TYPE_BOOLEAN) {
        return "TypeDef::Boolean";
    }

    if (kind == AKE_TYPE_STRUCT) {
        return "TypeDef::Struct";
    }

    if (kind == AKE_TYPE_ARRAY) {
        return "TypeDef::Array";
    }

    if (kind == AKE_TYPE_ARRAY_CONST) {
        return "TypeDef::ArrayConst";
    }

    if (kind == AKE_TYPE_SLICE) {
        return "TypeDef::Slice";
    }

    if (kind == AKE_TYPE_POINTER) {
        return "TypeDef::Pointer";
    }

    if (kind == AKE_TYPE_FUNCTION) {
        return "TypeDef::Function";
    }

    assert(false && "unknown kind");
}

char* Ake_type_param_cent_name(Ake_TypeParamKind kind)
{
    if (kind == AKE_TYPE_PARAM_REGULAR) {
        return "Param::Regular";
    }

    if (kind == AKE_TYPE_PARAM_SELF) {
        return "Param::Self";
    }

    if (kind == AKE_TYPE_PARAM_ELLIPSIS) {
        return "Param::Ellipsis";
    }

    assert(false && "unknown kind");
}