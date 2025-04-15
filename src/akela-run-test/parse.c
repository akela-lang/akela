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
#include "cent.h"
#include "zinc/os_unix.h"
#include "zinc/os_win.h"
#include "zinc/fs.h"
#include "lava/parse.h"

void Art_test_suite_header(Art_data* data, Art_suite* suite, Lava_dom* header);
void Art_test_suite_meta(Art_data* data, Art_suite* suite, Cent_value* value);
void Art_test_header(Art_data* data, Art_suite* suite, Lava_dom* header);
void Art_test_meta(Art_data* data, Art_suite* suite, Art_test* test, Cent_value* value);

void Run_collect(
    Art_data* data,
    Zinc_string* dir_path,
    Zinc_string* path,
    Zinc_string* file_name);

bool Run_validate_directory(const char* path)
{
    struct stat sb;
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
        fprintf(stderr, "%s\n", Zinc_error_message);
        return false;
    }

    if (!is_dir) {
        fprintf(stderr, "%s is not a directory", path);
        return false;
    }

    return true;
}

void Run_parse_files(Art_data* data, char* dir_name)
{
    Cob_re ext_re = Cob_compile_str("\\.md$");

    Zinc_string dir_path;
    Zinc_string_init(&dir_path);
    Zinc_string_add_str(&dir_path, dir_name);

    Zinc_string_list files;
    Zinc_string_list_init(&files);
    Zinc_list_files(Zinc_string_c_str(&dir_path), &files);

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
                Zinc_string_add_str(&path, dir_name);
                Zinc_path_append(&path, &node->value);
                Zinc_string_finish(&path);

                if (Zinc_is_reg_file(&path)) {
                    printf("%s\n", Zinc_string_c_str(&path));
                    Run_collect(data, &dir_path, &path, &node->value);
                }

                Zinc_string_destroy(&path);
            }
            Cob_result_destroy(&ext_mr);
        }
        node = node->next;
    }

    Zinc_string_destroy(&dir_path);
    Zinc_string_list_destroy(&files);
}

void Run_collect(Art_data* data, Zinc_string* dir_path, Zinc_string* path, Zinc_string* file_name)
{
    Zinc_vector_expand(&data->suites, 1);
    size_t index = data->suites.count;
    data->suites.count++;
    Art_suite* suite = (Art_suite*)ZINC_VECTOR_PTR(&data->suites, index);
    Art_suite_init(suite);

    Zinc_string_add_string(&suite->path, path);
    Zinc_string_add_string(&suite->name, file_name);

    FILE* fp = fopen(path->buf, "r");
    if (!fp) {
        perror(path->buf);
        return;
    }

    Lava_result lr = Lava_parse_file(fp);

    if (!lr.root) {
        Zinc_error_list_set(&suite->errors, NULL, "root is null");
        Lava_result_destroy(&lr);
        return;
    }

    if (lr.root->kind != LAVA_DOM_HEADER) {
        Zinc_error_list_set(&suite->errors, NULL, "expected top level header");
        Lava_result_destroy(&lr);
        return;
    }

    if (lr.root->data.LAVA_DOM_HEADER.level != 1) {
        Zinc_error_list_set(&suite->errors, &lr.root->loc, "expected top level header");
        Lava_result_destroy(&lr);
        return;
    }

    Zinc_string_slice title = Zinc_string_get_slice(&lr.root->data.LAVA_DOM_HEADER.title);
    title = Zinc_trim(title);
    Zinc_string_slice title_ref = Zinc_string_slice_from_str("Test Suite");
    if (!Zinc_string_slice_compare(&title, &title_ref)) {
        Zinc_error_list_set(&suite->errors, &lr.root->loc, "expected test suite");
        Lava_result_destroy(&lr);
        return;
    }

    Art_test_suite_header(data, suite, lr.root);

    Lava_result_destroy(&lr);
}

