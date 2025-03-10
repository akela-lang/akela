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
        if (value->data.natural > INT64_MAX) {
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