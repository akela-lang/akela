#include "parse_data.h"
#include "value.h"
#include <assert.h>

Cent_value* Cent_build_dispatch(Cent_ast* n);
Cent_value* Cent_build_number(Cent_ast* n);
Cent_value* Cent_build_string(Cent_ast* n);
Cent_value* Cent_build_boolean(Cent_ast* n);
Cent_value* Cent_build_enum(Cent_ast* n);

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