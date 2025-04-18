#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <centipede/base.h>
#include <sys/stat.h>
#include <zinc/os_win.h>

#include "zinc/input_unicode_file.h"
#include "centipede/module_finder_file.h"
#include "centipede/comp_table.h"
#include "zinc/os_unix.h"
#include "zinc/spec_error.h"
#include "compare.h"
#include "zinc/fs.h"
#include "zinc/string_list.h"
#include "cobble/compile.h"
#include "cobble/match.h"
#include "parse_tools.h"
#include "data.h"
#include "lava/parse.h"
#include "centipede/parse.h"

#define NAME "akela-parse-test"


void Apt_parse_test_suite(Apt_data* data, Zinc_string* path, Zinc_string* name);
void Apt_parse_test(
    Apt_data* data,
    Zinc_string* path,
    Apt_test_suite* ts,
    Lava_dom* dom,
    Zinc_string* name);
void Apt_parse_test_suite_meta(Apt_data* data, Apt_test_suite *suite, Cent_value* value);
void Apt_parse_test_case_meta(Apt_data* data, Apt_test_case* tc, Cent_value* value);
void Apt_parse_test_case_meta_prop(Zinc_string* name, Cent_value* prop);

bool Apt_validate_directory(char* path)
{
    if (!Zinc_file_exists(path)) {
        perror(path);
        Zinc_string cwd;
        Zinc_string_init(&cwd);
        Zinc_get_cwd(&cwd);
        Zinc_string_finish(&cwd);
        fprintf(stderr, "current working directory: %s\n", cwd.buf);
        Zinc_string_destroy(&cwd);
        return false;
    }

    bool is_dir;
    Zinc_result r = Zinc_is_directory(path, &is_dir);
    if (r == Zinc_result_error) {
        printf("%s\n", Zinc_error_message);
        return false;
    }

    if (!is_dir) {
        fprintf(stderr, "%s is not a directory", path);
        return false;
    }

    printf("%s\n", path);

    return true;
}

void Apt_parse_files(Apt_data* data)
{
    Zinc_string_list files;
    Zinc_string_list_init(&files);
    Zinc_list_files(Zinc_string_c_str(&data->dir_path), &files);

    Zinc_string_node* node = files.head;
    while (node) {
        if (!Zinc_string_compare_str(&node->value, ".") && !Zinc_string_compare_str(&node->value, "..")) {
            Zinc_string path;
            Zinc_string_init(&path);
            Zinc_string_add_string(&path, &data->dir_path);
            Zinc_path_append(&path, &node->value);

            if (Zinc_is_reg_file(&path)) {
                Apt_parse_test_suite(data, &path, &node->value);
            }

            Zinc_string_destroy(&path);
        }

        node = node->next;
    }

    Zinc_string_list_destroy(&files);
}

void Apt_parse_test_suite(Apt_data* data, Zinc_string* path, Zinc_string* name)
{
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

    Apt_test_suite* suite = NULL;
    Apt_test_suite_create(&suite);
    Apt_suite_list_add(&data->suites, suite);

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
        Zinc_string_add_string(&suite->name, path);

        Lava_dom* item = header->data.LAVA_DOM_HEADER.head;
        while (item) {
            if (item->kind == LAVA_DOM_TEXT) {
                if (suite->description.size > 0) {
                    Zinc_string_add_char(&suite->description, '\n');
                }
                Zinc_string_add_string(&suite->description, &item->data.LAVA_DOM_TEXT);
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
                Cent_comp_table_create_fp(&ct, &data->dir_path, name, fp);
                Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
                Cent_comp_unit_parse(ct->primary);
                Cent_comp_unit_build(ct->primary);
                if (ct->primary->errors.head) {
                    Zinc_error_list_print(&ct->primary->errors);
                }
                Apt_parse_test_suite_meta(data, suite, ct->primary->value);
                Cent_comp_table_destroy(ct);
                free(ct);
            } else if (item->kind == LAVA_DOM_HEADER) {
                Zinc_string_slice test_title = Zinc_string_get_slice(&item->data.LAVA_DOM_HEADER.title);
                test_title = Zinc_trim(test_title);
                Zinc_string_slice expected_title = Zinc_string_slice_from_str("Test");
                if (!Zinc_string_slice_compare(&test_title, &expected_title)) {
                    Zinc_error_list_set(&data->errors, &item->loc, "expected Test");
                } else if (item->data.LAVA_DOM_HEADER.level != 2) {
                    Zinc_error_list_set(&data->errors, &item->loc, "expected level 2");
                } else {
                    Apt_parse_test(data, path, suite, item, name);
                }
            }

            item = item->next;
        }
    }

    Lava_result_destroy(&pr);

    if (suite->solo) {
        data->has_suite_solo = true;
    }
}

