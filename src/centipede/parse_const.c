#include "ast.h"
#include "parse_tools.h"
#include "parse_expr.h"
#include "environment.h"

Cent_ast* Cent_parse_const(Cent_parse_data* pd)
{
    Cent_ast* n = NULL;
    Cent_ast_create(&n);
    n->type = Cent_ast_type_const;

    Cent_token* co = NULL;
    Cent_match(pd, Cent_token_const, "expected const", &co, n);
    Cent_token_destroy(co);
    free(co);

    Cent_ast* id_node = NULL;
    Cent_ast_create(&id_node);
    id_node->type = Cent_ast_type_id;

    Cent_token* id = NULL;
    Cent_match(pd, Cent_token_id, "expected id", &id, id_node);
    if (id) {
        Zinc_string_copy(&id->value, &id_node->text);
        Cent_token_destroy(id);
        free(id);
    }
    Cent_ast_add(n, id_node);

    Cent_token* eq = NULL;
    Cent_match(pd, Cent_token_equal, "expected equal", &eq, n);
    Cent_token_destroy(eq);
    free(eq);

    Cent_ast* a = Cent_parse_expr(pd);
    if (a) {
        Cent_ast_add(n, a);
    } else {
        Zinc_error_list_set(pd->errors, &pd->lookahead->loc, "expected expression");
        n->has_error = true;
    }

    if (!n->has_error) {
        Cent_symbol* sym = Cent_environment_get(pd->top, &id_node->text);
        if (sym) {
            if (sym->type == Cent_symbol_type_element || sym->type == Cent_symbol_type_enumerate) {
                Zinc_error_list_set(
                    pd->errors,
                    &id_node->loc,
                    "shadowing of type: %bf",
                    &id_node->text);
                n->has_error = true;
                /* test case: test_parse_const_error_shadow_type */
            } else if (sym->type == Cent_symbol_type_module) {
                Zinc_error_list_set(
                    pd->errors,
                    &id_node->loc,
                    "shadowing of module: %bf",
                    &id_node->text);
                n->has_error = true;
                /* test case: test_parse_const_error_shadow_module */
            } else {
                sym = Cent_environment_get_local(pd->top, &id_node->text);
                if (sym) {
                    Zinc_error_list_set(
                        pd->errors,
                        &id_node->loc,
                        "shadowing of local variable: %bf",
                        &id_node->text);
                    n->has_error = true;
                    /* test case: test_parse_const_error_shadow_local */
                }
            }
        }
    }

    if (!n->has_error) {
        Cent_symbol* sym = NULL;
        Cent_symbol_create(&sym);
        Cent_symbol_set_type(sym, Cent_symbol_type_variable);
        sym->data.variable.n = n;
        Cent_environment_add_symbol(pd->top, &id_node->text, sym);
    }

    return n;
}
