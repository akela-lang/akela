#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <sys/stat.h>
#include <zinc/os_win.h>
#include "zinc/input_unicode_file.h"
#include "centipede/comp_table.h"
#include "compare.h"
#include "zinc/fs.h"
#include "zinc/string_list.h"
#include "cobble/match.h"
#include "data.h"
#include "lava/parse.h"
#include "compare.h"

void Apt_parse_suite(
    Zinc_test* top_test,
    Zinc_string* test_cases_path,
    Zinc_string* path,
    Zinc_string* name);
void Apt_parse_suite_meta(
    Zinc_test* top_test,
    Zinc_test *suite_test,
    Cent_value* value);
void Apt_parse_case(
    Zinc_test* top_test,
    Zinc_string* path,
    Zinc_test* suite_test,
    Lava_dom* dom,
    Zinc_string* name);
void Apt_parse_case_meta(Zinc_test* top_test, Zinc_test* case_test, Cent_value* value);

void Apt__parse_suite_meta_prop__(Zinc_string* name, Cent_value* prop);
void Apt__parse_case_meta_prop__(Zinc_string* name, Cent_value* prop);
Zinc_test* Apt__parse_top_test__ = NULL;
Zinc_test* Apt__parse_suite_test__ = NULL;
Zinc_test* Apt__parse_case_test__ = NULL;

void Apt_parse_files(Zinc_test* top_test, Zinc_string* test_cases_path)
{
    Zinc_string_list files;
    Zinc_string_list_init(&files);
    Zinc_list_files(Zinc_string_c_str(test_cases_path), &files);

    Apt_top_data* top_data = top_test->data;

    Zinc_string_node* node = files.head;
    while (node) {
        if (!Zinc_string_compare_str(&node->value, ".") && !Zinc_string_compare_str(&node->value, "..")) {
            Zinc_string path;
            Zinc_string_init(&path);
            Zinc_string_add_string(&path, test_cases_path);
            Zinc_path_append(&path, &node->value);

            if (Zinc_is_reg_file(&path)) {
                Apt_parse_suite(top_test, test_cases_path, &path, &node->value);
            }

            Zinc_string_destroy(&path);
        }

        node = node->next;
    }

    Zinc_string_list_destroy(&files);
}

