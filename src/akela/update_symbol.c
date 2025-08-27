#include "ast.h"
#include "zinc/memory.h"
#include "parse_tools.h"
#include "parse_types.h"

void Ake_UpdateSymbolExtern(Ake_parse_state* ps, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Ast* proto = Ake_AstGet(n, 0);
        if (proto) {
            Ake_Ast *id_node = Ake_AstGet(proto, 0);
            struct Ake_Symbol *new_sym = NULL;
            Zinc_malloc_safe((void **) &new_sym, sizeof(struct Ake_Symbol));
            Ake_SymbolInit(new_sym);
            new_sym->kind = AKE_SYMBOL_VARIABLE;
            Ake_Type* type = Ake_TypeClone(n->type);
            new_sym->tu = type;
            Ake_EnvironmentAdd(ps->st->top, &id_node->data.id.value, new_sym);
        }
    }
}

void Ake_UpdateSymbolFor(Ake_parse_state* ps, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Ast* dec = Ake_AstGet(n, 0);
        if (!dec->has_error) {
            Ake_Ast* id_node = Ake_AstGet(dec, 0);
            Ake_Ast* type_node = Ake_AstGet(dec, 1);
            Ake_declare_type(ps, type_node, id_node, true);
        }
    }
}

void Ake_UpdateSymbolStruct(Ake_parse_state* ps, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Type* type = Ake_StructToType(n);
        Ake_symbol* sym = NULL;
        Ake_SymbolCreate(&sym);
        sym->kind = AKE_SYMBOL_TYPE;
        sym->td = type;
        Ake_EnvironmentAdd(ps->st->top, &n->struct_value, sym);
    }
}

void Ake_UpdateSymbolLet(Ake_parse_state* ps, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Ast* id_node = Ake_AstGet(n, 0);
        Ake_Ast* type_node = Ake_AstGet(n, 1);
        Ake_declare_type(ps, type_node, id_node, n->kind == Ake_ast_type_const);
    }
}

void Ake_UpdateSymbolPrototype(Ake_parse_state* ps, Ake_Ast* n)
{
    Ake_Ast* proto = Ake_AstGet(n, 0);
    Ake_declare_params(ps, proto);
}

void Ake_UpdateSymbolFunction(Ake_parse_state* ps, Ake_Ast* n)
{
    if (!n->has_error) {
        if (!n->parent || (n->parent->kind != Ake_ast_type_struct)) {
            Ake_Ast* proto = Ake_AstGet(n, 0);
            Ake_Ast* id_node = Ake_AstGet(proto, 0);
            struct Ake_Symbol* new_sym = NULL;
            Zinc_malloc_safe((void**)&new_sym, sizeof(struct Ake_Symbol));
            Ake_SymbolInit(new_sym);
            new_sym->kind = AKE_SYMBOL_VARIABLE;
            new_sym->tu = Ake_TypeClone(n->type);
            Ake_EnvironmentAdd(ps->st->top, &id_node->data.id.value, new_sym);
        }
    }
}
