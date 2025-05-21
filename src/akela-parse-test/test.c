#include <stdio.h>
#include "data.h"
#include "parse.h"
#include "compare.h"
#include "zinc/fs.h"
#include "zinc/test.h"
#include "errno.h"
#include <string.h>
#include "zinc/expect.h"

#define NAME "akela-parse-test"

void Apt_dir_validate(Zinc_test* test, char* path);

void Apt(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, NAME);
        test->mute = false;
        test->solo = false;
        return;
    }

    char* path_str = NULL;
    Zinc_get_exe_path(&path_str);
    Zinc_string path;
    Zinc_string_init(&path);
    Zinc_string_add_str(&path, path_str);
    Zinc_string dir;
    Zinc_string_init(&dir);
    Zinc_string filename;
    Zinc_string_init(&filename);
    Zinc_split_path(&path, &dir, &filename);
    Zinc_string_destroy(&path);
    Zinc_string_destroy(&filename);
    Zinc_path_append_str(&dir, "akela-parse-config.cent");
    printf("config: %s\n", Zinc_string_c_str(&dir));
    Zinc_result r = Zinc_is_reg_file(&dir);
    Zinc_test_expect_ok(test, r, "is reg file");
    if (r == Zinc_result_error) {
        Zinc_string_destroy(&dir);
        return;
    }

    // Apt_dir_validate(test, Zinc_string_c_str(&dir));

    Zinc_string_destroy(&dir);

    return;

    Apt_top_data top_data;
    Apt_top_data_init(&top_data);

    Apt_parse_files(&top_data);

    if (top_data.errors.head) {
        Zinc_error_list_print(&top_data.errors);
    } else {
        Apt_run(&top_data);

        if (top_data.errors.head) {
            Zinc_error_list_print(&top_data.errors);
        }
    }

    if (top_data.spec_errors.head) {
        Zinc_spec_error_list_print(&top_data.spec_errors);
    }

    Zinc_test_stat stat;
    Zinc_test_stat_init(&stat);
    Zinc_test_count(top_data.test, &stat);
    Zinc_test_print(&stat);

    Apt_top_data_destroy(&top_data);
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