void Apt__parse_test_suite_meta_prop(Zinc_string* name, Cent_value* prop);
Apt_data* Apt__parse_test_suite_meta_prop_data = NULL;
Apt_test_suite* Apt__parse_test_suite_meta_prop_suite = NULL;
void Apt_parse_test_suite_meta(Apt_data* data, Apt_test_suite *suite, Cent_value* value)
{
    if (value->type == Cent_value_type_dag) {
        if (Zinc_string_compare_str(&value->name, "TestSuite")) {
            Apt__parse_test_suite_meta_prop_data = data;
            Apt__parse_test_suite_meta_prop_suite = suite;
            Zinc_hash_map_string_map_name(
                &value->data.dag.properties,
                (Zinc_hash_map_string_func_name)Apt__parse_test_suite_meta_prop);
        } else {
            Cent_ast* n = value->n;
            Zinc_error_list_set(&data->errors, &n->loc, "expected TestSuite");
        }
    }
}

void Apt__parse_test_suite_meta_prop(Zinc_string* name, Cent_value* prop)
{
    Apt_data* data = Apt__parse_test_suite_meta_prop_data;
    Apt_test_suite* suite = Apt__parse_test_suite_meta_prop_suite;
    if (Zinc_string_compare_str(name, "solo")) {
        if (prop->type == Cent_value_type_boolean) {
            suite->solo = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&data->errors, &n->loc, "expected boolean");
        }
    } else if (Zinc_string_compare_str(name, "mute")) {
        if (prop->type == Cent_value_type_boolean) {
            suite->mute = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&data->errors, &n->loc, "expected boolean");
        }
    } else {
        Cent_ast* n = prop->n;
        Zinc_error_list_set(&data->errors, &n->loc, "expected solo or mute");
    }
}

void Apt_parse_test(
    Apt_data* data,
    Zinc_string* path,
    Apt_test_suite* ts,
    Lava_dom* dom,
    Zinc_string* name)
{
    Apt_test_case* tc = NULL;
    Apt_test_case_create(&tc);
    Apt_test_list_add(&ts->list, tc);
    bool seen_meta = false;
    bool seen_source = false;
    bool seen_ast = false;

    Lava_dom* item = dom->data.LAVA_DOM_HEADER.head;
    while (item) {
        if (item->kind == LAVA_DOM_TEXT) {
            if (tc->description.size > 0) {
                Zinc_string_add_char(&tc->description, '\n');
            }
            Zinc_string_add_string(&tc->description, &item->data.LAVA_DOM_TEXT);
        } else if (item->kind == LAVA_DOM_BACKQUOTE) {
            if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "cent")) {
                if (!seen_meta) {
                    seen_meta = true;
                    Cent_comp_table* ct = NULL;
                    FILE* fp = fopen(Zinc_string_c_str(path), "r");
                    Cent_comp_table_create_fp(&ct, &data->dir_path, name, fp);
                    Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
                    Cent_comp_unit_parse(ct->primary);
                    Cent_comp_unit_build(ct->primary);
                    Apt_parse_test_case_meta(data, tc, ct->primary->value);
                    Cent_comp_table_destroy(ct);
                    free(ct);
                } else {
                    seen_ast = true;
                    Zinc_string_add_string(&tc->ast_path, path);
                    tc->ast_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
                }
            } else if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "akela")) {
                seen_source = true;
                Zinc_string_add_string(&tc->source_path, path);
                tc->source_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
            }
        }

        item = item->next;
    }

    if (tc->solo) {
        data->has_test_solo = true;
    }
}

Apt_data* Apt__parse_test_case_data = NULL;
Apt_test_case* Apt__parse_test_case_tc = NULL;
void Apt_parse_test_case_meta(Apt_data* data, Apt_test_case* tc, Cent_value* value)
{
    if (value->type != Cent_value_type_dag) {
        Cent_ast* n = value->n;
        Zinc_error_list_set(&data->errors, &n->loc, "expected DAG type");
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "Test")) {
        Cent_ast* n = value->n;
        Zinc_error_list_set(&data->errors, &n->loc, "expected Test");
        return;
    }

    Apt__parse_test_case_data = data;
    Apt__parse_test_case_tc = tc;
    Zinc_hash_map_string_map_name(
        &value->data.dag.properties,
        (Zinc_hash_map_string_func_name)Apt_parse_test_case_meta_prop);
}

void Apt_parse_test_case_meta_prop(Zinc_string* name, Cent_value* prop)
{
    Apt_data* data = Apt__parse_test_suite_meta_prop_data;
    Apt_test_case* tc = Apt__parse_test_case_tc;

    if (Zinc_string_compare_str(name, "solo")) {
        if (prop->type == Cent_value_type_boolean) {
            tc->solo = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&data->errors, &n->loc, "expected boolean");
        }
    } else if (Zinc_string_compare_str(name, "mute")) {
        if (prop->type == Cent_value_type_boolean) {
            tc->mute = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&data->errors, &n->loc, "expected boolean");
        }
    } else if (Zinc_string_compare_str(name, "has_error")) {
        if (prop->type == Cent_value_type_boolean) {
            tc->has_error = prop->data.boolean;
        } else {
            Cent_ast* n = prop->n;
            Zinc_error_list_set(&data->errors, &n->loc, "expected boolean");
        }
    } else {
        Cent_ast* n = prop->n;
        Zinc_error_list_set(&data->errors, &n->loc, "invalid property: %bf", name);
    }
}