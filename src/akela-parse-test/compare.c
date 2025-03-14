#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <centipede/base.h>
#include "zinc/input_unicode_file.h"
#include "centipede/comp_table.h"
#include <assert.h>
#include <akela/comp_unit.h>
#include "zinc/spec_error.h"
#include "zinc/input_unicode_string.h"
#include "data.h"

typedef struct {Zinc_string name1; Zinc_string name2; Zinc_spec_error_list* list;} Apt_test_data;

void Apt_compare_type_use(Apt_test_data* data, Ake_type_use* tu, Cent_value* value);
void Apt_compare_type_def(Apt_test_data* data, Ake_type_def* td, Cent_value* value);

void Apt_print_errors(Zinc_spec_error_list* list)
{
    Zinc_spec_error* e = list->head;
    while (e) {
        Zinc_string_finish(&e->message);
        Zinc_string_finish(&e->name1);
        Zinc_string_finish(&e->name2);
        fprintf(
            stderr,
            "(%zu,%zu): %s\n"
            "(%zu,%zu): %s\n"
            "\t%s\n",
            e->loc1.line,
            e->loc1.col,
            e->name1.buf,
            e->loc2.line,
            e->loc2.col,
            e->name2.buf,
            e->message.buf);
        e = e->next;
    }
}

