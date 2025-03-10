#include "parse_data.h"
#include "value.h"
#include <assert.h>

#include "check_value_types.h"
#include "parse_tools.h"

Cent_value* Cent_build_dispatch(Cent_ast* n);
Cent_value* Cent_build_integer(Cent_ast* n);
Cent_value* Cent_build_natural(Cent_ast* n);
Cent_value* Cent_build_real(Cent_ast* n);
Cent_value* Cent_build_string(Cent_ast* n);
Cent_value* Cent_build_boolean(Cent_ast* n);
Cent_value* Cent_build_namespace(Cent_ast* n);
Cent_value* Cent_build_assign(Cent_ast* n);
Cent_value* Cent_build_const(Cent_ast* n);
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
            if (p->type == Cent_ast_type_const || p == pr->root->tail) {
                value = Cent_build_dispatch(p);
            }
            p = p->next;
        }
    }

    if (!pr->errors->head) {
        Cent_check_value_types(pr, value);
    }

    return value;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_dispatch(Cent_ast* n)
{
    if (n->type == Cent_ast_type_expr_integer) {
        return Cent_build_integer(n);
    }

    if (n->type == Cent_ast_type_expr_natural) {
        return Cent_build_natural(n);
    }

    if (n->type == Cent_ast_type_expr_real) {
        return Cent_build_real(n);
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

    if (n->type == Cent_ast_type_const) {
        return Cent_build_const(n);
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

Cent_value* Cent_build_integer(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_integer);

    Zinc_string_add_str(&value->name, "Integer");
    value->data.integer = n->data.integer;

    value->n = n;
    return value;
}

Cent_value* Cent_build_natural(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_natural);

    Zinc_string_add_str(&value->name, "Natural");
    value->data.natural = n->data.natural;

    value->n = n;
    return value;
}

Cent_value* Cent_build_real(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_real);

    Zinc_string_add_str(&value->name, "Real");
    value->data.real = n->data.real;

    value->n = n;
    return value;
}

Cent_value* Cent_build_string(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_string);
    Zinc_string_copy(&n->data.string, &value->data.string);
    Zinc_string_add_str(&value->name, "String");
    value->n = n;
    return value;
}

Cent_value* Cent_build_boolean(Cent_ast* n)
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
Cent_value* Cent_build_namespace(Cent_ast* n)
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
            if (sym->data.variable.value) {
                return sym->data.variable.value;
            } else {
                Cent_value* value = Cent_build_dispatch(sym->data.variable.n);
                sym->data.variable.value = value;
                return value;
            }
        }
    }

    return NULL;
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

/* NOLINTNEXTLINE(misc-no-recursion) */
Cent_value* Cent_build_const(Cent_ast* n)
{
    Cent_ast* a = Cent_ast_get(n, 0);
    Cent_ast* b = Cent_ast_get(n, 1);
    Cent_value* value = Cent_build_dispatch(b);
    assert(value);
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
Cent_value* Cent_build_object(Cent_ast* n)
{
    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_dag);
    Zinc_string_copy(&n->text, &value->name);

    Cent_symbol* sym = NULL;
    Cent_symbol_create(&sym);
    Cent_symbol_set_type(sym, Cent_symbol_type_object_value);
    sym->data.object_value = value;
    Cent_environment* top = Cent_get_environment(n);
    Cent_environment_add_symbol_str(top, "#object_value#", sym);

    Cent_ast* p = n->head;
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
