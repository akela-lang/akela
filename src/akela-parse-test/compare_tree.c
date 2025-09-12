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
bool Apt_compare_type_function(
    Zinc_test* case_test,
    Ake_Ast* n,
    Ake_Type* type,
    Cent_value* value);
void Apt_compare_value_string(Zinc_test* case_test, Ake_Ast* n, Zinc_string* actual, Cent_value* value);
bool Apt_compare_ast_list(Zinc_test* top_test, Zinc_test* case_test, Ake_AstList* n, Cent_value* value);

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
            &case_data->errors,
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
        if (ct->primary->errors.head) {
            Zinc_test_print_unseen(case_test);
            Zinc_error_list_print(&ct->primary->errors);
        }

        Zinc_string initial_line;
        Zinc_string_init(&initial_line);
        Zinc_string_add_str(&initial_line, "use lib::base::*");
        Ake_tree_print(ct->primary->root, &initial_line);
    } else {
        Cent_comp_table* expected_ct = NULL;
        fp = fopen(Zinc_string_c_str(&suite_data->path), "r");
        if (!fp) {
            Zinc_location loc;
            Zinc_location_init(&loc);
            Zinc_error_list_set(
                &case_data->errors,
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

        if (expected_ct->primary->errors.head) {
            Zinc_test_print_unseen(case_test);
            Zinc_error_list_print(&expected_ct->primary->errors);
        }

        if (case_data->has_error) {
            Apt_check_errors(
                top_test,
                case_test,
                &ct->primary->errors,
                expected_ct->primary->value);
        } else {
            if (pr.errors->head) {
                Zinc_error* e = pr.errors->head;
                while (e) {
                    Zinc_error_list_set(&case_data->errors, &e->loc, "%bf", &e->message);
                    e = e->next;
                }
                Zinc_test_print_unseen(case_test);
                Zinc_error_list_print(&case_data->errors);
                Ake_comp_table_free(ct);
                return;
            }

            if (expected_ct->primary->errors.head) {
                Zinc_error* e = expected_ct->primary->errors.head;
                while (e) {
                    Zinc_error_list_set(&case_data->errors, &e->loc, "%bf", &e->message);
                    e = e->next;
                }
                Zinc_test_print_unseen(case_test);
                Zinc_error_list_print(&case_data->errors);

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
        Zinc_test_print_unseen(case_test);
        Zinc_error_list_print(&case_data->errors);
    }

    if (case_data->spec_errors.head) {
        Zinc_test_print_unseen(case_test);
        Zinc_spec_error_list_print(&case_data->spec_errors);
    }
}

bool Apt_has_value(Ake_Ast* n)
{
    if (n->struct_value.size > 0) {
        return true;
    }

    return false;
}

Zinc_string* Apt_get_value(Ake_Ast* n)
{
    if (n->struct_value.size > 0) {
        return &n->struct_value;
    }

    return NULL;
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
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            NULL,
            "value is null");
        return false;
    } else {
        Zinc_expect_passed(case_test);
    }

    Cent_ast* value_n = value->n;

    if (!n && value) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            NULL,
            &value_n->loc,
            "node is null");
        return false;
    } else {
        Zinc_expect_passed(case_test);
    }

    if (!Zinc_string_compare_str(&value->name, "Ast")) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "expected AST value: %bf",
            &value->name);
        pass = false;
    } else {
        Zinc_expect_passed(case_test);
    }

    /* properties */
    Cent_value* tag = Cent_value_get_str(value, "@tag");
    assert(tag);
    if (n->kind != tag->data.enumeration.enum_value->value) {
        Zinc_expect_failed(case_test);
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
    } else {
        Zinc_expect_passed(case_test);
    }

    Cent_value* op = NULL;
    Cent_value* left = NULL;
    Cent_value* right = NULL;
    Cent_value* value2 = NULL;
    Ake_Ast* p = NULL;
    switch (n->kind) {
        case AKE_AST_NONE:
            break;
        case AKE_AST_ID:
            value2 = Cent_value_get_str(value, "value");
            Apt_compare_value_string(case_test, n, &n->data.id.value, value2);
            break;
        case AKE_AST_SIGN:
            op = Cent_value_get_str(value, "op");
            right = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.sign.op, op);
            Apt_compare_ast(top_test, case_test, n->data.sign.right, right);
            break;
        case AKE_AST_NUMBER:
            value2 = Cent_value_get_str(value, "value");
            Apt_compare_value_string(case_test, n, &n->data.number.value, value2);
            break;
        case AKE_AST_STRING:
            value2 = Cent_value_get_str(value, "value");
            Apt_compare_value_string(case_test, n, &n->data.string.value, value2);
            break;
        case AKE_AST_ASSIGN:
            left = Cent_value_get_str(value, "left");
            right = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.assign.left, left);
            Apt_compare_ast(top_test, case_test, n->data.assign.right, right);
            break;
        case AKE_AST_PLUS:
            left = Cent_value_get_str(value, "left");
            right = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.plus.left, left);
            Apt_compare_ast(top_test, case_test, n->data.plus.right, right);
            break;
        case AKE_AST_MINUS:
            left = Cent_value_get_str(value, "left");
            right = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.minus.left, left);
            Apt_compare_ast(top_test, case_test, n->data.minus.right, right);
            break;
        case AKE_AST_MULT:
            left = Cent_value_get_str(value, "left");
            right = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.mult.left, left);
            Apt_compare_ast(top_test, case_test, n->data.mult.right, right);
            break;
        case AKE_AST_DIVIDE:
            left = Cent_value_get_str(value, "left");
            right = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.divide.left, left);
            Apt_compare_ast(top_test, case_test, n->data.divide.right, right);
            break;
        case AKE_AST_STMTS:
            if (value->type == Cent_value_type_dag) {
                p = n->data.stmts.list.head;
                value2 = value->data.dag.head;
                while (p || value2) {
                    if (!p) {
                        Zinc_spec_error_list_set(
                            &case_data->spec_errors,
                            case_test,
                            NULL,
                            &value2->n->loc,
                            "actual expected");
                        break;
                    }
                    if (!value2) {
                        Zinc_spec_error_list_set(
                            &case_data->spec_errors,
                            case_test,
                            &p->loc,
                            NULL,
                            "value expected");
                        break;
                    }
                    Apt_compare_ast(top_test, case_test, p, value2);
                    p = p->next;
                    value2 = value2->next;
                }
            }
            break;
        case AKE_AST_FUNCTION:
            Cent_value* proto_value = Cent_value_get_str(value, "proto");
            Apt_compare_ast(top_test, case_test, n->data.function.proto, proto_value);

            Cent_value* body_value = Cent_value_get_str(value, "body");
            Apt_compare_ast(top_test, case_test, n->data.function.body, body_value);
            break;
        case AKE_AST_DSEQ:
            if (value->type == Cent_value_type_dag) {
                p = n->data.dseq.list.head;
                value2 = value->data.dag.head;
                while (p || value2) {
                    if (!p) {
                        Zinc_spec_error_list_set(
                            &case_data->spec_errors,
                            case_test,
                            NULL,
                            &value2->n->loc,
                            "actual expected");
                        break;
                    }
                    if (!value2) {
                        Zinc_spec_error_list_set(
                            &case_data->spec_errors,
                            case_test,
                            &p->loc,
                            NULL,
                            "value expected");
                        break;
                    }
                    Apt_compare_ast(top_test, case_test, p, value2);
                    p = p->next;
                    value2 = value2->next;
                }
            }
            break;
        case AKE_AST_DRET:
            value2 = Cent_value_get_str(value, "node");
            Apt_compare_ast(top_test, case_test, n->data.dret.node, value2);
            break;
        case AKE_AST_CALL:
            value2 = Cent_value_get_str(value, "func");
            Apt_compare_ast(top_test, case_test, n->data.call.func, value2);

            value2 = Cent_value_get_str(value, "args");
            Apt_compare_ast_list(top_test, case_test, &n->data.call.args, value2);
            break;
        case AKE_AST_IF:
            p = n->data._if_.branches.head;
            assert(value->type == Cent_value_type_dag);
            value2 = value->data.dag.head;
            while (p || value2) {
                if (!p) {
                    Zinc_spec_error_list_set(
                        &case_data->spec_errors,
                        case_test,
                        NULL,
                        &value2->n->loc,
                        "actual is NULL");
                    break;
                }

                if (!value2) {
                    Zinc_spec_error_list_set(
                        &case_data->spec_errors,
                        case_test,
                        &p->loc,
                        NULL,
                        "expected is NULL");
                    break;
                }

                Apt_compare_ast(top_test, case_test, p, value2);
                p = p->next;
                value2 = value2->next;
            }
            break;
        case AKE_AST_COND_BRANCH:
            value2 = Cent_value_get_str(value, "cond");
            Apt_compare_ast(top_test, case_test, n->data.cond_branch.cond, value2);

            value2 = Cent_value_get_str(value, "body");
            Apt_compare_ast(top_test, case_test, n->data.cond_branch.body, value2);
            break;
        case AKE_AST_DEFAULT_BRANCH:
            value2 = Cent_value_get_str(value, "body");
            Apt_compare_ast(top_test, case_test, n->data.default_branch.body, value2);
            break;
        case AKE_AST_EQUALITY:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.equality.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.equality.right, value2);
            break;
        case AKE_AST_NOT_EQUAL:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.not_equal.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.not_equal.right, value2);
            break;
        case AKE_AST_LESS_THAN:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.less_than.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.less_than.right, value2);
            break;
        case AKE_AST_LESS_THAN_OR_EQUAL:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.less_than_or_equal.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.less_than_or_equal.right, value2);
            break;
        case AKE_AST_GREATER_THAN:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.greater_than.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.greater_than.right, value2);
            break;
        case AKE_AST_GREATER_THAN_OR_EQUAL:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.greater_than_or_equal.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.greater_than_or_equal.right, value2);
            break;
        case AKE_AST_NOT:
            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data._not_.right, value2);
            break;
        case AKE_AST_AND:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data._and_.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data._and_.right, value2);
            break;
        case AKE_AST_OR:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data._or_.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data._or_.right, value2);
            break;
        case AKE_AST_WHILE:
            value2 = Cent_value_get_str(value, "cond");
            Apt_compare_ast(top_test, case_test, n->data._while_.cond, value2);

            value2 = Cent_value_get_str(value, "body");
            Apt_compare_ast(top_test, case_test, n->data._while_.body, value2);
            break;
        case AKE_AST_FOR_RANGE:
            value2 = Cent_value_get_str(value, "dec");
            Apt_compare_ast(top_test, case_test, n->data.for_range.dec, value2);

            value2 = Cent_value_get_str(value, "start");
            Apt_compare_ast(top_test, case_test, n->data.for_range.start, value2);

            value2 = Cent_value_get_str(value, "end");
            Apt_compare_ast(top_test, case_test, n->data.for_range.end, value2);

            value2 = Cent_value_get_str(value, "body");
            Apt_compare_ast(top_test, case_test, n->data.for_range.body, value2);
            break;
        case AKE_AST_FOR_ITERATION:
            value2 = Cent_value_get_str(value, "dec");
            Apt_compare_ast(top_test, case_test, n->data.for_iteration.dec, value2);

            value2 = Cent_value_get_str(value, "iterator");
            Apt_compare_ast(top_test, case_test, n->data.for_iteration.iterator, value2);

            value2 = Cent_value_get_str(value, "body");
            Apt_compare_ast(top_test, case_test, n->data.for_iteration.body, value2);
            break;
        case AKE_AST_DECLARATION:
            value2 = Cent_value_get_str(value, "id_node");
            Apt_compare_ast(top_test, case_test, n->data.declaration.id_node, value2);

            value2 = Cent_value_get_str(value, "type_node");
            Apt_compare_ast(top_test, case_test, n->data.declaration.type_node, value2);
            break;
        case AKE_AST_ARRAY_LITERAL:
            if (value->type == Cent_value_type_dag) {
                value2 = value->data.dag.head;
            }
            p = n->data.array_literal.list.head;
            while (p || value2) {
                Apt_compare_ast(top_test, case_test, p, value2);
                if (p) {
                    p = p->next;
                }
                if (value2) {
                    value2 = value2->next;
                }
            }
            break;
        case AKE_AST_ARRAY_SUBSCRIPT:
            value2 = Cent_value_get_str(value, "array");
            Apt_compare_ast(top_test, case_test, n->data.array_subscript.array, value2);

            value2 = Cent_value_get_str(value, "index");
            Apt_compare_ast(top_test, case_test, n->data.array_subscript.index, value2);
            break;
        case AKE_AST_BOOLEAN:
            value2 = Cent_value_get_str(value, "value");
            Apt_compare_value_string(case_test, n, &n->data.boolean.value, value2);
            break;
        case AKE_AST_PARENTHESIS:
            value2 = Cent_value_get_str(value, "expr");
            Apt_compare_ast(top_test, case_test, n->data.parenthesis.expr, value2);
            break;
        case AKE_AST_TYPE:
            break;
        case AKE_AST_POWER:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.power.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.power.right, value2);
            break;
        case AKE_AST_DOT:
            value2 = Cent_value_get_str(value, "left");
            Apt_compare_ast(top_test, case_test, n->data.dot.left, value2);

            value2 = Cent_value_get_str(value, "right");
            Apt_compare_ast(top_test, case_test, n->data.dot.right, value2);
            break;
        case AKE_AST_STRUCT:
            if (value->type == Cent_value_type_dag) {
                value2 = value->data.dag.head;
            } else {
                value2 = NULL;
            }
            p = n->data._struct_.list.head;
            while (p || value2) {
                Apt_compare_ast(top_test, case_test, p, value2);
                if (p) {
                    p = p->next;
                }
                if (value2) {
                    value2 = value2->next;
                }
            }
            break;
        case AKE_AST_RETURN:
            value2 = Cent_value_get_str(value, "expr");
            Apt_compare_ast(top_test, case_test, n->data._return_.expr, value2);
            break;
        case AKE_AST_PROTOTYPE:
            value2 = Cent_value_get_str(value, "id");
            Apt_compare_ast(top_test, case_test, n->data.prototype.id, value2);

            value2 = Cent_value_get_str(value, "dseq");
            Apt_compare_ast(top_test, case_test, n->data.prototype.dseq, value2);

            value2 = Cent_value_get_str(value, "ret");
            Apt_compare_ast(top_test, case_test, n->data.prototype.ret, value2);
            break;
        case AKE_AST_EXTERN:
            value2 = Cent_value_get_str(value, "proto");
            Apt_compare_ast(top_test, case_test, n->data._extern_.proto, value2);
            break;
        default:
            Cent_value* value_prop = Cent_value_get_str(value, "value");
            if (Apt_has_value(n)) {
                if (!value_prop) {
                    Zinc_expect_failed(case_test);
                    Zinc_spec_error_list_set(
                        &case_data->spec_errors,
                        case_test,
                        &n->loc,
                        &value_n->loc,
                        "value not expected");
                    pass = false;
                } else {
                    Zinc_expect_passed(case_test);
                    assert(value_prop->type == Cent_value_type_string);
                    if (!Zinc_string_compare(Apt_get_value(n), &value_prop->data.string)) {
                        Cent_ast* prop_n = value_prop->n;
                        Zinc_spec_error_list_set(
                            &case_data->spec_errors,
                            case_test,
                            &n->loc,
                            &prop_n->loc,
                            "AST values do not match (%bf) (%bf)",
                            Apt_get_value(n),
                            &value_prop->data.string);
                        pass = false;
                    } else {
                        Zinc_expect_passed(case_test);
                    }
                }
            } else {
                Zinc_expect_passed(case_test);
            }

            /* children */
            Ake_Ast* n2 = NULL;
            value2 = NULL;

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
            break;
    }

    Ake_Type* type = n->type;
    Cent_value* tu_value = Cent_value_get_str(value, "type");
    pass = Apt_compare_type(case_test, n, type, tu_value) && pass;

    return pass;
}

