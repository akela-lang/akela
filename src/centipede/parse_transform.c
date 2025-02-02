#include "parse_data.h"
#include "akela/environment.h"
#include <assert.h>
#include "parse_tools.h"
#include "zinc/hash_map_string.h"
#include "ast.h"
#include <assert.h>

void Cent_update_prop(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);
void Cent_update_child(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);

void Cent_update_element_type(Cent_parse_result* pr, Cent_ast* n)
{
    assert(n->type == Cent_ast_type_element_type);
    Cent_environment* env = Cent_get_environment(n);
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

void Cent_update_enum(Cent_parse_result* pr, Cent_ast* n)
{
    assert(n->type == Cent_ast_type_enum_type);
    Cent_environment* env = Cent_get_environment(n);
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

void Cent_update_enum_value(
    Cent_parse_result* pr,
    Cent_ast* n,
    Cent_enum_type* en,
    Cent_ast* id1,
    Cent_ast* id2);

void Cent_update_namespace(Cent_parse_result* pr, Cent_ast* n)
{
    if (!n->has_error) {
        Cent_namespace_result nr = Cent_namespace_lookup(n);
        assert(nr.sym);
        assert(nr.node);
        if (nr.sym->type == Cent_symbol_type_enumerate) {
            Cent_enum_type* en = nr.sym->data.enumerate;
            Cent_ast* id1 = nr.node;
            Cent_ast* id2 = id1->next;
            Cent_update_enum_value(pr, n, en, id1, id2);
        }
    }
}

void Cent_update_enum_value(
    Cent_parse_result* pr,
    Cent_ast* n,
    Cent_enum_type* en,
    Cent_ast* id1,
    Cent_ast* id2)
{
    Cent_ast_value_set_type(n, Cent_value_type_enum);
    n->data.enumeration.enum_type = en;

    if (!id2) {
        Zinc_error_list_set(pr->errors, &n->loc, "missing enum id");
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
        Zinc_error_list_set(pr->errors, &id2->loc, "could not find enum id: %b", &id2->text);
        n->has_error = true;
    }

    Cent_ast* p = id2->next;
    if (p) {
        Zinc_error_list_set(pr->errors, &p->loc, "invalid namespace");
        n->has_error = true;
    }
}