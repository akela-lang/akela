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
#include "data.h"
#include <errno.h>
#include <akela/comp_table.h>
#include <akela/ast_to_cent.h>
#include "zinc/expect.h"
#include "compare_errors.h"

bool Apt_compare_ast(Zinc_test* top_test, Zinc_test* case_test, Ake_Ast* n, Cent_value* value);
bool Apt_compare_type(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_integer(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_natural(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_real(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_struct(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_array(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_array_const(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_slice(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
bool Apt_compare_type_pointer(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);

void Apt_suite_run(Zinc_test* suite_test)
{
    Apt_suite_data* suite_data = suite_test->data;
    if (suite_data->errors.head) {
        Zinc_test_print_unseen(suite_test);
        Zinc_error_list_print(&suite_data->errors);
    }
    Zinc_test_perform(suite_test);
}

void Apt_case_run(Zinc_test* case_test)
{
    Zinc_test* suite_test = case_test->parent;
    Zinc_test* top_test = suite_test->parent;
    Apt_top_data* top_data = top_test->data;
    Apt_suite_data* suite_data = suite_test->data;
    Apt_case_data* case_data = case_test->data;

    case_test->ran = true;
    FILE* fp = fopen(Zinc_string_c_str(&suite_data->path), "r");
    if (!fp) {
        Zinc_location loc;
        Zinc_location_init(&loc);
        Zinc_error_list_set(
            &top_data->errors,
            &loc,
            "fopen: %s: %s",
            Zinc_string_c_str(&suite_data->path),
            strerror(errno));
        return;
    }

    Ake_comp_table* ct = NULL;
    Ake_comp_table_create_fp(&ct, suite_data->path, fp);
    Ake_comp_unit_set_bounds(ct->primary, &case_data->source_bounds);
    Ake_parse_result pr = Ake_comp_unit_parse(ct->primary);

    // close file before reopening
    Ake_comp_unit_destroy_input(ct->primary);

    if (case_data->snapshot) {
        Ake_ast_cent_print(ct->primary->root, 0);
    } else {
        Cent_comp_table* expected_ct = NULL;
        fp = fopen(Zinc_string_c_str(&suite_data->path), "r");
        if (!fp) {
            Zinc_location loc;
            Zinc_location_init(&loc);
            Zinc_error_list_set(
                &top_data->errors,
                &loc,
                "fopen: %s: %s",
                Zinc_string_c_str(&suite_data->path),
                strerror(errno));
            Ake_comp_table_free(ct);
            return;
        }
        Cent_comp_table_create_fp(&expected_ct, &top_data->dir_path, &case_data->source_name, fp);
        Cent_comp_unit_set_bounds(expected_ct->primary, &case_data->ast_bounds);
        Cent_comp_unit_parse(expected_ct->primary);
        Cent_comp_unit_build(expected_ct->primary);

        if (case_data->has_error) {
            case_test->pass = Apt_check_errors(
                top_test,
                case_test,
                &ct->primary->errors,
                expected_ct->primary->value);
        } else {
            if (pr.errors->head) {
                Zinc_error* e = pr.errors->head;
                while (e) {
                    Zinc_error_list_set(&top_data->errors, &e->loc, "%bf", &e->message);
                    e = e->next;
                }
                Zinc_error_list_print(&top_data->errors);
                Ake_comp_table_free(ct);
                return;
            }

            if (expected_ct->primary->errors.head) {
                Zinc_error* e = expected_ct->primary->errors.head;
                while (e) {
                    Zinc_error_list_set(&top_data->errors, &e->loc, "%bf", &e->message);
                    e = e->next;
                }
                Zinc_error_list_print(&top_data->errors);

                Ake_comp_table_free(ct);
                Cent_comp_table_destroy(expected_ct);
                free(expected_ct);
                return;
            }

            case_test->pass = Apt_compare_ast(
                top_test,
                case_test,
                ct->primary->root,
                expected_ct->primary->value);
        }

        Cent_comp_table_destroy(expected_ct);
        free(expected_ct);
    }

    Ake_comp_table_free(ct);
    if (case_data->errors.head) {
        Zinc_error_list_print(&case_data->errors);
    }

    if (case_data->spec_errors.head) {
        Zinc_spec_error_list_print(&case_data->spec_errors);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_ast(Zinc_test* top_test, Zinc_test* case_test, Ake_Ast* n, Cent_value* value)
{
    Apt_top_data* top_data = top_test->data;
    Apt_case_data* case_data = case_test->data;

    bool pass = true;

    if (!n && !value) {
        return true;
    }

    if (n && !value) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            NULL,
            "value is null");
        return false;
    }

    Cent_ast* value_n = value->n;

    if (!n && value) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            NULL,
            &value_n->loc,
            "node is null");
        return false;
    }

    if (!Zinc_string_compare_str(&value->name, "Ast")) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "expected AST value: %bf",
            &value->name);
        pass = false;
    }

    /* properties */
    Cent_value* tag = Cent_value_get_str(value, "@tag");
    assert(tag);
    if (n->kind != tag->data.enumeration.enum_value->value) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "Ast type does not match (%d-%s) (%d-%bf)",
            n->kind,
            Ast_type_name(n->kind),
            tag->data.enumeration.enum_value->value,
            &tag->data.enumeration.enum_value->display);
        pass = false;
    }

    Cent_value* value_prop = Cent_value_get_str(value, "value");
    if (n->value.size > 0) {
        if (!value_prop) {
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "value not expected");
                pass = false;
        } else {
            assert(value_prop->type == Cent_value_type_string);
            if (!Zinc_string_compare(&n->value, &value_prop->data.string)) {
                Cent_ast* prop_n = value_prop->n;
                Zinc_spec_error_list_set(
                    &case_data->spec_errors,
                    case_test,
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
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "value expected");
            pass = false;
        }
    }

    Ake_Type* type = n->type;
    Cent_value* tu_value = Cent_value_get_str(value, "tu");
    pass = Apt_compare_type(case_test, n, type, tu_value) && pass;

    /* children */
    Ake_Ast* n2 = NULL;
    Cent_value* value2 = NULL;

    n2 = n->head;
    if (value->type == Cent_value_type_dag) {
        value2 = value->data.dag.head;
    }

    while (n2 || value2) {
        pass = Apt_compare_ast(top_test, case_test, n2, value2) && pass;
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
bool Apt_compare_type(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    bool pass = true;

    if (!type && !value) {
        return true;
    }

    Apt_case_data* case_data = case_test->data;

    if (type && !value) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            NULL,
            "type use value is null: (%d-%s)-(NULL)",
            n->kind,
            Ast_type_name(n->kind));
        return false;
    }

    Ake_Ast* value_n = value->n;

    if (!type && value) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "type use node is null: %d-%s");
        return false;
    }

    if (!Zinc_string_compare_str(&value->name, "TypeDef")) {
        Zinc_spec_error_list_set(
        &case_data->spec_errors,
        case_test,
        &n->loc,
        &value_n->loc,
        "expected type use value: %bf",
        &value->name);
        pass = false;
    }

    Cent_value* name_value = Cent_value_get_str(value, "name");
    if (!name_value) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "name not set");
    } else {
        if (!Zinc_string_compare(&type->name, &name_value->data.string)) {
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def name does not match (%bf) (%bf)",
                &type->name,
                &name_value->data.string);
            pass = false;
        }
    }

    Cent_value* tag = Cent_value_get_str(value, "@tag");
    if (Zinc_expect_size_t_equal(case_test, type->kind, tag->data.enumeration.number, "type tag")) {
        return pass;
    }

    switch (type->kind) {
        case AKE_TYPE_INTEGER:
            return Apt_compare_type_integer(case_test, n, type, value);
        case AKE_TYPE_NATURAL:
            return Apt_compare_type_natural(case_test, n, type, value);
        case AKE_TYPE_REAL:
            return Apt_compare_type_real(case_test, n, type, value);
        case AKE_TYPE_BOOLEAN:
            break;
        case AKE_TYPE_STRUCT:
            return Apt_compare_type_struct(case_test, n, type, value);
        case AKE_TYPE_ARRAY:
            return Apt_compare_type_array(case_test, n, type, value);
        case AKE_TYPE_SLICE:
            return Apt_compare_type_slice(case_test, n, type, value);
        case AKE_TYPE_POINTER:
            return Apt_compare_type_pointer(case_test, n, type, value);
        default:
            assert(false && "invalid kind");
    }

    return pass;
}

