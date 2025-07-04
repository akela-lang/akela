#include "zinc/test.h"
#include "zinc/error.h"
#include "centipede/value.h"
#include "data.h"
#include "zinc/expect.h"

bool Apt_check_error(
    Zinc_test* top_test,
    Zinc_test* case_test,
    Zinc_error_list* errors,
    Cent_value* expected_error);

bool Apt_check_errors(
    Zinc_test* top_test,
    Zinc_test* case_test,
    Zinc_error_list* errors,
    Cent_value* expected)
{
    Apt_top_data* top_data = top_test->data;
    Apt_case_data* case_data = case_test->data;

    if (!expected) {
        Zinc_error_list_set(&top_data->errors, NULL, "expected Errors");
        return false;
    }

    Cent_ast* n = expected->n;

    if (!Zinc_string_compare_str(&expected->name, "Errors")) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            NULL,
            &n->loc,
            "expected Errors");
        return false;
    }

    if (expected->type != Cent_value_type_dag) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            NULL,
            &n->loc,
            "expected Dict");
        return false;
    }

    bool pass = true;
    Cent_value* expected_error = expected->data.dag.head;
    while (expected_error) {
        pass = Apt_check_error(top_test, case_test, errors, expected_error) && pass;
        expected_error = expected_error->next;
    }

    return pass;
}

bool Apt_check_error(
    Zinc_test* top_test,
    Zinc_test* case_test,
    Zinc_error_list* errors,
    Cent_value* expected_error)
{
    Apt_top_data* top_data = top_test->data;
    Apt_case_data* case_data = case_test->data;

    Cent_ast* n = expected_error->n;
    Cent_value* message = Cent_value_get_str(expected_error, "message");
    bool valid = true;

    if (!message) {
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected message");
        valid = false;
    }

    if (message && message->type != Cent_value_type_string) {
        Zinc_error_list_set(&top_data->errors, &n->loc, "message is not a string");
        valid = false;
    }

    Cent_value* line = Cent_value_get_str(expected_error, "line");
    if (!line) {
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected line");
        valid = false;
    }

    if (line && line->type != Cent_value_type_natural) {
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected line to be a natural");
        valid = false;
    }

    Cent_value* col = Cent_value_get_str(expected_error, "col");
    if (!col) {
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected col");
        valid = false;
    }

    if (col && col->type != Cent_value_type_natural) {
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected col to be a natural");
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

    Zinc_string expect_message;
    Zinc_string_init(&expect_message);
    Zinc_string_add_format(&expect_message, "could not find error: %s", Zinc_string_c_str(&message->data.string));
    if (Zinc_expect_true(case_test, found, Zinc_string_c_str(&expect_message))) {
        Zinc_expect_size_t_equal(case_test, line->data.natural, e->loc.line, "incorrect line");
        Zinc_expect_size_t_equal(case_test, col->data.natural, e->loc.col, "incorrect column");
    }

    Zinc_string_destroy(&expect_message);
    return valid;
}