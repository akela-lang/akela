#include <float.h>
#include <math.h>

#include "value.h"
#include "element.h"
#include <stdint.h>
#include "ast.h"

bool Cent_is_number_type(Cent_value_type type);

void Cent_cast_value(Cent_value* value, Cent_element_type* declared, Zinc_error_list* errors)
{
    if (!Cent_is_number_type(value->type) || !Cent_is_number_type(declared->type)) {
        return;
    }

    Cent_ast* n = value->n;
    if (value->type == Cent_value_type_natural && declared->type == Cent_value_type_integer) {
        if (value->data.natural > (uint64_t)INT64_MAX) {
            Zinc_error_list_set(
                errors,
                &n->loc,
                "Natural (%lu) too large to cast to Integer",
                value->data.natural);
            return;
        }
        value->type = Cent_value_type_integer;
        value->data.integer = (int64_t)value->data.natural;
        Zinc_string_clear(&value->name);
        Zinc_string_add_str(&value->name, "Integer");
        return;
    }

    if (value->type == Cent_value_type_integer && declared->type == Cent_value_type_natural) {
        if (value->data.integer < 0) {
            Zinc_error_list_set(
                errors,
                &n->loc,
                "Integer (%ld) is negative; cannot cast to Natural",
                value->data.integer);
            return;
        }
        value->type = Cent_value_type_natural;
        value->data.natural = (uint64_t)value->data.integer;
        Zinc_string_clear(&value->name);
        Zinc_string_add_str(&value->name, "Natural");
        return;
    }

    if (value->type == Cent_value_type_natural && declared->type == Cent_value_type_real) {
        value->type = Cent_value_type_real;
        value->data.real = (double)value->data.natural;
        Zinc_string_clear(&value->name);
        Zinc_string_add_str(&value->name, "Real");
        return;
    }

    if (value->type == Cent_value_type_real && declared->type == Cent_value_type_natural) {
        if (value->data.real < 0) {
            Zinc_error_list_set(
                errors,
                &n->loc,
                "Real (%lf) is negative; cannot cast to Natural",
                value->data.real);
            return;
        }

        if (value->data.real > (double)UINT64_MAX) {
            Zinc_error_list_set(
                errors,
                &n->loc,
                "Real (%lf) is too large; cannot cast to Natural",
                value->data.real);
            return;
        }
        value->type = Cent_value_type_natural;
        value->data.natural = (uint64_t)value->data.real;
        Zinc_string_clear(&value->name);
        Zinc_string_add_str(&value->name, "Natural");
        return;
    }

    if (value->type == Cent_value_type_integer && declared->type == Cent_value_type_real) {
        value->type = Cent_value_type_real;
        value->data.real = (double)value->data.integer;
        Zinc_string_clear(&value->name);
        Zinc_string_add_str(&value->name, "Real");
        return;
    }

    if (value->type == Cent_value_type_real && declared->type == Cent_value_type_integer) {
        if (value->data.real < (double)INT64_MIN) {
            Zinc_error_list_set(
                errors,
                &n->loc,
                "Real (%lf) is too large of a negative; cannot cast to Integer",
                value->data.real);
            return;
        }
        if (value->data.real > (double)INT64_MAX) {
            Zinc_error_list_set(
                errors,
                &n->loc,
                "Real (%lf) is too large; cannot cast to Integer",
                value->data.real);
            return;
        }
        value->type = Cent_value_type_integer;
        value->data.integer = (int64_t)value->data.real;
        Zinc_string_clear(&value->name);
        Zinc_string_add_str(&value->name, "Integer");
        return;
    }
}

bool Cent_is_number_type(Cent_value_type type)
{
    if (type == Cent_value_type_natural) {
        return true;
    }

    if (type == Cent_value_type_integer) {
        return true;
    }

    if (type == Cent_value_type_real) {
        return true;
    }

    return false;
}