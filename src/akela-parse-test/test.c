#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "zinc/fs.h"
#include "zinc/test.h"
#include "zinc/expect.h"
#include "data.h"
#include "parse.h"
#include "compare_tree.h"
#include "centipede/comp_table.h"

#define NAME "akela-parse-test"

void Apt_dir_validate(Zinc_test* test, char* path);
void Apt_destroy(Zinc_test* top_test);

void Apt(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, NAME);
        test->mute = false;
        test->solo = false;

        char* exe_str = NULL;
        Zinc_get_exe_path(&exe_str);

        Zinc_string exe;
        Zinc_string_init(&exe);
        Zinc_string_add_str(&exe, exe_str);
        free(exe_str);

        Zinc_string dir;
        Zinc_string_init(&dir);
        Zinc_string filename;
        Zinc_string_init(&filename);
        Zinc_split_path(&exe, &dir, &filename);

        Zinc_string_destroy(&exe);
        Zinc_string_destroy(&filename);

        Zinc_string name;
        Zinc_string_init(&name);
        Zinc_string_add_str(&name, "test-config.cent");

        Zinc_string config_path;
        Zinc_string_init(&config_path);
        Zinc_string_add_string(&config_path, &dir);
        Zinc_path_append(&config_path, &name);

        Zinc_result r = Zinc_is_reg_file(&config_path);
        Zinc_expect_ok(test, r, "is reg file");
        if (r == Zinc_result_error) {
            Zinc_string_destroy(&dir);
            Zinc_string_destroy(&name);
            Zinc_string_destroy(&config_path);
            return;
        }

        FILE* fp = fopen(Zinc_string_c_str(&config_path), "r");
        Zinc_expect_ptr(test, fp, "fopen");
        if (!fp) {
            fprintf(stderr, "could not open file");
            return;
        }

        Cent_comp_table* ct = NULL;
        Cent_comp_table_create_fp(&ct, &dir, &name, fp);
        Cent_comp_unit_parse(ct->primary);
        Cent_comp_unit_build(ct->primary);
        Cent_value* value = ct->primary->value;
        if (value->type != Cent_value_type_dag) {
            fprintf(stderr, "expected DAG");
            return;
        }

        if (!Zinc_string_compare_str(&value->name, "Test")) {
            fprintf(stderr, "\texpected Test\n");
            return;
        }

        Cent_value* dir_value = Cent_value_get_str(value, "akela_parse_test");
        if (!dir_value) {
            fprintf(stderr, "\texpected akela_parse_test\n");
            return;
        }

        if (dir_value->type != Cent_value_type_string) {
            fprintf(stderr, "\texpected string\n");
            return;
        }

        Zinc_string test_cases_path;
        Zinc_string_init(&test_cases_path);
        Zinc_string_add_string(&test_cases_path, &dir_value->data.string);

        Zinc_string_destroy(&dir);
        Zinc_string_destroy(&name);
        Zinc_string_destroy(&config_path);
        Cent_comp_table_destroy(ct);
        free(ct);

        Apt_dir_validate(test, Zinc_string_c_str(&test_cases_path));

        Apt_top_data* top_data = NULL;
        Apt_top_data_create(&top_data);
        test->data = top_data;

        Zinc_string_add_string(&top_data->dir_path, &test_cases_path);

        Apt_parse_files(test, &test_cases_path);

        if (top_data->errors.head) {
            Zinc_error_list_print(&top_data->errors);
        }

        Zinc_string_destroy(&test_cases_path);

    } else {
        Zinc_test_perform(test);
        Apt_destroy(test);
    }
}

void Apt_dir_validate(Zinc_test* test, char* path)
{
    test->check_count++;
    if (!Zinc_file_exists(path)) {
        fprintf(stderr, "\t%s:%s", strerror(errno), path);

        Zinc_string cwd;
        Zinc_string_init(&cwd);
        Zinc_get_cwd(&cwd);
        test->check_failed++;
        test->pass = false;
        fprintf(stderr, "\tcurrent working directory: %s\n", Zinc_string_c_str(&cwd));
        Zinc_string_destroy(&cwd);
        return;
    }
    test->check_passed++;

    bool is_dir;
    Zinc_result r = Zinc_is_directory(path, &is_dir);
    test->check_count++;
    if (r == Zinc_result_error) {
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
        fprintf(stderr, "\t%s\n", Zinc_error_message);
        return;
    }
    test->check_passed++;

    test->check_count++;
    if (!is_dir) {
        test->check_failed++;
        test->pass = false;
        Zinc_test_print_unseen(test);
        fprintf(stderr, "%s is not a directory", path);
        return;
    }
    test->check_passed++;
}

void Apt_destroy(Zinc_test* top_test)
{
    Apt_top_data* top_data = top_test->data;
    Apt_top_data_destroy(top_data);
    free(top_data);

    Zinc_test* suite_test = top_test->head;
    while (suite_test) {
        Apt_suite_data* suite_data = suite_test->data;
        Apt_suite_data_destroy(suite_data);
        free(suite_data);

        Zinc_test* case_test = suite_test->head;
        while (case_test) {
            Apt_case_data* case_data = case_test->data;
            Apt_case_data_destroy(case_data);
            free(case_data);
            case_test = case_test->next;
        }

        suite_test = suite_test->next;
    }
}