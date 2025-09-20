#include "ast.h"
#include "zinc/memory.h"
#include "parse_tools.h"
#include "parse_types.h"

void Ake_UpdateSymbolExtern(Ake_symbol_table* st, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Ast* proto = n->data._extern_.proto;
        if (proto) {
            Ake_Ast *id_node = proto->data.prototype.id;
            struct Ake_Symbol *new_sym = NULL;
            Zinc_malloc_safe((void **) &new_sym, sizeof(struct Ake_Symbol));
            Ake_SymbolInit(new_sym);
            new_sym->kind = AKE_SYMBOL_VARIABLE;
            Ake_Type* type = Ake_TypeClone(n->type);
            new_sym->tu = type;
            Ake_EnvironmentAdd(st->top, &id_node->data.id.value, new_sym);
        }
    }
}

void Ake_UpdateSymbolFor(Ake_symbol_table* st, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Ast* dec = NULL;
        if (n->kind == AKE_AST_FOR_RANGE) {
            dec = n->data.for_range.dec;
        } else if (n->kind == AKE_AST_FOR_ITERATION) {
            dec = n->data.for_iteration.dec;
        } else {
            return;
        }

        if (!dec->has_error) {
            Ake_Ast* id_node = dec->data.declaration.id_node;
            Ake_Ast* type_node = dec->data.declaration.type_node;
            Ake_declare_type(st, type_node, id_node, true);
        }
    }
}

void Ake_UpdateSymbolStruct(Ake_symbol_table* st, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Type* type = Ake_StructToType(n);
        Ake_symbol* sym = NULL;
        Ake_SymbolCreate(&sym);
        sym->kind = AKE_SYMBOL_TYPE;
        sym->td = type;
        Ake_EnvironmentAdd(st->top, &n->data._struct_.name, sym);
    }
}

void Ake_UpdateSymbolLet(Ake_symbol_table* st, Ake_Ast* n)
{
    if (!n->has_error) {
        Ake_Let* data = &n->data._const_;
        Ake_Ast* id_node = data->id;
        Ake_Ast* type_node = data->type_node;
        Ake_declare_type(st, type_node, id_node, n->kind == AKE_AST_CONST);
    }
}

void Ake_UpdateSymbolPrototype(Ake_symbol_table* st, Ake_Ast* n)
{
    Ake_Ast* proto = n->data.function.proto;
    Ake_declare_params(st, proto);
}

void Ake_UpdateSymbolFunction(Ake_symbol_table* st, Ake_Ast* n)
{
    if (!n->has_error) {
        if (!n->parent || (n->parent->kind != AKE_AST_STRUCT)) {
            Ake_Ast* proto = n->data.function.proto;
            Ake_Ast* id_node = proto->data.prototype.id;
            struct Ake_Symbol* new_sym = NULL;
            Zinc_malloc_safe((void**)&new_sym, sizeof(struct Ake_Symbol));
            Ake_SymbolInit(new_sym);
            new_sym->kind = AKE_SYMBOL_VARIABLE;
            new_sym->tu = Ake_TypeClone(n->type);
            Ake_EnvironmentAdd(st->top, &id_node->data.id.value, new_sym);
        }
    }
}
