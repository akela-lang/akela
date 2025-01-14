#include "parse_data.h"
#include "akela/environment.h"
#include <assert.h>
#include "zinc/hash_map_string.h"

void Cent_update_stmts(Cent_parse_result* pr, Cent_ast* n);
void Cent_update_et(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env);
void Cent_update_prop(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);
void Cent_update_child(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);
void Cent_update_enum(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env);

void Cent_update_types(Cent_parse_result* pr)
{
    if (pr->root && pr->root->type == Cent_ast_type_stmts) {
        Cent_update_stmts(pr, pr->root);
    }
}

void Cent_update_stmts(Cent_parse_result* pr, Cent_ast* n)
{
    Cent_environment* env = n->env;
    Cent_ast* stmt = n->head;
    while (stmt) {
        if (stmt->type == Cent_ast_type_element_type) {
            if (!stmt->has_error) {
                Cent_update_et(pr, stmt, env);
            }
        } else if (stmt->type == Cent_ast_type_enum_type) {
            if (!stmt->has_error) {
                Cent_update_enum(pr, stmt, env);
            }
        }
        stmt = stmt->next;
    }
}

void Cent_update_et(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env)
{
    Cent_symbol* sym = Cent_environment_get(env, &n->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_element);
    Cent_element_type* et = sym->data.element;

    Cent_ast* p = n->head;
    while (p) {
        if (p->type == Cent_ast_type_prop) {
            Cent_ast* prop_dec = p->head;
            while (prop_dec) {
                Cent_update_prop(pr, prop_dec, et, env);
                prop_dec = prop_dec->next;
            }
        } else if (p->type == Cent_ast_type_children) {
            Cent_ast* child = p->head;
            while (child) {
                Cent_update_child(pr, child, et, env);
                child = child->next;
            }
        } else {
            assert(false && "unexpected type");
        }
        p = p->next;
    }
}

void Cent_update_prop(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env)
{
    Cent_ast* name = Cent_ast_get(n, 0);
    Cent_ast* type = Cent_ast_get(n, 1);
    Cent_ast* modifier = Cent_ast_get(n, 2);

    Cent_property_type* prop = NULL;
    Cent_property_type_create(&prop);
    Zinc_string_copy(&name->text, &prop->name);
    Cent_symbol* sym = Cent_environment_get(env, &type->text);
    if (!sym) {
        Zinc_error_list_set(pr->errors, &n->loc, "unknown type: %b", &type->text);
        n->has_error = true;
        /* test case: test_parse_element_property_unknown_type */
    } else {
        if (sym->type == Cent_symbol_type_element) {
            Cent_property_type_set_type(prop, Cent_types_element);
            prop->data.et = sym->data.element;
        } else if (sym->type == Cent_symbol_type_enumerate) {
            Cent_property_type_set_type(prop, Cent_types_enum);
            prop->data.en = sym->data.enumerate;
        } else {
            Zinc_error_list_set(pr->errors, &n->loc, "type is not an element or enum type: %b", &type->text);
            n->has_error = true;
            /* test case: test_parse_element_property_type_not_element */
        }

        if (modifier && Zinc_string_compare_str(&modifier->text, "required")) {
            prop->required = true;
        }
    }

    prop->loc = n->loc;
    prop->has_error = n->has_error;

    Cent_element_set(et, &name->text, prop);
    Cent_property_type* prop2 = Cent_element_get(et, &name->text);
}

void Cent_update_child(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env)
{
    Cent_symbol* sym = Cent_environment_get(env, &n->text);
    if (!sym) {
        Zinc_error_list_set(pr->errors, &n->loc, "unknown type: %b", &n->text);
        n->has_error = true;
        /* test case: test_parse_element_child_unknown_type */
    } else {
        if (sym->type == Cent_symbol_type_element) {
            Cent_element_add_et(et, sym->data.element, &n->loc, n->has_error);
        } else if (sym->type == Cent_symbol_type_enumerate) {
            Cent_element_add_en(et, sym->data.enumerate, &n->loc, n->has_error);
        } else {
            Zinc_error_list_set(pr->errors, &n->loc, "type is not an element type: %b", &n->text);
            /* test case: test_parse_element_child_type_not_an_element_type */
            n->has_error = true;
        }
    }
}

void Cent_update_enum(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env)
{
    Cent_symbol* sym = Cent_environment_get(env, &n->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_enumerate);
    Cent_enum_type* en = sym->data.enumerate;
    en->loc = n->loc;
    Cent_ast* p = n->head;
    while (p) {
        Cent_enum_value* ev = Cent_enumerate_get(en, &p->text);
        if (ev) {
            Zinc_error_list_set(pr->errors, &p->loc, "duplicate enum value: %b::%b", &n->text, &p->text);
            n->has_error = true;
            p->has_error = true;
            /* test case: test_parse_enum_duplicate_id */
        } else {
            Cent_enumerate_add_name(en, &p->text, &p->loc);
        }
        p = p->next;
    }
}