#include "centipede/parse.h"
#include "centipede/build.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <zinc/input_unicode_string.h>
#include "zinc/os_unix.h"
#include "zinc/spec_error.h"
#include "cobble/match.h"
#include "data.h"
#include "zinc/string_slice.h"
#include <assert.h>
#include "zinc/os_unix.h"
#include "zinc/os_win.h"
#include "zinc/fs.h"
#include "lava/parse.h"
void Art_collect(Zinc_test* top_test, Zinc_string* path, Zinc_string* file_name);
void Art_test_suite_header(Zinc_test* top_test, Zinc_test* suite_test, Lava_dom* header);
void Art_test_suite_meta(Zinc_test* top_test, Zinc_test* suite_test, Cent_value* value);
void Art_test_header(Zinc_test* top_test, Zinc_test* suite_test, Lava_dom* header);
void Art_test_meta(Zinc_test* top_test, Zinc_test* suite_test, Zinc_test* case_test, Cent_value* value);

void Art_parse_files(Zinc_test* top_test)
{
    Art_top_data* top_data = top_test->data;

    Cob_re ext_re = Cob_compile_str("\\.md$");

    Zinc_string_list files;
    Zinc_string_list_init(&files);
    Zinc_list_files(Zinc_string_c_str(&top_data->dir_path), &files);

    Zinc_string_node* node = files.head;
    while (node) {
        if (!Zinc_string_compare_str(&node->value, ".") && !Zinc_string_compare_str(&node->value, "..")) {
            Zinc_string_slice name_slice = {
                .p = Zinc_string_c_str(&node->value),
                .size = node->value.size,
            };
            Cob_result ext_mr = Cob_match(&ext_re, name_slice);
            if (ext_mr.matched) {
                Zinc_string path;
                Zinc_string_init(&path);
                Zinc_string_add_string(&path, &top_data->dir_path);
                Zinc_path_append(&path, &node->value);
                Zinc_string_finish(&path);

                if (Zinc_is_reg_file(&path)) {
                    printf("%s\n", Zinc_string_c_str(&path));
                    Art_collect(top_test, &path, &node->value);
                }

                Zinc_string_destroy(&path);
            }
            Cob_result_destroy(&ext_mr);
        }
        node = node->next;
    }

    Zinc_string_list_destroy(&files);
    Cob_re_destroy(&ext_re);
}

void Art_collect(Zinc_test* top_test, Zinc_string* path, Zinc_string* file_name)
{
    Art_top_data* top_data = top_test->data;

    Art_suite_data* suite_data = NULL;
    Art_suite_create(&suite_data);
    Art_data_add(top_data, suite_data);

    Zinc_string_add_string(&suite_data->path, path);
    Zinc_string_add_string(&suite_data->name, file_name);

    Zinc_test* suite_test = NULL;
    Zinc_test_create(&suite_test);
    suite_data->test = suite_test;
    suite_test->data = suite_data;
    Zinc_test_add(top_data->test, suite_test);

    FILE* fp = fopen(path->buf, "r");
    if (!fp) {
        perror(path->buf);
        return;
    }

    Lava_result lr = Lava_parse_file(fp);

    if (!lr.root) {
        Zinc_error_list_set(&suite_data->errors, NULL, "root is null");
        Lava_result_destroy(&lr);
        return;
    }

    if (lr.root->kind != LAVA_DOM_HEADER) {
        Zinc_error_list_set(&suite_data->errors, NULL, "expected top level header");
        Lava_result_destroy(&lr);
        return;
    }

    if (lr.root->data.LAVA_DOM_HEADER.level != 1) {
        Zinc_error_list_set(&suite_data->errors, &lr.root->loc, "expected top level header");
        Lava_result_destroy(&lr);
        return;
    }

    Zinc_string_slice title = Zinc_string_get_slice(&lr.root->data.LAVA_DOM_HEADER.title);
    title = Zinc_trim(title);
    Zinc_string_slice title_ref = Zinc_string_slice_from_str("Test Suite");
    if (!Zinc_string_slice_compare(&title, &title_ref)) {
        Zinc_error_list_set(&suite_data->errors, &lr.root->loc, "expected test suite");
        Lava_result_destroy(&lr);
        return;
    }

    Art_test_suite_header(top_test, suite_test, lr.root);

    Lava_result_destroy(&lr);
}

