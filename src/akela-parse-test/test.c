#include <stdio.h>
#include "data.h"
#include "parse.h"
#include "compare.h"
#include "zinc/fs.h"

#define NAME "akela-parse-test"

bool Apt_validate(int argc, char* argv[], Apt_top_data* top_data);

int main(int argc, char **argv)
{
    Apt_top_data top_data;
    Apt_top_data_init(&top_data);

    if (!Apt_validate(argc, argv, &top_data)) {
        return 1;
    }

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

    return 0;
}

bool Apt_validate(int argc, char* argv[], Apt_top_data* top_data)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [DIR]\n", NAME);
        return 1;
    }

    char* path = argv[1];
    Zinc_string_add_str(&top_data->dir_path, path);

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
