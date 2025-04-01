#include <stdio.h>
#include "data.h"
#include "parse.h"

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

    Apt_data data;
    Apt_data_init(&data);
    Zinc_string_add_str(&data.dir_path, path);

    Apt_parse_files(&data);
    Zinc_error_list_print(&data.errors);

    Apt_data_destroy(&data);

    return 0;
}