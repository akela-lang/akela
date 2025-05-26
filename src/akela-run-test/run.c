#include "zinc/os_unix.h"
#include "zinc/zstring.h"
#include "parse.h"
#include "test_case.h"
#include "data.h"
#include "type_info.h"
#include "zinc/fs.h"

#define NAME "akela-run-test"

bool Art_validate_directory(const char* path);
bool Art_print_errors(Art_data* data);

void Art(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
    } else {
        Zinc_test_perform(test);
    }

    return;
    // if (!Art_validate_directory(dir_path)) {
    //     return 1;
    // }
    //
    // Art_data data;
    // Art_data_init(&data);
    //
    // Zinc_string_add_str(&data.dir_path, dir_path);
    // Art_parse_files(&data, dir_path);
    //
    // if (Art_print_errors(&data)) {
    //     Art_data_destroy(&data);
    //     return 1;
    // }
    //
    // Run_get_type_info(&data);
    //
    // if (Art_print_errors(&data)) {
    //     Art_data_destroy(&data);
    //     return 1;
    // }
    //
    // Art_run(&data);
    //
    // Zinc_test_stat stat;
    // Zinc_test_stat_init(&stat);
    // Zinc_test_count(data.test, &stat);
    // Zinc_test_print(&stat);
    //
    // Art_data_destroy(&data);
    //
    // return 0;
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

bool Art_print_errors(Art_data* data)
{
    bool has_errors = false;
    Art_suite* suite = data->head;
    while (suite) {
        if (suite->errors.head) {
            has_errors = true;
            Zinc_error_list_print(&suite->errors);
        }

        Art_test* test = suite->head;
        while (test) {
            if (test->spec_errors.head) {
                has_errors = true;
                Zinc_spec_error_list_print(&test->spec_errors);
            }
            test = test->next;
        }

        suite = suite->next;
    }
    return has_errors;
}