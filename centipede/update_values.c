#include "parse_data.h"
#include "ast.h"
#include <assert.h>
#include "parse_tools.h"

void Cent_update_values_ast(Cent_parse_result* pr, Cent_ast* n);
void Cent_update_values_enum(Cent_parse_result* pr, Cent_ast* n);

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

    if (n->type == Cent_ast_type_expr_enum) {
        Cent_update_values_enum(pr, n);
    }
}

void Cent_update_values_enum(Cent_parse_result* pr, Cent_ast* n)
{
    if (!n->has_error) {
        Cent_environment* top = Cent_get_environment(n);
        Cent_symbol* sym = Cent_environment_get(top, &n->data.enumeration.id1);
        if (!sym) {
            error_list_set(
                pr->errors,
                &n->data.enumeration.loc1,
                "could not find enum: %b",
                &n->data.enumeration.id1);
            n->has_error = true;
            /* test case: test_parse_enum_error_could_not_find_enum */
        } else {
            Cent_enum_type* en = sym->data.enumerate;
            assert(en);
            bool found = false;
            Cent_enumerate_value* enum_value = en->head;
            while (enum_value) {
                if (buffer_compare(&enum_value->display, &n->data.enumeration.id2)) {
                    n->data.enumeration.number = enum_value->value;
                    found = true;
                    break;
                }
                enum_value = enum_value->next;
            }
            if (!found) {
                error_list_set(
                    pr->errors,
                    &n->data.enumeration.loc2,
                    "could not find enum id: %b",
                    &n->data.enumeration.id2);
                n->has_error = true;
                /* test case: test_parse_enum_error_could_not_find_enum_id */
            }
        }
    }
}