void Apt_error(Apt_test_data* data, Ake_ast* n, Cent_value* value, Zinc_string* message)
{
    Zinc_spec_error* error = NULL;
    Zinc_spec_error_create(&error);
    if (n) {
        error->loc1 = n->loc;
    }
    if (value) {
        Cent_ast* n2 = value->n;
        error->loc2 = n2->loc;
    }
    Zinc_string_add_string(&error->name1, &data->name1);
    Zinc_string_add_string(&error->name2, &data->name2);
    Zinc_string_add_string(&error->message, message);
    Zinc_spec_error_list_add(data->list, error);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Apt_compare_ast(Apt_test_data* data, Ake_ast* n, Cent_value* value)
{
    if (!n && !value) {
        return;
    }

    if (n && !value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "value is null");
        Apt_error(data, n, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (!n && value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "node is null");
        Apt_error(data, n, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "Ast")) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "expected AST value: %bf", &value->name);
        Apt_error(data, n, value, &message);
        Zinc_string_destroy(&message);
    }

    /* properties */
    Cent_value* type = Cent_value_get_str(value, "type");
    assert(type);
    if (n->type != type->data.enumeration.enum_value->value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(
            &message,
            "Ast type does not match (%d-%s) (%d-%bf)\n",
            n->type,
            Ast_type_name(n->type),
            type->data.enumeration.enum_value->value,
            &type->data.enumeration.enum_value->display);
        Apt_error(data, n, value, &message);
        Zinc_string_destroy(&message);
    }

    Cent_value* value_prop = Cent_value_get_str(value, "value");
    if (n->value.size > 0) {
        if (!value_prop) {
            Zinc_string message;
            Zinc_string_init(&message);
            Zinc_string_add_format(&message, "value not expected");
            Apt_error(data, n, value, &message);
            Zinc_string_destroy(&message);
        } else {
            assert(value_prop->type == Cent_value_type_string);
            if (!Zinc_string_compare(&n->value, &value_prop->data.string)) {
                Zinc_string message;
                Zinc_string_init(&message);
                Zinc_string_add_format(
                    &message,
                    "AST values do not match (%bf) (%bf)",
                    &n->value,
                    &value_prop->data.string);
                Apt_error(data, n, value, &message);
                Zinc_string_destroy(&message);
            }
        }
    } else {
        if (value_prop && value_prop->data.string.size > 0) {
            Zinc_string message;
            Zinc_string_init(&message);
            Zinc_string_add_format(&message, "value expected");
            Apt_error(data, n, value, &message);
            Zinc_string_destroy(&message);
        }
    }

    Ake_type_use* tu = n->tu;
    Cent_value* tu_value = Cent_value_get_str(value, "tu");
    Apt_compare_type_use(data, tu, tu_value);

    /* children */
    Ake_ast* n2 = NULL;
    Cent_value* value2 = NULL;

    n2 = n->head;
    if (value->type == Cent_value_type_dag) {
        value2 = value->data.dag.head;
    }

    while (n2 || value2) {
        Apt_compare_ast(data, n2, value2);
        if (n2) {
            n2 = n2->next;
        }
        if (value2) {
            value2 = value2->next;
        }
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Apt_compare_type_use(Apt_test_data* data, Ake_type_use* tu, Cent_value* value)
{
    if (!tu && !value) {
        return;
    }

    if (tu && !value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "type use value is null");
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (!tu && value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "type use node is null");
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "Type_use")) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "expected type use value: %bf", &value->name);
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
    }

    /* properties */
    Ake_type_def* td = tu->td;
    Cent_value* td_value = Cent_value_get_str(value, "td");
    Apt_compare_type_def(data, td, td_value);

    /* children */
    Ake_type_use* tu2 = NULL;
    Cent_value* value2 = NULL;

    tu2 = tu->head;
    if (value->type == Cent_value_type_dag) {
        value2 = value->data.dag.head;
    }

    while (tu2 || value2) {
        Apt_compare_type_use(data, tu2, value2);
        if (tu2) {
            tu2 = tu2->next;
        }
        if (value2) {
            value2 = value2->next;
        }
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Apt_compare_type_def(Apt_test_data* data, Ake_type_def* td, Cent_value* value)
{
    if (!td && !value) {
        return;
    }

    if (td && !value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "type def value is null");
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (!td && value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "type use node is null");
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "Type_def")) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "expected type def value: %bf", &value->name);
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
    }

    Cent_value* name_value = Cent_value_get_str(value, "name");
    if (!name_value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "name not set");
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
    } else {
        if (!Zinc_string_compare(&td->name, &name_value->data.string)) {
            assert(name_value->type == Cent_value_type_string);
            Zinc_string message;
            Zinc_string_init(&message);
            Zinc_string_add_format(
                &message,
                "type def name does not match (%bf) (%bf)",
                &td->name,
                &name_value->data.string);
            Apt_error(data, NULL, value, &message);
            Zinc_string_destroy(&message);
        }
    }

    Cent_value* type_value = Cent_value_get_str(value, "type");
    if (!type_value) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message, "type not set");
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
    } else {
        assert(type_value->type == Cent_value_type_enum);
        if (td->type != type_value->data.enumeration.enum_value->value) {
            Zinc_string message;
            Zinc_string_init(&message);
            Zinc_string_add_format(&message,
                "type def type does not match (%d) (%d)",
                td->type,
                type_value->data.enumeration.enum_value->value);
            Apt_error(data, NULL, value, &message);
            Zinc_string_destroy(&message);
        }
    }

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_natural);
        if (td->bit_count != bit_count_value->data.integer) {
            Zinc_string message;
            Zinc_string_init(&message);
            Zinc_string_add_format(&message,
                "type def bit_count does not match (%d) (%d)",
                td->bit_count,
                bit_count_value->data.integer);
            Apt_error(data, NULL, value, &message);
            Zinc_string_destroy(&message);
        }
    } else if (td->bit_count > 0) {
        Zinc_string message;
        Zinc_string_init(&message);
        Zinc_string_add_format(&message,
            "bit_count not set");
        Apt_error(data, NULL, value, &message);
        Zinc_string_destroy(&message);
    }

    Cent_value* is_signed_value = Cent_value_get_str(value, "is_signed");
    if (is_signed_value) {
        assert(is_signed_value->type == Cent_value_type_boolean);
        if (td->is_signed != is_signed_value->data.boolean) {
            Zinc_string message;
            Zinc_string_init(&message);
            Zinc_string_add_format(&message,
                "type def is_signed does not match (%d) (%d)",
                td->is_signed,
                is_signed_value->data.boolean);
            Apt_error(data, NULL, value, &message);
            Zinc_string_destroy(&message);
        }
    }
}