void Art_test_suite_header(Art_data* data, Art_suite* suite, Lava_dom* header)
{
    for (size_t i = 0; i < header->data.LAVA_DOM_HEADER.items.count; i++) {
        Lava_dom* item = (Lava_dom*)ZINC_VECTOR_PTR(&header->data.LAVA_DOM_HEADER.items, i);
        if (item->kind == LAVA_DOM_TEXT) {
            if (suite->description.size > 0) {
                Zinc_string_add_char(&suite->description, '\n');
            }
            Zinc_string_add_string(&suite->description, &item->data.LAVA_DOM_TEXT);
        } else if (item->kind == LAVA_DOM_BACKQUOTE) {
            if (!Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "cent")) {
                Zinc_error_list_set(&suite->errors, &item->loc, "expected cent backquote");
                break;
            }
            Cent_comp_table* ct = NULL;
            FILE* fp = fopen(Zinc_string_c_str(&suite->path), "r");
            if (!fp) {
                Zinc_error_list_set(
                    &suite->errors,
                    &item->loc,
                    "failed to open file: %bf",
                    &suite->path);
                break;
            }
            Zinc_string name;
            Zinc_string_init(&name);
            Cent_comp_table_create_fp(&ct, &data->dir_path, &name, fp);
            Zinc_string_destroy(&name);
            Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
            Cent_comp_unit_parse(ct->primary);
            Cent_comp_unit_build(ct->primary);
            if (ct->primary->pr.errors->head) {
                Zinc_error* e = ct->primary->pr.errors->head;
                while (e) {
                    Zinc_error_list_set(&suite->errors, &e->loc, Zinc_string_c_str(&e->message));
                    e = e->next;
                }
                Cent_comp_table_destroy(ct);
                break;
            }
            Art_test_suite_meta(data, suite, ct->primary->value);
            Cent_comp_table_destroy(ct);
        } else if (item->kind == LAVA_DOM_HEADER) {
            Art_test_header(data, suite, item);
        }
    }
}

void Art_test_suite_meta(Art_data* data, Art_suite* suite, Cent_value* value)
{
    if (!value) {
        Zinc_error_list_set(&suite->errors, NULL, "expected test suite in backquote");
        return;
    }

    Cent_ast* value_n = value->n;
    if (value->type != Cent_value_type_dag) {
        Zinc_error_list_set(&suite->errors, &value_n->loc, "expected dag in backquote");
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "TestSuite")) {
        Zinc_error_list_set(&suite->errors, &value_n->loc, "expected test suite");
        return;
    }

    Cent_value* solo = Cent_value_get_str(value, "solo");
    if (!solo) {
        Zinc_error_list_set(&suite->errors, &value_n->loc, "expected solo property");
        return;
    }

    if (solo->type != Cent_value_type_boolean) {
        Zinc_error_list_set(&suite->errors, &value_n->loc, "expected solo to be boolean");
        return;
    }

    suite->solo = solo->data.boolean;
    if (suite->solo) {
        data->has_solo = true;
    }

    Cent_value* mute = Cent_value_get_str(value, "mute");
    if (!mute) {
        Zinc_error_list_set(&suite->errors, &value_n->loc, "expected mute");
        return;
    }

    if (mute->type != Cent_value_type_boolean) {
        Zinc_error_list_set(&suite->errors, &value_n->loc, "expected mute to be boolean");
        return;
    }

    suite->mute = mute->data.boolean;
}

