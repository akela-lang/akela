#include "parse_data.h"
#include "akela/environment.h"
#include <assert.h>
#include "zinc/hash.h"

void Cent_update_stmts(Cent_parse_result* pr, Cent_ast* n);
void Cent_update_et(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env);
void Cent_update_prop(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);
void Cent_update_child(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env);
void Cent_update_enum(Cent_parse_result* pr, Cent_ast* n, Cent_environment* env);

void Cent_circular_dep_element_type(Cent_parse_result* pr, Cent_element_type* et);
void Cent_circular_dep_element_type2(
    Cent_parse_result* pr,
    Cent_element_type* et,
    struct hash_table* ht,
    struct buffer* trace);
void Cent_circular_dep_element_type_parts(
    Cent_parse_result* pr,
    Cent_element_type* et,
    struct hash_table* ht,
    struct buffer* trace);
void Cent_circular_dep_property_type(Cent_property_type* prop);

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

    prop->loc = n->loc;
    prop->has_error = n->has_error;

    Cent_element_set(et, &name->text, prop);
    Cent_property_type* prop2 = Cent_element_get(et, &name->text);
}

void Cent_update_child(Cent_parse_result* pr, Cent_ast* n, Cent_element_type* et, Cent_environment* env)
{
    Cent_symbol* sym = Cent_environment_get(env, &n->text);
    if (!sym) {
        error_list_set(pr->errors, &n->loc, "unknown type: %b", &n->text);
        n->has_error = true;
        /* test case: test_parse_element_child_unknown_type */
    } else {
        if (sym->type == Cent_symbol_type_element) {
            Cent_element_add_et(et, sym->data.element, &n->loc, n->has_error);
        } else {
            error_list_set(pr->errors, &n->loc, "type is not an element type: %b", &n->text);
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
    Cent_enum_type* enumerate = sym->data.enumerate;
    Cent_ast* p = n->head;
    while (p) {
        Cent_enumerate_value* ev = Cent_enumerate_get(enumerate, &p->text);
        if (ev) {
            error_list_set(pr->errors, &p->loc, "duplicate enum value: %b::%b", &n->text, &p->text);
            n->has_error = true;
        } else {
            Cent_enumerate_add_name(enumerate, &p->text);
        }
        p = p->next;
    }
}

void Cent_circular_dep(Cent_parse_result* pr)
{
    if (pr->root && pr->root->type == Cent_ast_type_stmts) {
        Cent_environment* env = pr->root->env;
        Cent_ast* stmt = pr->root->head;
        while (stmt) {
            if (stmt->type == Cent_ast_type_element_type) {
                if (!stmt->has_error) {
                    Cent_symbol* sym = Cent_environment_get(env, &stmt->text);
                    assert(sym);
                    assert(sym->type == Cent_symbol_type_element);
                    Cent_element_type* et = sym->data.element;
                    Cent_circular_dep_element_type(pr, et);
                }
            }
            stmt = stmt->next;
        }
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_circular_dep_element_type(Cent_parse_result* pr, Cent_element_type* et)
{
    struct hash_table ht;
    hash_table_init(&ht, 32);

    struct buffer trace;
    buffer_init(&trace);

    Cent_circular_dep_element_type2(pr, et, &ht, &trace);

    hash_table_destroy(&ht);
    buffer_destroy(&trace);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_circular_dep_element_type2(
    Cent_parse_result* pr,
    Cent_element_type* et,
    struct hash_table* ht,
    struct buffer* trace)
{
    hash_table_add(ht, &et->name, et);
    if (trace->size > 0) {
        buffer_copy_str(trace, "--");
    }
    buffer_copy(&et->name, trace);
    Cent_circular_dep_element_type_parts(pr, et, ht, trace);
}

Cent_parse_result* Cent_circular_dep_pr = NULL;
struct hash_table* Cent_circular_dep_ht = NULL;
Cent_element_type* Cent_circular_dep_et = NULL;
struct buffer* Cent_circular_dep_trace = NULL;

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_circular_dep_element_type_parts(
    Cent_parse_result* pr,
    Cent_element_type* et,
    struct hash_table* ht,
    struct buffer* trace)
{
    Cent_circular_dep_pr = pr;
    Cent_circular_dep_ht = ht;
    Cent_circular_dep_et = et;
    Cent_circular_dep_trace = trace;

    hash_table_map(&et->properties, (hash_table_func)Cent_circular_dep_property_type);

    Cent_element_type_node* node = et->children.head;
    while (node) {
        Cent_element_type* et2 = hash_table_get(ht, &node->et->name);

        if (et2) {
            if (trace->size > 0) {
                buffer_copy_str(trace, "--");
            }
            buffer_copy(&node->et->name, trace);

            error_list_set(pr->errors, &node->loc, "circular reference: %b", trace);
            node->has_error = true;
            return;
        }
        Cent_circular_dep_element_type2(pr, node->et, ht, trace);
        node = node->next;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_circular_dep_property_type(Cent_property_type* prop)
{
    if (!prop->has_error) {
        if (Cent_circular_dep_trace->size > 0) {
            buffer_copy_str(Cent_circular_dep_trace, "--");
        }
        buffer_copy(&prop->name, Cent_circular_dep_trace);

        Cent_element_type* et = hash_table_get(Cent_circular_dep_ht, &prop->et->name);
        if (et) {
            buffer_copy_str(Cent_circular_dep_trace, "--");
            buffer_copy(&et->name, Cent_circular_dep_trace);

            error_list_set(
                Cent_circular_dep_pr->errors,
                &prop->loc,
                "circular reference: %b",
                Cent_circular_dep_trace);
            return;
        }
        Cent_circular_dep_element_type2(
            Cent_circular_dep_pr,
            prop->et,
            Cent_circular_dep_ht,
            Cent_circular_dep_trace);
    }
}