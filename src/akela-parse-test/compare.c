#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <centipede/base.h>
#include "zinc/input_unicode_file.h"
#include "centipede/comp_table.h"
#include <assert.h>
#include <string.h>
#include <akela/comp_unit.h>
#include "zinc/spec_error.h"
#include "zinc/input_unicode_string.h"
#include "data.h"
#include <errno.h>
#include <akela/comp_table.h>
#include <akela/ast_to_cent.h>

void Apt_run_suite(Apt_data* data, Apt_suite* suite);
void Apt_run_test(Apt_data* data, Apt_suite* suite, Apt_test* test);
bool Apt_compare_ast(Apt_data* data, Zinc_test* test, Ake_ast* n, Cent_value* value);
bool Apt_compare_type_use(
    Apt_data* data,
    Zinc_test* test,
    Zinc_location* loc,
    Ake_type_use* tu,
    Ake_ast* parent_n,
    Cent_value* value,
    Cent_value* parent_value);
bool Apt_compare_type_def(
    Apt_data* data,
    Zinc_test* test,
    Zinc_location* loc,
    Ake_type_def* td,
    Cent_value* value);
bool Apt_check_errors(
    Apt_data* data,
    Apt_test* test,
    Zinc_error_list* errors,
    Cent_value* expected);
bool Apt_check_error(
    Apt_data* data,
    Apt_test* test,
    Zinc_error_list* errors,
    Cent_value* expected_error);

void Apt_run(Apt_data* data)
{
    data->test->ran = true;
    Apt_suite* suite = data->suites.head;
    while (suite) {
        if (!suite->test->mute && (!data->test->has_solo || (suite->test->solo))) {
            Apt_run_suite(data, suite);
        }
        suite = suite->next;
    }
}

void Apt_run_suite(Apt_data* data, Apt_suite* suite)
{
    Zinc_string_add_string(&suite->test->name, &suite->description);
    suite->test->ran = true;
    Apt_test* tc = suite->list.head;
    while (tc) {
        if (!tc->test->mute && (!suite->test->has_solo || (tc->test->solo))) {
            Apt_run_test(data, suite, tc);
        }
        tc = tc->next;
    }
}

void Apt_run_test(Apt_data* data, Apt_suite* suite, Apt_test* test)
{
    bool pass = true;

    Zinc_string_add_string(&test->test->name, &test->description);
    test->test->ran = true;
    FILE* fp = fopen(Zinc_string_c_str(&suite->path), "r");
    if (!fp) {
        Zinc_location loc;
        Zinc_location_init(&loc);
        Zinc_error_list_set(
            &data->errors,
            &loc,
            "fopen: %s: %s",
            Zinc_string_c_str(&suite->path),
            strerror(errno));
        return;
    }

    Ake_comp_table* ct = NULL;
    Ake_comp_table_create_fp(&ct, suite->path, fp);
    Ake_comp_unit_set_bounds(ct->primary, &test->source_bounds);
    Ake_parse_result pr = Ake_comp_unit_parse(ct->primary);

    // close file before reopening
    Ake_comp_unit_destroy_input(ct->primary);

    if (test->snapshot) {
        Ake_ast_cent_print(ct->primary->root, 0);
    } else {
        Cent_comp_table* expected_ct = NULL;
        fp = fopen(Zinc_string_c_str(&suite->path), "r");
        if (!fp) {
            Zinc_location loc;
            Zinc_location_init(&loc);
            Zinc_error_list_set(
                &data->errors,
                &loc,
                "fopen: %s: %s",
                Zinc_string_c_str(&suite->path),
                strerror(errno));
            Ake_comp_table_free(ct);
            return;
        }
        Cent_comp_table_create_fp(&expected_ct, &data->dir_path, &test->source_name, fp);
        Cent_comp_unit_set_bounds(expected_ct->primary, &test->ast_bounds);
        Cent_comp_unit_parse(expected_ct->primary);
        Cent_comp_unit_build(expected_ct->primary);

        if (test->has_error) {
            test->test->pass = Apt_check_errors(
                data,
                test,
                &ct->primary->errors,
                expected_ct->primary->value);
        } else {
            if (pr.errors->head) {
                Zinc_error* e = pr.errors->head;
                while (e) {
                    Zinc_error_list_set(&data->errors, &e->loc, "%bf", &e->message);
                    e = e->next;
                }
                Zinc_error_list_print(&data->errors);
                Ake_comp_table_free(ct);
                return;
            }

            if (expected_ct->primary->errors.head) {
                Zinc_error* e = expected_ct->primary->errors.head;
                while (e) {
                    Zinc_error_list_set(&data->errors, &e->loc, "%bf", &e->message);
                    e = e->next;
                }
                Zinc_error_list_print(&data->errors);

                Ake_comp_table_free(ct);
                Cent_comp_table_destroy(expected_ct);
                free(expected_ct);
                return;
            }

            test->test->pass = Apt_compare_ast(
                data,
                test->test,
                ct->primary->root,
                expected_ct->primary->value);
        }

        Cent_comp_table_destroy(expected_ct);
        free(expected_ct);
    }

    Ake_comp_table_free(ct);
}