void Apt_compare_value_string(Zinc_test* case_test, Ake_Ast* n, Zinc_string* actual, Cent_value* value)
{
    Apt_case_data* case_data = case_test->data;

    if (value || actual->size > 0) {
        if (!value) {
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                NULL,
                "no expected value");
        } else if (actual->size == 0) {
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                NULL,
                &value->n->loc,
                "no actual value");
        } else {
            if (value->type == Cent_value_type_string) {
                Zinc_string* a = actual;
                Zinc_string* b = &value->data.string;
                if (!Zinc_string_compare(a, b)) {
                    Zinc_spec_error_list_set(
                        &case_data->spec_errors,
                        case_test,
                        &n->loc,
                        &value->n->loc,
                        "value not equal: (%bf) (%bf)",
                        a,
                        b);
                }
            } else {
                Zinc_test_print_unseen(case_test);
                fprintf(stderr,"expected string\n");
                Zinc_expect_failed(case_test);
            }
        }
    }
}

bool Apt_compare_ast_list(Zinc_test* top_test, Zinc_test* case_test, Ake_AstList* n, Cent_value* value)
{
    Apt_case_data* case_data = case_test->data;
    if (!n->head && !value) {
        return true;
    }

    if (!n->head) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            NULL,
            &value->n->loc,
            "actual AST List is NULL"
            );
        return false;
    }

    if (!value) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            NULL,
            "expected AST List is NULL"
            );
        return false;
    }

    if (value->type != Cent_value_type_dag) {
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value->n->loc,
            "expected AST List to be a dag"
            );
        return false;
    }

    bool pass = true;
    Ake_Ast* n2 = n->head;
    Cent_value* value2 = value->data.dag.head;
    while (n2 || value2) {
        pass = Apt_compare_ast(top_test, case_test, n2, value2) && pass;
        n2 = n2->next;
        value2 = value2->next;
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
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            NULL,
            "type use value is null: (%d-%s)-(NULL)",
            n->kind,
            Ast_type_name(n->kind));
        return false;
    } else {
        Zinc_expect_passed(case_test);
    }

    Cent_ast* value_n = value->n;

    if (!type && value) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "type use node is null");
    } else {
        Zinc_expect_passed(case_test);
    }

    if (!type) {
        return pass;
    }

    if (!Zinc_string_compare_str(&value->name, "Type")) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
        &case_data->spec_errors,
        case_test,
        &n->loc,
        &value_n->loc,
        "expected Type value: %bf",
        &value->name);
        pass = false;
    } else {
        Zinc_expect_passed(case_test);
    }

    Cent_value* name_value = Cent_value_get_str(value, "name");
    if (type && type->name.size > 0 && !name_value) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "name not set");
    } else {
        if (type && name_value && !Zinc_string_compare(&type->name, &name_value->data.string)) {
            Cent_ast* name_n = name_value->n;
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def name does not match (%bf) (%bf)",
                &type->name,
                &name_value->data.string);
            pass = false;
        } else {
            Zinc_expect_passed(case_test);
        }
    }

    Cent_value* tag = Cent_value_get_str(value, "@tag");
    if (type && Zinc_expect_size_t_equal(case_test, type->kind, tag->data.enumeration.number, "type tag")) {
        Zinc_expect_passed(case_test);
        return pass;
    } else {
        Zinc_expect_failed(case_test);
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
        case AKE_TYPE_FUNCTION:
            return Apt_compare_type_function(case_test, n, type, value);
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
    Cent_ast* value_n = value->n;

    bool pass = true;

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_natural);
        if (type->data.integer.bit_count != bit_count_value->data.natural) {
            Zinc_expect_failed(case_test);
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def integer bit_count does not match (%d) (%d)",
                type->data.integer.bit_count,
                bit_count_value->data.integer);
            pass = false;
        } else {
            Zinc_expect_passed(case_test);
        }
    } else if (type->data.integer.bit_count > 0) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "bit_count not set");
        pass = false;
    } else {
        Zinc_expect_passed(case_test);
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
    Cent_ast* value_n = value->n;

    bool pass = true;

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_natural);
        if (type->data.natural.bit_count != bit_count_value->data.natural) {
            Zinc_expect_failed(case_test);
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def natural bit_count does not match (%d) (%d)",
                type->data.natural.bit_count,
                bit_count_value->data.natural);
            pass = false;
        } else {
            Zinc_expect_passed(case_test);
        }
    } else if (type->data.natural.bit_count > 0) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "bit_count not set");
        pass = false;
    } else {
        Zinc_expect_passed(case_test);
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
    Cent_ast* value_n = value->n;

    bool pass = true;

    Cent_value* bit_count_value = Cent_value_get_str(value, "bit_count");
    if (bit_count_value) {
        assert(bit_count_value->type == Cent_value_type_natural);
        if (type->data.real.bit_count != bit_count_value->data.natural) {
            Zinc_expect_failed(case_test);
            Zinc_spec_error_list_set(
                &case_data->spec_errors,
                case_test,
                &n->loc,
                &value_n->loc,
                "type def real bit_count does not match (%d) (%d)",
                type->data.real.bit_count,
                bit_count_value->data.real);
            pass = false;
        } else {
            Zinc_expect_passed(case_test);
        }
    } else if (type->data.real.bit_count > 0) {
        Zinc_expect_failed(case_test);
        Zinc_spec_error_list_set(
            &case_data->spec_errors,
            case_test,
            &n->loc,
            &value_n->loc,
            "bit_count not set");
        pass = false;
    } else {
        Zinc_expect_passed(case_test);
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
    Cent_ast* value_n = value->n;

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