void Art_test_suite_header(Zinc_test* top_test, Zinc_test* suite_test, Lava_dom* header)
{
    Art_top_data* top_data = top_test->data;
    Art_suite_data* suite_data = suite_test->data;

    Lava_dom* item = header->data.LAVA_DOM_HEADER.head;
    while (item) {
        if (item->kind == LAVA_DOM_TEXT) {
            if (suite_data->description.size > 0) {
                Zinc_string_add_char(&suite_data->description, '\n');
            }
            Zinc_string_add_string(&suite_data->description, &item->data.LAVA_DOM_TEXT);
        } else if (item->kind == LAVA_DOM_BACKQUOTE) {
            if (!Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "cent")) {
                Zinc_error_list_set(&suite_data->errors, &item->loc, "expected cent backquote");
                break;
            }
            Cent_comp_table* ct = NULL;
            FILE* fp = fopen(Zinc_string_c_str(&suite_data->path), "r");
            if (!fp) {
                Zinc_error_list_set(
                    &suite_data->errors,
                    &item->loc,
                    "failed to open file: %bf",
                    &suite_data->path);
                break;
            }
            Zinc_string name;
            Zinc_string_init(&name);
            Cent_comp_table_create_fp(&ct, &top_data->dir_path, &name, fp);
            Zinc_string_destroy(&name);
            Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
            Cent_comp_unit_parse(ct->primary);
            Cent_comp_unit_build(ct->primary);
            if (ct->primary->pr.errors->head) {
                Zinc_error* e = ct->primary->pr.errors->head;
                while (e) {
                    Zinc_error_list_set(&suite_data->errors, &e->loc, Zinc_string_c_str(&e->message));
                    e = e->next;
                }
                Cent_comp_table_destroy(ct);
                break;
            }
            Art_test_suite_meta(top_test, suite_test, ct->primary->value);
            Cent_comp_table_destroy(ct);
            free(ct);
        } else if (item->kind == LAVA_DOM_HEADER) {
            Art_test_header(top_test, suite_test, item);
        }

        item = item->next;
    }

    Zinc_string_add_string(&suite_data->test->name, &suite_data->description);
}

void Art_test_suite_meta(Zinc_test* top_test, Zinc_test* suite_test, Cent_value* value)
{
    Art_top_data* top_data = top_test->data;
    Art_suite_data* suite_data = suite_test->data;

    if (!value) {
        Zinc_error_list_set(&suite_data->errors, NULL, "expected test suite in backquote");
        return;
    }

    Cent_ast* value_n = value->n;
    if (value->type != Cent_value_type_dag) {
        Zinc_error_list_set(&suite_data->errors, &value_n->loc, "expected dag in backquote");
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "TestSuite")) {
        Zinc_error_list_set(&suite_data->errors, &value_n->loc, "expected test suite");
        return;
    }

    Cent_value* solo = Cent_value_get_str(value, "solo");
    if (!solo) {
        Zinc_error_list_set(&suite_data->errors, &value_n->loc, "expected solo property");
        return;
    }

    if (solo->type != Cent_value_type_boolean) {
        Zinc_error_list_set(&suite_data->errors, &value_n->loc, "expected solo to be boolean");
        return;
    }

    suite_data->test->solo = solo->data.boolean;
    if (suite_data->test->solo) {
        top_data->has_solo = true;
    }

    Cent_value* mute = Cent_value_get_str(value, "mute");
    if (!mute) {
        Zinc_error_list_set(&suite_data->errors, &value_n->loc, "expected mute");
        return;
    }

    if (mute->type != Cent_value_type_boolean) {
        Zinc_error_list_set(&suite_data->errors, &value_n->loc, "expected mute to be boolean");
        return;
    }

    suite_data->test->mute = mute->data.boolean;
}

