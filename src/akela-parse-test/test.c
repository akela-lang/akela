#include <stdio.h>
#include "data.h"
#include "parse.h"
#include "compare.h"

#define NAME "akela-parse-test"

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [DIR]\n", NAME);
        return 1;
    }

    char* path = argv[1];
    if (!Apt_validate_directory(path)) {
        return 1;
    }

    Apt_top_data top_data;
    Apt_top_data_init(&top_data);
    Zinc_string_add_str(&top_data.dir_path, path);

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