void Art_test_header(Art_data* data, Art_suite* suite, Lava_dom* header)
{
    Zinc_string_slice title = Zinc_string_get_slice(&header->data.LAVA_DOM_HEADER.title);
    title = Zinc_trim(title);
    Zinc_string_slice title_ref = Zinc_string_slice_from_str("Test");
    if (!Zinc_string_slice_compare(&title, &title_ref)) {
        Zinc_error_list_set(&suite->errors, &header->loc, "expected test header");
        return;
    }

    if (header->data.LAVA_DOM_HEADER.level != 2) {
        Zinc_error_list_set(&suite->errors, &header->loc, "expected level 2 test header");
        return;
    }

    Art_test* test = NULL;
    size_t index = suite->tests.count;
    Zinc_vector_expand(&suite->tests, 1);
    suite->tests.count++;
    test = (Art_test*)ZINC_VECTOR_PTR(&suite->tests, index);
    Art_test_init(test);

    for (size_t i = 0; i < header->data.LAVA_DOM_HEADER.items.count; i++) {
        Lava_dom* item = (Lava_dom*)ZINC_VECTOR_PTR(&header->data.LAVA_DOM_HEADER.items, i);
        if (item->kind == LAVA_DOM_TEXT) {
            if (test->description.size > 0) {
                Zinc_string_add_char(&test->description, '\n');
            }
            Zinc_string_add_string(&test->description, &item->data.LAVA_DOM_TEXT);
        } else if (item->kind == LAVA_DOM_BACKQUOTE) {
            if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "akela")) {
                test->source_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
            } else if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "llvm")) {
                test->llvm_bounds = item->data.LAVA_DOM_BACKQUOTE.bounds;
                Zinc_string_add_string(&test->llvm, &item->data.LAVA_DOM_BACKQUOTE.text);
            } else if (Zinc_string_compare_str(&item->data.LAVA_DOM_BACKQUOTE.format, "cent")) {
                FILE* fp = fopen(Zinc_string_c_str(&suite->path), "r");
                if (!fp) {
                    Zinc_error_list_set(
                        &suite->errors,
                        &item->loc,
                        "failed to open file: %bf",
                        &suite->path);
                }
                Cent_comp_table* ct = NULL;
                Zinc_string name;
                Zinc_string_init(&name);
                Cent_comp_table_create_fp(&ct, &data->dir_path, &name, fp);
                Zinc_string_destroy(&name);
                Cent_comp_unit_set_bounds(ct->primary, &item->data.LAVA_DOM_BACKQUOTE.bounds);
                Cent_comp_unit_parse(ct->primary);
                Cent_comp_unit_build(ct->primary);
                if (ct->primary->pr.errors->head) {
                    Zinc_error* e = ct->primary->pr.errors->head;
                    while (e) {
                        Zinc_error_list_set(&suite->errors, &e->loc, Zinc_string_c_str(&e->message));
                        e = e->next;
                    }
                } else {
                    if (!ct->primary->value) {
                        Zinc_error_list_set(&suite->errors, &item->loc, "expected value");
                    } else {
                        Art_test_meta(data, suite, test, ct->primary->value);
                    }
                }
                Cent_comp_table_destroy(ct);
            } else {
                Zinc_error_list_set(
                    &suite->errors,
                    &item->loc,
                    "unexpected format: %bf",
                    &item->data.LAVA_DOM_BACKQUOTE.format);
            }
        }
    }
}

void Art_test_meta(Art_data* data, Art_suite* suite, Art_test* test, Cent_value* value)
{
    Cent_ast* n = value->n;
    if (value->type != Cent_value_type_dag) {
        Zinc_error_list_set(&suite->errors, &n->loc, "expected DAG");
        test->has_error = true;
        return;
    }

    if (!Zinc_string_compare_str(&value->name, "Test")) {
        Zinc_error_list_set(&suite->errors, &n->loc, "expected Test");
        test->has_error = true;
        return;
    }

    Cent_value* solo = Cent_value_get_str(value, "solo");
    if (!solo) {
        Zinc_error_list_set(&suite->errors, &n->loc, "expected solo");
        test->has_error = true;
    } else {
        if (solo->type != Cent_value_type_boolean) {
            Zinc_error_list_set(&suite->errors, &n->loc, "expected solo to be boolean");
            test->has_error = true;
        } else {
            test->solo = solo->data.boolean;
        }
    }

    Cent_value* mute = Cent_value_get_str(value, "mute");
    if (!mute) {
        Zinc_error_list_set(&suite->errors, &n->loc, "expected mute");
        test->has_error = true;
    } else {
        if (mute->type != Cent_value_type_boolean) {
            Zinc_error_list_set(&suite->errors, &n->loc, "expected mute to be boolean");
            test->has_error = true;
        } else {
            test->mute = mute->data.boolean;
        }
    }

    Cent_value* snapshot = Cent_value_get_str(value, "snapshot");
    if (!snapshot) {
        Zinc_error_list_set(&suite->errors, &n->loc, "expected snapshot");
        test->has_error = true;
    } else {
        if (snapshot->type != Cent_value_type_boolean) {
            Zinc_error_list_set(&suite->errors, &n->loc, "expected snapshot to be boolean");
            test->has_error = true;
        } else {
            test->snapshot = snapshot->data.boolean;
        }
    }

    if (!test->has_error) {
        test->value = Cent_value_clone(value->data.dag.tail);
    }
}