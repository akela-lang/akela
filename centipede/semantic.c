#include "parse_data.h"
#include "akela/environment.h"
#include <assert.h>

void Cent_validate_stmts(Cent_parse_result* pr, Cent_ast* n);
void Cent_validate_et(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env);
void Cent_validate_prop(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);
void Cent_validate_child(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);

void Cent_validate(Cent_parse_result* pr)
{
    if (pr->root && pr->root->type == Cent_ast_type_stmts) {
        Cent_validate_stmts(pr, pr->root);
    }
}

void Cent_validate_stmts(Cent_parse_result* pr, Cent_ast* n)
{
    Cent_environment* env = n->env;
    Cent_ast* stmt = n->head;
    while (stmt) {
        if (stmt->type == Cent_ast_type_element_type) {
            if (!stmt->has_error) {
                Cent_validate_et(pr, stmt, env);
            }
        }
        stmt = stmt->next;
    }
}

void Cent_validate_et(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env)
{
    Cent_symbol* sym = Cent_environment_get(env, &n->text);
    Cent_element_type* et = sym->data.element;

    Cent_ast* p = n->head;
    while (p) {
        if (p->type == Cent_ast_type_prop) {
            Cent_ast* prop_dec = p->head;
            while (prop_dec) {
                Cent_validate_prop(pr, prop_dec, et, env);
                prop_dec = prop_dec->next;
            }
        } else if (p->type == Cent_ast_type_children) {
            Cent_ast* child = p->head;
            while (child) {
                Cent_validate_child(pr, child, et, env);
                child = child->next;
            }
        } else {
            assert(false && "unexpected type");
        }
        p = p->next;
    }
}

void Cent_validate_prop(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env)
{
    Cent_ast* name = Cent_ast_get(n, 0);
    Cent_ast* type = Cent_ast_get(n, 1);
    Cent_ast* modifier = Cent_ast_get(n, 2);

    Cent_property_type* prop = NULL;
    Cent_property_type_create(&prop);
    buffer_copy(&name->text, &prop->name);
    Cent_symbol* sym = Cent_environment_get(env, &type->text);
    if (!sym) {
        error_list_set(pr->errors, &n->loc, "unknown type: %b", &type->text);
        n->has_error = true;
        /* test case: test_parse_element_property_unknown_type */
    } else {
        if (sym->type == Cent_symbol_type_element) {
            prop->et = sym->data.element;
        } else {
            error_list_set(pr->errors, &n->loc, "type is not an element type: %b", &type->text);
            n->has_error = true;
            /* test case: test_parse_element_property_type_not_element */
        }

        if (modifier && buffer_compare_str(&modifier->text, "required")) {
            prop->required = true;
        }
    }

    hash_table_add(&et->properties, &name->text, prop);
}

void Cent_validate_child(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env)
{
    Cent_symbol* sym = Cent_environment_get(env, &n->text);
    if (!sym) {
        error_list_set(pr->errors, &n->loc, "unknown type: %b", &n->text);
        n->has_error = true;
        /* test case: test_parse_element_child_unknown_type */
    } else {
        if (sym->type == Cent_symbol_type_element) {
            list_add_item(&et->children, sym->data.element);
        } else {
            error_list_set(pr->errors, &n->loc, "type is not an element type: %b", &n->text);
            /* test case: test_parse_element_child_type_not_an_element_type */
            n->has_error = true;
        }
    }
}