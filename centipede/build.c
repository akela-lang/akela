#include "parse_data.h"
#include "value.h"
#include <assert.h>

#include "parse_tools.h"

Cent_value* Cent_build_dispatch(Cent_ast* n);
Cent_value* Cent_build_number(Cent_ast* n);
Cent_value* Cent_build_string(Cent_ast* n);
Cent_value* Cent_build_boolean(Cent_ast* n);
Cent_value* Cent_build_enum(Cent_ast* n);
Cent_value* Cent_build_assign(Cent_ast* n);
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
        Cent_ast* last = pr->root->tail;
        if (last) {
            value = Cent_build_dispatch(last);
        }
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

    if (n->type == Cent_ast_type_expr_enum) {
        return Cent_build_enum(n);
    }

    if (n->type == Cent_ast_type_expr_assign) {
        return Cent_build_assign(n);
    }

    if (n->type == Cent_ast_type_expr_function_top) {
        return Cent_build_function_top(n);
    }

    if (n->type == Cent_ast_type_expr_function_file_name) {
        return Cent_build_function_file_name(n);
    }

    if (n->type == Cent_ast_type_expr_object) {
        return Cent_build_object(n);
    }

    return NULL;
}

Cent_value* Cent_build_number(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_number);

    if (n->number_type == Cent_number_type_integer) {
        value->number_type = Cent_number_type_integer;
        value->data.integer = n->data.integer;
    } else if (n->number_type == Cent_number_type_fp) {
        value->number_type = Cent_number_type_fp;
        value->data.fp = n->data.fp;
    } else {
        assert(false && "not possible");
    }

    return value;
}

Cent_value* Cent_build_string(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_string);
    buffer_copy(&n->data.string, &value->data.string);
    return value;
}

Cent_value* Cent_build_boolean(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_boolean);
    value->data.boolean = n->data.boolean;
    return value;
}

Cent_value* Cent_build_enum(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_enum);
    buffer_copy(&n->data.enumeration.id1, &value->data.enumeration.id1);
    buffer_copy(&n->data.enumeration.id2, &value->data.enumeration.id2);
    buffer_copy(&n->data.enumeration.display, &value->data.enumeration.display);
    value->data.enumeration.number = n->data.enumeration.number;
    return value;
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
    return NULL;
}

Cent_value* Cent_build_function_top(Cent_ast* n)
{
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get_str(top, "#object_value#");
    assert(sym);
    assert(sym->type == Cent_symbol_type_object_value);
    return sym->data.object_value;
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
    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_object(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_object);

    Cent_ast* stmts = Cent_ast_get(n, 0);
    assert(stmts->type == Cent_ast_type_object_stmts);
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
            Cent_value_add(value, value2);
        }
        p = p->next;
    }

    Cent_symbol* sym = NULL;
    Cent_symbol_create(&sym);
    sym->type = Cent_symbol_type_object_value;
    sym->data.object_value = value;
    Cent_environment* top = Cent_get_environment(stmts);
    Cent_environment_add_symbol_str(top, "#object_value#", sym);

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
    Cent_ast* id = Cent_ast_get(a, 0);
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &id->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    Cent_value_add(sym->data.variable.value, value);
}

void Cent_build_object_method_property_of(Cent_ast* n, Cent_value* value)
{
    Cent_ast* a = Cent_ast_get(n, 0);
    assert(a->type == Cent_ast_type_expr_variable);
    Cent_ast* id = Cent_ast_get(a, 0);
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &id->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    Cent_value_set(sym->data.variable.value, &id->text, value);
}