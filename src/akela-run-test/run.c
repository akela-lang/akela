#include "zinc/os_unix.h"
#include "zinc/zstring.h"
#include "parse.h"
#include "test_case.h"
#include "data.h"
#include "type_info.h"
#include "zinc/fs.h"
#include "centipede/comp_table.h"

#define NAME "akela-run-test"

bool Art_validate_directory(const char* path);
bool Art_print_errors(Zinc_test* top_test);
void Art_destroy(Zinc_test* top_test);

void Art(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        char* exe;
        Zinc_get_exe_path(&exe);
        Zinc_string exe_path;
        Zinc_string_init(&exe_path);
        Zinc_string_add_str(&exe_path, exe);

        Zinc_string dir_path;
        Zinc_string_init(&dir_path);

        Zinc_string filename;
        Zinc_string_init(&filename);

        Zinc_split_path(&exe_path, &dir_path, &filename);

        Zinc_string config_name;
        Zinc_string_init(&config_name);

        char* config_name_str = "test-config.cent";
        Zinc_string_add_str(&config_name, config_name_str);

        Zinc_string config_path;
        Zinc_string_init(&config_path);

        Zinc_string_add_string(&config_path, &dir_path);
        Zinc_path_append_str(&config_path, config_name_str);

        FILE* fp = fopen(Zinc_string_c_str(&config_path), "r");
        if (!fp) {
            fprintf(stderr, "could not open: %s\n", Zinc_string_c_str(&config_path));
            return;
        }

        Cent_comp_table* ct = NULL;
        Cent_comp_table_create_fp(&ct, &dir_path, &config_name, fp);
        Cent_comp_unit_parse(ct->primary);
        Cent_comp_unit_build(ct->primary);

        if (ct->primary->errors.head) {
            fprintf(stderr, "Errors:\n");
            Zinc_error_list_print(&ct->primary->errors);
            return;
        }

        Cent_value* value = ct->primary->value;
        if (!value) {
            fprintf(stderr, "expected value\n");
            return;
        }

        if (value->type != Cent_value_type_dag) {
            fprintf(stderr, "expected DAG\n");
            return;
        }

        if (!Zinc_string_compare_str(&value->name, "Test")) {
            fprintf(stderr, "expected Test\n");
            return;
        }

        Cent_value* akela_run_test_value = Cent_value_get_str(value, "akela_run_test");
        if (!akela_run_test_value) {
            fprintf(stderr, "expected akela_run_test");
            return;
        }

        if (akela_run_test_value->type != Cent_value_type_string) {
            fprintf(stderr, "expected type string for akela_run_test\n");
            return;
        }

        Zinc_string akela_run_test_dir;
        Zinc_string_init(&akela_run_test_dir);
        Zinc_string_add_string(&akela_run_test_dir, &akela_run_test_value->data.string);

        if (!Art_validate_directory(Zinc_string_c_str(&akela_run_test_dir))) {
            return;
        }

        Zinc_string_destroy(&exe_path);
        Zinc_string_destroy(&dir_path);
        Zinc_string_destroy(&filename);
        Zinc_string_destroy(&config_path);
        Zinc_string_destroy(&config_name);
        Cent_comp_table_destroy(ct);

        Art_top_data* top_data = NULL;
        Art_top_data_create(&top_data);
        test->data = top_data;

        Zinc_string_add_string(&top_data->dir_path, &akela_run_test_dir);
        Art_parse_files(test);

        Zinc_test* top_test = test;

        Run_get_type_info(top_data);

        if (Art_print_errors(top_test)) {
            return;
        }

    } else {
        Zinc_test_perform(test);
        Art_destroy(test);
    }


}

bool Art_validate_directory(const char* path)
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
        fprintf(stderr, "%s\n", Zinc_error_message);
        return false;
    }

    if (!is_dir) {
        fprintf(stderr, "%s is not a directory", path);
        return false;
    }

    return true;
}

bool Art_print_errors(Zinc_test* top_test)
{
    bool has_errors = false;
    Zinc_test* suite_test = top_test->head;
    while (suite_test) {
        Art_suite_data* suite_data = suite_test->data;
        if (suite_data->errors.head) {
            has_errors = true;
            Zinc_error_list_print(&suite_data->errors);
        }

        Zinc_test* case_test = suite_test->head;
        while (case_test) {
            Art_case_data* case_data = case_test->data;
            if (case_data->spec_errors.head) {
                has_errors = true;
                Zinc_spec_error_list_print(&case_data->spec_errors);
            }
            case_test = case_test->next;
        }

        suite_test = suite_test->next;
    }
    return has_errors;
}

void Art_destroy(Zinc_test* top_test)
{
    Art_top_data* top_data = top_test->data;
    Art_top_data_destroy(top_data);
    free(top_data);

    Zinc_test* suite_test = top_test->head;
    while (suite_test) {
        Art_suite_data* suite_data = suite_test->data;
        Art_suite_data_destroy(suite_data);
        free(suite_data);

        Zinc_test* case_test = suite_test->head;
        while (case_test) {
            Art_case_data* case_data = case_test->data;
            Art_case_data_destroy(case_data);
            free(case_data);

            case_test = case_test->next;
        }

        suite_test = suite_test->next;
    }
}