void Art_test_header(Zinc_test* top_test, Zinc_test* suite_test, Lava_dom* header)
{
    Art_top_data* top_data = top_test->data;
    Art_suite_data* suite_data = suite_test->data;

    Zinc_string_slice title = Zinc_string_get_slice(&header->data.LAVA_DOM_HEADER.title);
    title = Zinc_trim(title);
    Zinc_string_slice title_ref = Zinc_string_slice_from_str("Test");
    if (!Zinc_string_slice_compare(&title, &title_ref)) {
        Zinc_error_list_set(&suite_data->errors, &header->loc, "expected test header");
        return;
    }

    if (header->data.LAVA_DOM_HEADER.level != 2) {
        Zinc_error_list_set(&suite_data->errors, &header->loc, "expected level 2 test header");
        return;
    }

    Art_case_data* case_data = NULL;
    Art_test_create(&case_data);
    Art_suite_add(suite_data, case_data);

    Zinc_test* case_test = NULL;
    Zinc_test_create(&case_test);
    case_data->test = case_test;
    case_test->data = case_data;
    Zinc_test_add(suite_data->test, case_test);

    Lava_dom* item = header->data.LAVA_DOM_HEADER.head;
    while (item) {
        if (item->kind == LAVA_DOM_TEXT) {
            if (case_data->description.size > 0) {
                Zinc_string_add_char(&case_data->description, '\n');
            }
            Zinc_string_add_string(&case_data->description, &item->data.LAVA_DOM_TEXT);
        } else if (item->kind == LAVA_DOM_BACKQUOTE) {
            if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "akela")) {
                case_data->source_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
            } else if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "llvm")) {
                case_data->llvm_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
                Zinc_string_add_string(&case_data->llvm, &item->data.LAVA_DOM_BACKQUOTE.text);
            } else if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "cent")) {
                FILE* fp = fopen(Zinc_string_c_str(&suite_data->path), "r");
                if (!fp) {
                    Zinc_error_list_set(
                        &suite_data->errors,
                        &item->loc,
                        "failed to open file: %bf",
                        &suite_data->path);
                }
                Cent_comp_table* ct = NULL;
                Cent_comp_table_create_fp(&ct, &top_data->dir_path, &suite_data->name, fp);
                Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
                Cent_comp_unit_parse(ct->primary);
                Cent_comp_unit_build(ct->primary);
                if (ct->primary->pr.errors->head) {
                    Zinc_error* e = ct->primary->pr.errors->head;
                    while (e) {
                        Zinc_error_list_set(&suite_data->errors, &e->loc, Zinc_string_c_str(&e->message));
                        e = e->next;
                    }
                } else {
                    if (!ct->primary->value) {
                        Zinc_error_list_set(&suite_data->errors, &item->loc, "expected value");
                    } else {
                        Art_test_meta(top_test, suite_test, case_test, ct->primary->value);
                    }
                }
                case_data->ct = ct;
            } else {
                Zinc_error_list_set(
                    &suite_data->errors,
                    &item->loc,
                    "unexpected format: %bf",
                    &item->data.LAVA_DOM_BACKQUOTE.format);
            }
        }

        item = item->next;
    }

    Zinc_string_add_string(&case_test->name, &case_data->description);
}

void Art_test_meta(Zinc_test* top_test, Zinc_test* suite_test, Zinc_test* case_test, Cent_value* value)
{
    Art_top_data* top_data = top_test->data;
    Art_suite_data* suite_data = suite_test->data;
    Art_case_data* case_data = case_test->data;

    Cent_ast* n = value->n;
    if (value->type != Cent_value_type_dag) {
        Zinc_error_list_set(&suite_data->errors, &n->loc, "expected DAG");
        case_data->has_error = true;
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "Test")) {
        Zinc_error_list_set(&suite_data->errors, &n->loc, "expected Test");
        case_data->has_error = true;
        return;
    }

    Cent_value* solo = Cent_value_get_str(value, "solo");
    if (!solo) {
        Zinc_error_list_set(&suite_data->errors, &n->loc, "expected solo");
        case_data->has_error = true;
    } else {
        if (solo->type != Cent_value_type_boolean) {
            Zinc_error_list_set(&suite_data->errors, &n->loc, "expected solo to be boolean");
            case_data->has_error = true;
        } else {
            case_data->test->solo = solo->data.boolean;
            if (case_data->test->solo) {
                suite_data->test->has_solo = true;
            }
        }
    }

    Cent_value* mute = Cent_value_get_str(value, "mute");
    if (!mute) {
        Zinc_error_list_set(&suite_data->errors, &n->loc, "expected mute");
        case_data->has_error = true;
    } else {
        if (mute->type != Cent_value_type_boolean) {
            Zinc_error_list_set(&suite_data->errors, &n->loc, "expected mute to be boolean");
            case_data->has_error = true;
        } else {
            case_data->test->mute = mute->data.boolean;
        }
    }

    Cent_value* snapshot = Cent_value_get_str(value, "snapshot");
    if (!snapshot) {
        Zinc_error_list_set(&suite_data->errors, &n->loc, "expected snapshot");
        case_data->has_error = true;
    } else {
        if (snapshot->type != Cent_value_type_boolean) {
            Zinc_error_list_set(&suite_data->errors, &n->loc, "expected snapshot to be boolean");
            case_data->has_error = true;
        } else {
            case_data->snapshot = snapshot->data.boolean;
        }
    }

    if (!case_data->has_error) {
        case_data->value = value->data.dag.tail;
    }
}