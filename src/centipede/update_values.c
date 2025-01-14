#include "parse_data.h"
#include "ast.h"
#include "parse_tools.h"
#include <assert.h>

void Cent_update_values_ast(Cent_parse_result* pr, Cent_ast* n);
void Cent_update_values_namespace(Cent_parse_result* pr, Cent_ast* n);
void Cent_update_values_enum(
    Cent_parse_result* pr,
    Cent_ast* n,
    Cent_enum_type* en,
    Cent_ast* id1,
    Cent_ast* id2);

void Cent_update_values(Cent_parse_result* pr)
{
    if (pr->root) {
        Cent_update_values_ast(pr, pr->root);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_update_values_ast(Cent_parse_result* pr, Cent_ast* n)
{
    Cent_ast* p = n->head;
    while (p) {
        Cent_update_values_ast(pr, p);
        p = p->next;
    }

    if (n->type == Cent_ast_type_namespace) {
        Cent_update_values_namespace(pr, n);
    }
}

void Cent_update_values_namespace(Cent_parse_result* pr, Cent_ast* n)
{
    if (!n->has_error) {
        Cent_namespace_result nr = Cent_namespace_lookup(n);
        assert(nr.sym);
        assert(nr.node);
        if (nr.sym->type == Cent_symbol_type_enumerate) {
            Cent_enum_type* en = nr.sym->data.enumerate;
            Cent_ast* id1 = nr.node;
            Cent_ast* id2 = id1->next;
            Cent_update_values_enum(pr, n, en, id1, id2);
        }
    }
}

void Cent_update_values_enum(
    Cent_parse_result* pr,
    Cent_ast* n,
    Cent_enum_type* en,
    Cent_ast* id1,
    Cent_ast* id2)
{
    Cent_ast_value_set_type(n, Cent_value_type_enum);
    n->data.enumeration.enum_type = en;

    if (!id2) {
        error_list_set(pr->errors, &n->loc, "missing enum id");
        n->has_error = true;
        return;
    }

    bool found = false;
    Cent_enum_value* v = en->head;
    while (v) {
        if (Zinc_string_compare(&v->display, &id2->text)) {
            n->data.enumeration.enum_value = v;
            n->data.enumeration.number = v->value;
            found = true;
        }
        v = v->next;
    }

    if (!found) {
        error_list_set(pr->errors, &id2->loc, "could not find enum id: %b", &id2->text);
        n->has_error = true;
    }

    Cent_ast* p = id2->next;
    if (p) {
        error_list_set(pr->errors, &p->loc, "invalid namespace");
        n->has_error = true;
    }
}