bool Apt_compare_type_integer(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    Apt_case_data* case_data = case_test->data;
    Ake_Ast* value_n = value->n;

    bool pass = true;

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_natural);
        if (type->data.integer.bit_count != bit_count_value->data.natural) {
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def integer bit_count does not match (%d) (%d)",
                type->data.integer.bit_count,
                bit_count_value->data.integer);
            pass = false;
        }
    } else if (type->data.integer.bit_count > 0) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "bit_count not set");
        pass = false;
    }

    return pass;
}

bool Apt_compare_type_natural(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    Apt_case_data* case_data = case_test->data;
    Ake_Ast* value_n = value->n;

    bool pass = true;

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_natural);
        if (type->data.natural.bit_count != bit_count_value->data.natural) {
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def natural bit_count does not match (%d) (%d)",
                type->data.natural.bit_count,
                bit_count_value->data.natural);
            pass = false;
        }
    } else if (type->data.natural.bit_count > 0) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "bit_count not set");
        pass = false;
    }

    return pass;
}

bool Apt_compare_type_real(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    Apt_case_data* case_data = case_test->data;
    Ake_Ast* value_n = value->n;

    bool pass = true;

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_natural);
        if (type->data.real.bit_count != bit_count_value->data.natural) {
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def real bit_count does not match (%d) (%d)",
                type->data.real.bit_count,
                bit_count_value->data.real);
            pass = false;
        }
    } else if (type->data.real.bit_count > 0) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "bit_count not set");
        pass = false;
    }

    return pass;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_type_struct(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    Apt_case_data* case_data = case_test->data;
    Ake_Ast* value_n = value->n;

    bool pass = true;

    Ake_TypeField* tf = type->data.fields.head;
    Cent_value* field_value = value->data.dag.head;

    while (tf || field_value) {
        if (!tf && !field_value) {
            break;
        }

        if (!Zinc_expect_true(case_test, tf, "type field null")) {
            break;
        }

        if (!Zinc_expect_true(case_test, field_value, "field value null")) {
            break;
        }

        assert(tf);
        assert(field_value);

        Cent_value* name_value = Cent_value_get_str(field_value, "name");

        if (!Zinc_expect_ptr(case_test, name_value, "name not in type field")) {
            break;
        }

        if (!Zinc_expect_string(
            case_test,
            &name_value->data.string,
            Zinc_string_c_str(&tf->name),
            "name match")) {
            break;
        }

        Cent_value* type_value = Cent_value_get_str(field_value, "td");
        if (!Zinc_expect_ptr(case_test, type_value, "td not in type field")) {
            break;
        }

        pass = Apt_compare_type(case_test, n, tf->type, type_value) && pass;

        tf = tf->next;
        field_value = field_value->next;
    }

    return pass;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_type_array(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    bool pass = true;

    Cent_value* is_const_value = Cent_value_get_str(value, "is_const");
    pass = Zinc_expect_boolean_equal(
        case_test,
        type->data.array.is_const,
        is_const_value->data.boolean,
        "is_const") && pass;

    Cent_value* dim_value = Cent_value_get_str(value, "dim");
    pass = Zinc_expect_uint64_t_equal(
        case_test,
        type->data.array.dim,
        dim_value->data.natural,
        "dim") && pass;

    Cent_value* type_value = Cent_value_get_str(value, "type");
    return Apt_compare_type(case_test, n, type->data.array.type, type_value) && pass;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_type_slice(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    bool pass = true;

    Cent_value* type_value = Cent_value_get_str(value, "type");
    return Apt_compare_type(case_test, n, type->data.slice.type, type_value) && pass;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_type_pointer(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    bool pass = true;

    Cent_value* type_value = Cent_value_get_str(value, "type");
    return Apt_compare_type(case_test, n, type->data.pointer.type, type_value) && pass;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Apt_compare_type_function(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value)
{
    bool pass = true;

    Ake_TypeParam* tp = type->data.function.input_head;
    Cent_value* input_value = Cent_value_get_str(value, "input");
    Cent_value* tp_value = input_value->data.dag.head;
    while (tp || tp_value) {
        if (!Zinc_expect_ptr(case_test, tp, "expected type param")) {
            pass = false;
            break;
        }
        if (!Zinc_expect_ptr(case_test, tp_value, "expected type param")) {
            pass = false;
            break;
        }

        Cent_value* name_value = Cent_value_get_str(tp_value, "name");
        pass = Zinc_expect_string(
            case_test,
            &tp->name,
            Zinc_string_c_str(&name_value->data.string),
            "name") && pass;

        Cent_value* tp_type_value = Cent_value_get_str(tp_value, "type");
        pass = Apt_compare_type(case_test, n, tp->type, tp_type_value) && pass;

        tp = tp->next;
        tp_value = tp_value->next;
    }

    Cent_value* output_value = Cent_value_get_str(value, "output");
    pass = Apt_compare_type(case_test, n, type->data.function.output, output_value) && pass;

    return pass;
}