void Apt_parse_suite(
    Zinc_test* top_test,
    Zinc_string* test_cases_path,
    Zinc_string* path,
    Zinc_string* name)
{
    Apt_top_data* top_data = top_test->data;

    printf("%s\n", Zinc_string_c_str(path));
    FILE* fp = fopen(Zinc_string_c_str(path), "r");
    if (!fp) {
        perror(Zinc_string_c_str(path));
        return;
    }

    Lava_result pr = Lava_parse_file(fp);
    if (pr.errors->head) {
        Zinc_error_list_print(pr.errors);
        Lava_result_destroy(&pr);
        return;
    }

    Zinc_test* suite_test = NULL;
    Zinc_test_create(&suite_test);
    Zinc_test_add(top_test, suite_test);

    Apt_suite_data* suite_data = NULL;
    Apt_suite_data_create(&suite_data);
    suite_test->data = suite_data;
    Zinc_string_add_string(&suite_data->path, path);

    if (pr.root->kind != LAVA_DOM_HEADER) {
        Zinc_error_list_set(pr.errors, &pr.root->loc, "expected header");
        Zinc_error_list_print(pr.errors);
        Lava_result_destroy(&pr);
        return;
    }

    Lava_dom* header = pr.root;

    if (header->data.LAVA_DOM_HEADER.level != 1) {
        Zinc_error_list_set(pr.errors, &pr.root->loc, "expected level 1 header");
        Zinc_error_list_print(pr.errors);
        Lava_result_destroy(&pr);
        return;
    }

    Zinc_string_slice title = Zinc_string_get_slice(&header->data.LAVA_DOM_HEADER.title);
    title = Zinc_trim(title);
    Zinc_string_slice test_suite_slice = Zinc_string_slice_from_str("Test Suite");

    if (!Zinc_string_slice_compare(&title, &test_suite_slice)) {
        Zinc_error_list_set(pr.errors, &pr.root->loc, "expected test suite");
        Zinc_error_list_print(pr.errors);
        Lava_result_destroy(&pr);
        return;
    }

    if (pr.root->kind == LAVA_DOM_HEADER) {
        Zinc_string_add_string(&suite_data->name, path);

        Lava_dom* item = header->data.LAVA_DOM_HEADER.head;
        while (item) {
            if (item->kind == LAVA_DOM_TEXT) {
                if (suite_data->description.size > 0) {
                    Zinc_string_add_char(&suite_data->description, '\n');
                }
                Zinc_string_add_string(&suite_data->description, &item->data.LAVA_DOM_TEXT);
            } else if (item->kind == LAVA_DOM_BACKQUOTE) {
                Zinc_string_slice format = Zinc_string_get_slice(&item->data.LAVA_DOM_BACKQUOTE.format);
                format = Zinc_trim(format);
                Zinc_string_slice cent = Zinc_string_slice_from_str("cent");
                if (!Zinc_string_slice_compare(&format, &cent)) {
                    Zinc_error_list_set(pr.errors, &pr.root->loc, "expected cent format");
                    Zinc_error_list_print(pr.errors);
                    Lava_result_destroy(&pr);
                    return;
                }
                Cent_comp_table* ct = NULL;
                fp = fopen(Zinc_string_c_str(path), "r");
                Cent_comp_table_create_fp(&ct, test_cases_path, name, fp);
                Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
                Cent_comp_unit_parse(ct->primary);
                Cent_comp_unit_build(ct->primary);
                if (ct->primary->errors.head) {
                    Zinc_error_list_print(&ct->primary->errors);
                }
                Apt_parse_suite_meta(top_test, suite_test, ct->primary->value);
                Cent_comp_table_destroy(ct);
                free(ct);
            } else if (item->kind == LAVA_DOM_HEADER) {
                Zinc_string_slice test_title = Zinc_string_get_slice(&item->data.LAVA_DOM_HEADER.title);
                test_title = Zinc_trim(test_title);
                Zinc_string_slice expected_title = Zinc_string_slice_from_str("Test");
                if (!Zinc_string_slice_compare(&test_title, &expected_title)) {
                    Zinc_error_list_set(&top_data->errors, &item->loc, "expected Test");
                } else if (item->data.LAVA_DOM_HEADER.level != 2) {
                    Zinc_error_list_set(&top_data->errors, &item->loc, "expected level 2");
                } else {
                    Apt_parse_case(top_test, path, suite_test, item, name);
                }
            }

            item = item->next;
        }
    }

    Lava_result_destroy(&pr);

    if (suite_test->solo) {
        top_test->has_solo = true;
    }

    Zinc_string_add_string(&suite_test->name, &suite_data->description);
    suite_test->func = Apt_suite_run;
}

void Apt_parse_suite_meta(Zinc_test* top_test, Zinc_test* suite_test, Cent_value* value)
{
    Apt_top_data* top_data = top_test->data;

    if (value->type == Cent_value_type_dag) {
        if (Zinc_string_compare_str(&value->name, "TestSuite")) {
            Apt__parse_top_test__ = top_test;
            Apt__parse_suite_test__ = suite_test;
            Zinc_hash_map_string_map_name(
                &value->data.dag.properties,
                (Zinc_hash_map_string_func_name)Apt__parse_suite_meta_prop__);
        } else {
            Cent_ast* n = value->n;
            Zinc_error_list_set(&top_data->errors, &n->loc, "expected TestSuite");
        }
    }
}

void Apt__parse_suite_meta_prop__(Zinc_string* name, Cent_value* prop)
{
    Zinc_test* top_test = Apt__parse_top_test__;
    Zinc_test* suite_test = Apt__parse_suite_test__;
    Apt_top_data* top_data = top_test->data;
    Apt_suite_data* suite_data = suite_test->data;

    if (Zinc_string_compare_str(name, "solo")) {
        if (prop->type == Cent_value_type_boolean) {
            suite_test->solo = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&top_data->errors, &n->loc, "expected boolean");
        }
    } else if (Zinc_string_compare_str(name, "mute")) {
        if (prop->type == Cent_value_type_boolean) {
            suite_test->mute = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&top_data->errors, &n->loc, "expected boolean");
        }
    } else {
        Cent_ast* n = prop->n;
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected solo or mute");
    }
}

