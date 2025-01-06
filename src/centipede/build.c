#include "parse_data.h"
#include "value.h"
#include <assert.h>

#include "check_types.h"
#include "parse_tools.h"

Cent_value* Cent_build_dispatch(Cent_ast* n);
Cent_value* Cent_build_number(Cent_ast* n);
Cent_value* Cent_build_string(Cent_ast* n);
Cent_value* Cent_build_boolean(Cent_ast* n);
Cent_value* Cent_build_namespace(Cent_ast* n);
Cent_value* Cent_build_assign(Cent_ast* n);
Cent_value* Cent_build_variable(Cent_ast* n);
Cent_value* Cent_build_function_top(Cent_ast* n);
Cent_value* Cent_build_function_file_name(Cent_ast* n);
Cent_value* Cent_build_object(Cent_ast* n);
void Cent_build_object_prop_set(Cent_ast* n, Cent_value* value);
void Cent_build_object_method_child_of(Cent_ast* n, Cent_value* value);
void Cent_build_object_method_property_of(Cent_ast* n, Cent_value* value);

Cent_value* Cent_build(Cent_parse_result* pr)
{
    Cent_value* value = NULL;

    if (!pr->errors->head) {
        Cent_ast* p = pr->root->head;
        while (p) {
            if (p->type == Cent_ast_type_expr_assign || p == pr->root->tail) {
                value = Cent_build_dispatch(p);
            }
            p = p->next;
        }
    }

    if (!pr->errors->head) {
        Cent_check_types(pr, value);
    }

    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_dispatch(Cent_ast* n)
{
    if (n->type == Cent_ast_type_expr_number) {
        return Cent_build_number(n);
    }

    if (n->type == Cent_ast_type_expr_string) {
        return Cent_build_string(n);
    }

    if (n->type == Cent_ast_type_expr_boolean) {
        return Cent_build_boolean(n);
    }

    if (n->type == Cent_ast_type_namespace) {
        return Cent_build_namespace(n);
    }

    if (n->type == Cent_ast_type_expr_assign) {
        return Cent_build_assign(n);
    }

    if (n->type == Cent_ast_type_expr_function_file_name) {
        return Cent_build_function_file_name(n);
    }

    if (n->type == Cent_ast_type_expr_object) {
        return Cent_build_object(n);
    }

    if (n->type == Cent_ast_type_expr_variable) {
        return Cent_build_variable(n);
    }

    return NULL;
}

Cent_value* Cent_build_number(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_number);

    if (n->number_type == Cent_number_type_integer) {
        buffer_copy_str(&value->name, "Integer");
        value->number_type = Cent_number_type_integer;
        value->data.integer = n->data.integer;
    } else if (n->number_type == Cent_number_type_fp) {
        buffer_copy_str(&value->name, "Float");
        value->number_type = Cent_number_type_fp;
        value->data.fp = n->data.fp;
    } else {
        assert(false && "not possible");
    }

    value->n = n;
    return value;
}

Cent_value* Cent_build_string(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_string);
    buffer_copy(&n->data.string, &value->data.string);
    buffer_copy_str(&value->name, "String");
    value->n = n;
    return value;
}

Cent_value* Cent_build_boolean(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_boolean);
    value->data.boolean = n->data.boolean;
    buffer_copy_str(&value->name, "Bool");
    value->n = n;
    return value;
}

Cent_value* Cent_build_namespace(Cent_ast* n)
{
    if (n->value_type == Cent_value_type_enum) {
        Cent_value* value = NULL;
        Cent_value_create(&value);
        Cent_value_set_type(value, Cent_value_type_enum);
        buffer_copy(&n->data.enumeration.id1, &value->data.enumeration.id1);
        buffer_copy(&n->data.enumeration.id2, &value->data.enumeration.id2);
        buffer_copy(&n->data.enumeration.display, &value->data.enumeration.display);
        value->data.enumeration.number = n->data.enumeration.number;
        buffer_copy(&n->data.enumeration.id1, &value->name);
        value->n = n;
        return value;
    } else {
        assert(false && "not implemented");
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_assign(Cent_ast* n)
{
    Cent_ast* a = Cent_ast_get(n, 0);
    Cent_ast* b = Cent_ast_get(n, 1);
    Cent_value* value = Cent_build_dispatch(b);
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &a->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    sym->data.variable.value = value;
    value->n = n;
    return NULL;
}

Cent_value* Cent_build_variable(Cent_ast* n)
{
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &n->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    return sym->data.variable.value;
}

Cent_value* Cent_build_function_file_name(Cent_ast* n)
{
    Cent_environment* top = Cent_get_environment(n);
    assert(top);
    Cent_symbol* sym = Cent_environment_get_str(top, "#file_name#");
    assert(sym);
    assert(sym->type == Cent_symbol_type_file_name);
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_string);
    buffer_add(&value->data.string, sym->data.file_name.p, sym->data.file_name.size);
    buffer_copy_str(&value->name, "String");
    value->n = n;
    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_object(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_object);
    buffer_copy(&n->text, &value->name);

    Cent_ast* stmts = Cent_ast_get(n, 0);
    assert(stmts->type == Cent_ast_type_object_stmts);

    Cent_symbol* sym = NULL;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_object_value);
    sym->data.object_value = value;
    Cent_environment* top = Cent_get_environment(stmts);
    Cent_environment_add_symbol_str(top, "#object_value#", sym);

    Cent_ast* p = stmts->head;
    while (p) {
        if (p->type == Cent_ast_type_prop_set) {
            Cent_build_object_prop_set(p, value);
        } else if (p->type == Cent_ast_type_method_child_of) {
            Cent_build_object_method_child_of(p, value);
        } else if (p->type == Cent_ast_type_method_property_of) {
            Cent_build_object_method_property_of(p, value);
        } else {
            Cent_value* value2 = Cent_build_dispatch(p);
            if (value2) {
                Cent_value_add(value, value2);
            }
        }
        p = p->next;
    }

    value->n = n;

    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Cent_build_object_prop_set(Cent_ast* n, Cent_value* value)
{
    Cent_ast* id = Cent_ast_get(n, 0);
    Cent_ast* expr = Cent_ast_get(n, 1);
    Cent_value* expr_value = Cent_build_dispatch(expr);
    Cent_value_set(value, &id->text, expr_value);
}

void Cent_build_object_method_child_of(Cent_ast* n, Cent_value* value)
{
    Cent_ast* a = Cent_ast_get(n, 0);
    assert(a->type == Cent_ast_type_expr_variable);
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &a->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    Cent_value_add(sym->data.variable.value, value);
}

void Cent_build_object_method_property_of(Cent_ast* n, Cent_value* value)
{
    Cent_ast* a = Cent_ast_get(n, 0);
    assert(a->type == Cent_ast_type_expr_variable);
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &a->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    Cent_value* object = sym->data.variable.value;

    Cent_ast* b = Cent_ast_get(n, 1);
    assert(b->type == Cent_ast_type_expr_string);
    assert(b->data.string.size > 0);

    Cent_value_set(object, &b->data.string, value);
}