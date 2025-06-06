#include "parse_data.h"
#include "value.h"
#include <assert.h>

#include "check_value_types.h"
#include "parse_tools.h"
#include "build_data.h"

Cent_value* Cent_build_dispatch(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_integer(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_natural(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_real(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_string(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_boolean(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_namespace(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_const(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_variable(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_function_top(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_function_file_name(Cent_build_data* bd, Cent_ast* n);
Cent_value* Cent_build_object(Cent_build_data* bd, Cent_ast* n);
void Cent_build_object_prop_set(Cent_build_data* bd, Cent_ast* n, Cent_value* value);

Cent_value* Cent_build(Cent_parse_result* pr)
{
    Cent_value* value = NULL;
    Cent_build_data bd;
    Cent_build_data_init(&bd, pr->errors);

    if (!pr->errors->head) {
        Cent_ast* p = pr->root->head;
        while (p) {
            if (p->type == Cent_ast_type_const || p == pr->root->tail) {
                value = Cent_build_dispatch(&bd, p);
            }
            p = p->next;
        }
    }

    if (!pr->errors->head) {
        Cent_check_value_types(pr, value);
    }

    Cent_build_data_destroy(&bd);
    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_dispatch(Cent_build_data* bd, Cent_ast* n)
{
    if (Cent_ast_stack_has_node(bd->top, n)) {
        Zinc_error_list_set(bd->errors, &n->loc, "cycle detected");
        n->has_error = true;
        return NULL;
    }

    Cent_ast_stack_push(&bd->top, n);

    Cent_value* value = NULL;

    if (n->type == Cent_ast_type_expr_integer) {
        value = Cent_build_integer(bd, n);
    }

    if (n->type == Cent_ast_type_expr_natural) {
        value = Cent_build_natural(bd, n);
    }

    if (n->type == Cent_ast_type_expr_real) {
        value = Cent_build_real(bd, n);
    }

    if (n->type == Cent_ast_type_expr_string) {
        value = Cent_build_string(bd, n);
    }

    if (n->type == Cent_ast_type_expr_boolean) {
        value = Cent_build_boolean(bd, n);
    }

    if (n->type == Cent_ast_type_namespace) {
        value = Cent_build_namespace(bd, n);
    }

    if (n->type == Cent_ast_type_const) {
        //value = Cent_build_const(bd, n);
    }

    if (n->type == Cent_ast_type_expr_function_file_name) {
        value = Cent_build_function_file_name(bd, n);
    }

    if (n->type == Cent_ast_type_expr_object) {
        value = Cent_build_object(bd, n);
    }

    if (n->type == Cent_ast_type_expr_variable) {
        value = Cent_build_variable(bd, n);
    }

    Cent_ast_stack_pop(&bd->top);

    return value;
}

Cent_value* Cent_build_integer(Cent_build_data* bd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_integer);

    Zinc_string_add_str(&value->name, "Integer");
    value->data.integer = n->data.integer;

    value->n = n;
    return value;
}

Cent_value* Cent_build_natural(Cent_build_data* bd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_natural);

    Zinc_string_add_str(&value->name, "Natural");
    value->data.natural = n->data.natural;

    value->n = n;
    return value;
}

Cent_value* Cent_build_real(Cent_build_data* bd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_real);

    Zinc_string_add_str(&value->name, "Real");
    value->data.real = n->data.real;

    value->n = n;
    return value;
}

Cent_value* Cent_build_string(Cent_build_data* bd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_string);
    Zinc_string_copy(&n->data.string, &value->data.string);
    Zinc_string_add_str(&value->name, "String");
    value->n = n;
    return value;
}

Cent_value* Cent_build_boolean(Cent_build_data* bd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_boolean);
    value->data.boolean = n->data.boolean;
    Zinc_string_add_str(&value->name, "Bool");
    value->n = n;
    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_namespace(Cent_build_data* bd, Cent_ast* n)
{
    if (n->value_type == Cent_value_type_enum) {
        Cent_value* value = NULL;
        Cent_value_create(&value);
        Cent_value_set_type(value, Cent_value_type_enum);
        value->data.enumeration.enum_type = n->data.enumeration.enum_type;
        value->data.enumeration.enum_value = n->data.enumeration.enum_value;
        value->data.enumeration.number = n->data.enumeration.number;
        Zinc_string_copy(&n->data.enumeration.enum_type->name, &value->name);
        value->n = n;
        return value;
    }

    Cent_namespace_result nr = Cent_namespace_lookup(n);
    if (nr.sym) {
        Cent_symbol* sym = nr.sym;
        if (sym->type == Cent_symbol_type_variable) {
            Cent_ast* n = sym->data.variable.n;
            assert(n->type == Cent_ast_type_const);
            Cent_ast* expr = Cent_ast_get(n, 1);
            return Cent_build_dispatch(bd, expr);
        }
    }

    return NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_const(Cent_build_data* bd, Cent_ast* n)
{
    Cent_ast* a = Cent_ast_get(n, 0);
    Cent_ast* b = Cent_ast_get(n, 1);
    Cent_value* value = Cent_build_dispatch(bd, b);
    assert(value);
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &a->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    value->n = n;

    return NULL;
}

Cent_value* Cent_build_variable(Cent_build_data* bd, Cent_ast* n)
{
    Cent_environment* top = Cent_get_environment(n);
    Cent_symbol* sym = Cent_environment_get(top, &n->text);
    assert(sym);
    assert(sym->type == Cent_symbol_type_variable);
    Cent_ast* expr = Cent_ast_get(sym->data.variable.n, 1);
    sym->is_used = true;
    return Cent_build_dispatch(bd, expr);
}

Cent_value* Cent_build_function_file_name(Cent_build_data* bd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_string);
    Cent_ast* a = Cent_ast_get(n, 0);
    Zinc_string_add(&value->data.string, a->data.string.buf, a->data.string.size);
    Zinc_string_add_str(&value->name, "String");
    value->n = n;
    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_object(Cent_build_data* bd, Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_dag);
    Zinc_string_copy(&n->text, &value->name);

    Cent_symbol* sym = NULL;
    Cent_symbol_create(&sym);
    Cent_environment* top = Cent_get_environment(n);
    Cent_environment_add_symbol_str(top, "#object_value#", sym);

    Cent_ast* p = n->head;
    while (p) {
        if (p->type == Cent_ast_type_prop_set) {
            Cent_build_object_prop_set(bd, p, value);
        } else {
            Cent_value* value2 = Cent_build_dispatch(bd, p);
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
void Cent_build_object_prop_set(Cent_build_data* bd, Cent_ast* n, Cent_value* value)
{
    Cent_ast* id = Cent_ast_get(n, 0);
    Cent_ast* expr = Cent_ast_get(n, 1);
    Cent_value* expr_value = Cent_build_dispatch(bd, expr);
    Cent_value_set(value, &id->text, expr_value);
}