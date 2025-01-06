#include "parse_data.h"
#include "ast.h"
#include <assert.h>
#include "parse_tools.h"

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
    Cent_ast* p = n->head;
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &p->text);
    assert(sym);

    if (sym->type == Cent_symbol_type_enumerate) {
        Cent_enum_type* en = sym->data.enumerate;
        Cent_ast* id1 = p;
        Cent_ast* id2 = p->next;
        Cent_update_values_enum(pr, n, en, id1, id2);
    } else if (sym->type == Cent_symbol_type_module) {
        Cent_module* mod = sym->data.module;
        p = p->next;
        Cent_module* mod2 = mod;
        while (p) {
            Cent_symbol* sym2 = NULL;
            if (mod2->env) {
                sym2 = Cent_environment_get(mod2->env, &p->text);
            }
            mod2 = Cent_module_get(mod2, &p->text);

            if (sym2) {
                if (mod2) {
                    error_list_set(pr->errors, &p->loc, "ambiguous namespace: %b", &p->text);
                    n->has_error = true;
                    break;
                }

                if (sym2->type == Cent_symbol_type_enumerate) {
                    Cent_enum_type* en = sym2->data.enumerate;
                    Cent_ast* id1 = p;
                    Cent_ast* id2 = p->next;
                    Cent_update_values_enum(pr, n, en, id1, id2);
                    break;
                }
            }

            if (!mod2) {
                error_list_set(pr->errors, &p->loc, "could not find submodule: %b", &p->text);
                n->has_error = true;
                break;
            }

            p = p->next;
        }
    } else {
        assert(false && "not possible");
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
    buffer_copy(&id1->text, &n->data.enumeration.id1);
    buffer_copy(&id1->text, &n->data.enumeration.display);
    n->data.enumeration.loc1 = id1->loc;

    if (!id2) {
        error_list_set(pr->errors, &n->loc, "missing enum id");
        n->has_error = true;
        return;
    }

    bool found = false;
    Cent_enumerate_value* v = en->head;
    while (v) {
        if (buffer_compare(&v->display, &id2->text)) {
            buffer_copy(&id2->text, &n->data.enumeration.id2);
            buffer_copy_str(&n->data.enumeration.display, "::");
            buffer_copy(&id2->text, &n->data.enumeration.display);
            n->data.enumeration.number = v->value;
            n->data.enumeration.loc2 = id2->loc;
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