void Apt_parse_case(
    Zinc_test* top_test,
    Zinc_string* path,
    Zinc_test* suite_test,
    Lava_dom* dom,
    Zinc_string* name)
{
    Apt_top_data* top_data = top_test->data;
    Apt_suite_data* suite_data = suite_test->data;

    Zinc_test* case_test = NULL;
    Zinc_test_create(&case_test);
    Zinc_test_add(suite_test, case_test);

    Apt_case_data* case_data = NULL;
    Apt_case_data_create(&case_data);
    case_test->data = case_data;

    bool seen_meta = false;

    Lava_dom* item = dom->data.LAVA_DOM_HEADER.head;
    while (item) {
        if (item->kind == LAVA_DOM_TEXT) {
            if (case_data->description.size > 0) {
                Zinc_string_add_char(&case_data->description, '\n');
            }
            Zinc_string_add_string(&case_data->description, &item->data.LAVA_DOM_TEXT);
        } else if (item->kind == LAVA_DOM_BACKQUOTE) {
            if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "cent")) {
                if (!seen_meta) {
                    seen_meta = true;
                    Cent_comp_table* ct = NULL;
                    FILE* fp = fopen(Zinc_string_c_str(path), "r");
                    Cent_comp_table_create_fp(&ct, &top_data->dir_path, name, fp);
                    Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
                    Cent_comp_unit_parse(ct->primary);
                    if (!ct->primary->errors.head) {
                        Cent_comp_unit_build(ct->primary);
                        Apt_parse_case_meta(top_test, case_test, ct->primary->value);
                    } else {
                        Zinc_error* e = ct->primary->errors.head;
                        while (e) {
                            Zinc_error_list_set(
                                &top_data->errors,
                                &e->loc,
                                "%s", Zinc_string_c_str(&e->message));
                            e = e->next;
                        }
                    }
                    Cent_comp_table_destroy(ct);
                    free(ct);
                } else {
                    case_data->ast_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
                }
            } else if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "akela")) {
                case_data->source_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
            }
        }

        item = item->next;
    }

    if (case_test->solo) {
        suite_test->has_solo = true;
    }

    Zinc_string_add_string(&case_test->name, &case_data->description);
    case_test->func = Apt_case_run;
}

void Apt_parse_case_meta(Zinc_test* top_test, Zinc_test* case_test, Cent_value* value)
{
    Apt_top_data* top_data = top_test->data;
    Apt_case_data* case_data = case_test->data;

    if (!value) {
        Zinc_error_list_set(&top_data->errors, NULL, "expected a value");
        return;
    }

    if (value->type != Cent_value_type_dag) {
        Cent_ast* n = value->n;
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected DAG type");
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "Test")) {
        Cent_ast* n = value->n;
        Zinc_error_list_set(&top_data->errors, &n->loc, "expected Test");
        return;
    }

    Apt__parse_top_test__ = top_test;
    Apt__parse_case_test__ = case_test;
    Zinc_hash_map_string_map_name(
        &value->data.dag.properties,
        (Zinc_hash_map_string_func_name)Apt__parse_case_meta_prop__);
}

void Apt__parse_case_meta_prop__(Zinc_string* name, Cent_value* prop)
{
    Zinc_test* top_test = Apt__parse_top_test__;
    Zinc_test* case_test = Apt__parse_case_test__;
    Apt_top_data* top_data = top_test->data;
    Apt_case_data* case_data = case_test->data;

    if (Zinc_string_compare_str(name, "solo")) {
        if (prop->type == Cent_value_type_boolean) {
            case_test->solo = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&top_data->errors, &n->loc, "expected boolean");
        }
    } else if (Zinc_string_compare_str(name, "mute")) {
        if (prop->type == Cent_value_type_boolean) {
            case_test->mute = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&top_data->errors, &n->loc, "expected boolean");
        }
    } else if (Zinc_string_compare_str(name, "snapshot")) {
        if (prop->type == Cent_value_type_boolean) {
            case_data->snapshot = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&top_data->errors, &n->loc, "expected boolean");
        }
    } else if (Zinc_string_compare_str(name, "has_error")) {
        if (prop->type == Cent_value_type_boolean) {
            case_data->has_error = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&top_data->errors, &n->loc, "expected boolean");
        }
    } else {
        Cent_ast* n = prop->n;
        Zinc_error_list_set(&top_data->errors, &n->loc, "invalid property: %bf", name);
    }
}