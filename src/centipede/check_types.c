#include <stdbool.h>
#include "parse_data.h"
#include <assert.h>

#include "parse_tools.h"

bool Cent_check_enum(Cent_parse_result* pr, Cent_enum_type* en, Cent_ast* id1, Cent_ast* id2);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_check_namespace(Cent_parse_result* pr, Cent_ast* n)
{
    Cent_ast* p = n->head;
    assert(p);
    if (p->type != Cent_ast_type_id) {
        Zinc_error_list_set(pr->errors, &p->loc, "expected id");
        n->has_error = true;
    }
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &p->text);
    if (!sym) {
        Zinc_error_list_set(
            pr->errors,
            &p->loc,
            "id is not a variable: %b",
            &p->text);
        n->has_error = true;
    } else if (sym->type == Cent_symbol_type_enumerate) {
        Cent_enum_type* en = sym->data.enumerate;
        Cent_ast* id1 = p;
        Cent_ast* id2 = p->next;
        Cent_check_enum(pr, en, id1, id2);
    } else if (sym->type == Cent_symbol_type_module) {
        Cent_module* mod = sym->data.module;
        p = p->next;
        while (p) {
            Cent_module* mod2 = Cent_module_get(mod, &p->text);
            Cent_environment* top2 = mod->env;
            Cent_symbol* sym2 = Cent_environment_get(top2, &p->text);
            if (mod2 && sym2) {
                Zinc_error_list_set(
                    pr->errors,
                    &p->loc,
                    "ambiguous id (%b) is both a submodule and variable",
                    &p->text);
                n->has_error = true;
                break;
            }

            if (sym2 && sym2->type == Cent_symbol_type_enumerate) {
                Cent_enum_type* en = sym2->data.enumerate;
                Cent_ast* id1 = p;
                Cent_ast* id2 = p->next;
                if (!Cent_check_enum(pr, en, id1, id2)) {
                    n->has_error = true;
                }
                break;
            }

            if (sym2 && sym2->type != Cent_symbol_type_enumerate && p->next) {
                Zinc_error_list_set(pr->errors, &p->loc, "expected module or enum", &p->text);
                n->has_error = true;
                break;
            }

            if (!sym2 && !mod2) {
                Zinc_error_list_set(pr->errors, &p->loc, "not a valid id: %b", &p->text);
                n->has_error = true;
            }

            if (mod2) {
                mod = mod2;
            }

            assert(p->type == Cent_ast_type_id);
            p = p->next;
        }
    } else {
        Zinc_error_list_set(pr->errors,
            &p->loc,
            "namespace does not start with an enum or module");
        n->has_error = true;
    }
}

bool Cent_check_enum(Cent_parse_result* pr, Cent_enum_type* en, Cent_ast* id1, Cent_ast* id2)
{
    if (!id2) {
        Zinc_error_list_set(pr->errors, &id1->loc, "expecting enum id after enum");
        return false;
    }

    Cent_enum_value* val = en->head;
    bool found = false;
    while (val) {
        if (Zinc_string_compare(&val->display, &id2->text)) {
            found = true;
            break;
        }
        val = val->next;
    }
    if (!found) {
        Zinc_error_list_set(pr->errors, &id2->loc, "invalid enum id: %b", &id2->text);
        return false;
    }

    if (id2->next) {
        Zinc_error_list_set(
            pr->errors,
            &id2->next->loc,
            "namespace after enum id: %b",
            &id2->next->text);
        return false;
    }

    return true;
}

void Cent_check_variables(Cent_parse_result* pr, Cent_ast* n)
{
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &n->text);
    if (!sym) {
        Zinc_error_list_set(pr->errors, &n->loc, "unknown variable: %b", &n->text);
        n->has_error = true;
    }
}