bool Apt_check_errors(Apt_data* data, Apt_test* test, Zinc_error_list* errors, Cent_value* expected)
{
    printf("check errors\n");
    if (!expected) {
        Zinc_error_list_set(&data->errors, NULL, "expected Errors");
        return false;
    }

    Cent_ast* n = expected->n;

    if (!Zinc_string_compare_str(&expected->name, "Errors")) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test->test,
            NULL,
            &n->loc,
            "expected Errors");
        return false;
    }

    if (expected->type != Cent_value_type_dag) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test->test,
            NULL,
            &n->loc,
            "expected Dict");
        return false;
    }

    bool pass = true;
    Cent_value* expected_error = expected->data.dag.head;
    while (expected_error) {
        pass = Apt_check_error(data, test, errors, expected_error) && pass;
        expected_error = expected_error->next;
    }

    return pass;
}

bool Apt_check_error(
    Apt_data* data,
    Apt_test* test,
    Zinc_error_list* errors,
    Cent_value* expected_error)
{
    Cent_ast* n = expected_error->n;
    Cent_value* message = Cent_value_get_str(expected_error, "message");
    bool valid = true;

    if (!message) {
        Zinc_error_list_set(&data->errors, &n->loc, "expected message");
        valid = false;
    }

    if (message && message->type != Cent_value_type_string) {
        Zinc_error_list_set(&data->errors, &n->loc, "message is not a string");
        valid = false;
    }

    Cent_value* line = Cent_value_get_str(expected_error, "line");
    if (!line) {
        Zinc_error_list_set(&data->errors, &n->loc, "expected line");
        valid = false;
    }

    if (line && line->type != Cent_value_type_natural) {
        Zinc_error_list_set(&data->errors, &n->loc, "expected line to be a natural");
        valid = false;
    }

    Cent_value* col = Cent_value_get_str(expected_error, "col");
    if (!col) {
        Zinc_error_list_set(&data->errors, &n->loc, "expected col");
        valid = false;
    }

    if (col && col->type != Cent_value_type_natural) {
        Zinc_error_list_set(&data->errors, &n->loc, "expected col to be a natural");
        valid = false;
    }

    if (!valid) {
        return valid;
    }

    Zinc_error* e = errors->head;
    bool found = false;
    while (e) {
        if (Zinc_string_compare(&e->message, &message->data.string)) {
            found = true;
            break;
        }
        e = e->next;
    }

    if (found) {
        if (line->data.natural != e->loc.line) {
            Zinc_error_list_set(
                &data->errors,
                &n->loc,
                "line does not match: (%zu), (%zu)",
                e->loc.line,
                line->data.natural);
        }

        if (col->data.natural != e->loc.col) {
            Zinc_error_list_set(
                &data->errors,
                &n->loc,
                "col does not match: (%zu) (%zu)",
                e->loc.col,
                col->data.natural);
        }
    } else {
        Zinc_string s;
        Zinc_string_init(&s);
        Zinc_string_add_format(&s, "could not find error: %bf", &message->data.string);
        Zinc_string_add_format(&s, "\n\tErrors:");
        Zinc_error* e2 = errors->head;
        while (e2) {
            Zinc_string_add_format(
                &s,
                "\n\t(%zu,%zu): %bf",
                e2->loc.line,
                e2->loc.col,
                &e2->message);
            e2 = e2->next;
        }
        Zinc_error_list_set(
            &data->errors,
            &n->loc,
            Zinc_string_c_str(&s));
        Zinc_string_destroy(&s);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_ast(Apt_data* data, Zinc_test* test, Ake_ast* n, Cent_value* value)
{
    bool pass = true;

    if (!n && !value) {
        return true;
    }

    if (n && !value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            &n->loc,
            NULL,
            "value is null");
        return false;
    }

    Cent_ast* value_n = value->n;

    if (!n && value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            NULL,
            &value_n->loc,
            "node is null");
        return false;
    }

    if (!Zinc_string_compare_str(&value->name, "Ast")) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            &n->loc,
            &value_n->loc,
            "expected AST value: %bf",
            &value->name);
        pass = false;
    }

    /* properties */
    Cent_value* type = Cent_value_get_str(value, "@tag");
    assert(type);
    if (n->type != type->data.enumeration.enum_value->value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            &n->loc,
            &value_n->loc,
            "Ast type does not match (%d-%s) (%d-%bf)",
            n->type,
            Ast_type_name(n->type),
            type->data.enumeration.enum_value->value,
            &type->data.enumeration.enum_value->display);
        pass = false;
    }

    Cent_value* value_prop = Cent_value_get_str(value, "value");
    if (n->value.size > 0) {
        if (!value_prop) {
            Zinc_spec_error_list_set(
                &data->spec_errors,
                test,
                &n->loc,
                &value_n->loc,
                "value not expected");
                pass = false;
        } else {
            assert(value_prop->type == Cent_value_type_string);
            if (!Zinc_string_compare(&n->value, &value_prop->data.string)) {
                Cent_ast* prop_n = value_prop->n;
                Zinc_spec_error_list_set(
                    &data->spec_errors,
                    test,
                    &n->loc,
                    &prop_n->loc,
                    "AST values do not match (%bf) (%bf)",
                    &n->value,
                    &value_prop->data.string);
                pass = false;
            }
        }
    } else {
        if (value_prop && value_prop->data.string.size > 0) {
            Zinc_spec_error_list_set(
                &data->spec_errors,
                test,
                &n->loc,
                &value_n->loc,
                "value expected");
            pass = false;
        }
    }

    Ake_type_use* tu = n->tu;
    Cent_value* tu_value = Cent_value_get_str(value, "tu");
    pass = Apt_compare_type_use(data, test, &n->loc, tu, n, tu_value, value) && pass;

    /* children */
    Ake_ast* n2 = NULL;
    Cent_value* value2 = NULL;

    n2 = n->head;
    if (value->type == Cent_value_type_dag) {
        value2 = value->data.dag.head;
    }

    while (n2 || value2) {
        pass = Apt_compare_ast(data, test, n2, value2) && pass;
        if (n2) {
            n2 = n2->next;
        }
        if (value2) {
            value2 = value2->next;
        }
    }

    return pass;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_type_use(
    Apt_data* data,
    Zinc_test* test,
    Zinc_location* loc,
    Ake_type_use* tu,
    Ake_ast* parent_n,
    Cent_value* value,
    Cent_value* parent_value)
{
    bool pass = true;

    if (!tu && !value) {
        return true;
    }

    if (tu && !value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            NULL,
            "type use value is null: (%d-%s)-(NULL)",
            parent_n->type,
            Ast_type_name(parent_n->type));
        return false;
    }

    Ake_ast* value_n = value->n;

    if (!tu && value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            &value_n->loc,
            "type use node is null: %d-%s");
        return false;
    }

    if (!Zinc_string_compare_str(&value->name, "TypeUse")) {
            Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            &value_n->loc,
            "expected type use value: %bf",
            &value->name);
        pass = false;
    }

    /* properties */
    Ake_type_def* td = tu->td;
    Cent_value* td_value = Cent_value_get_str(value, "td");
    pass = Apt_compare_type_def(data, test, loc, td, td_value) && pass;

    /* children */
    Ake_type_use* tu2 = NULL;
    Cent_value* value2 = NULL;

    tu2 = tu->head;
    if (value->type == Cent_value_type_dag) {
        value2 = value->data.dag.head;
    }

    while (tu2 || value2) {
        pass = Apt_compare_type_use(data, test, loc, tu2, parent_n, value2, parent_value) && pass;
        if (tu2) {
            tu2 = tu2->next;
        }
        if (value2) {
            value2 = value2->next;
        }
    }

    return pass;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_type_def(
    Apt_data* data,
    Zinc_test* test,
    Zinc_location* loc,
    Ake_type_def* td,
    Cent_value* value)
{
    bool pass = true;

    if (!td && !value) {
        return true;
    }

    if (td && !value) {
        Zinc_spec_error_list_set(&data->spec_errors, test, loc, NULL, "type def value is null");
        return false;
    }

    Cent_ast* value_n = value->n;
    if (!td && value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            &value_n->loc,
            "type def node is null");
        return false;
    }

    if (!Zinc_string_compare_str(&value->name, "TypeDef")) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            &value_n->loc,
            "expected type def value: %bf", &value->name);
        pass = false;
    }

    Cent_value* name_value = Cent_value_get_str(value, "name");
    if (!name_value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            &value_n->loc,
            "name not set");
    } else {
        if (!Zinc_string_compare(&td->name, &name_value->data.string)) {
            Zinc_spec_error_list_set(
                &data->spec_errors,
                test,
                loc,
                &value_n->loc,
                "type def name does not match (%bf) (%bf)",
                &td->name,
                &name_value->data.string);
            pass = false;
        }
    }

    Cent_value* type_value = Cent_value_get_str(value, "@tag");
    if (!type_value) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            &value_n->loc,
            "type not set");
    } else {
        assert(type_value->type == Cent_value_type_enum);
        if (td->type != type_value->data.enumeration.enum_value->value) {
            Zinc_spec_error_list_set(
                &data->spec_errors,
                test,
                loc,
                &value_n->loc,
                "type def type does not match (%d) (%d)",
                td->type,
                type_value->data.enumeration.enum_value->value);
            pass = false;
        }
    }

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_integer);
        if (td->bit_count != bit_count_value->data.integer) {
            Zinc_spec_error_list_set(
                &data->spec_errors,
                test,
                loc,
                &value_n->loc,
                "type def bit_count does not match (%d) (%d)",
                td->bit_count,
                bit_count_value->data.integer);
            pass = false;
        }
    } else if (td->bit_count > 0) {
        Zinc_spec_error_list_set(
            &data->spec_errors,
            test,
            loc,
            &value_n->loc,
            "bit_count not set");
        pass = false;
    }

    Cent_value* is_signed_value = Cent_value_get_str(value, "is_signed");
    if (is_signed_value) {
        assert(is_signed_value->type == Cent_value_type_boolean);
        if (td->is_signed != is_signed_value->data.boolean) {
            Zinc_spec_error_list_set(
                &data->spec_errors,
                test,
                loc,
                &value_n->loc,
                "type def is_signed does not match (%d) (%d)",
                td->is_signed,
                is_signed_value->data.boolean);
            pass = false;
        }
    }

    return pass;
}