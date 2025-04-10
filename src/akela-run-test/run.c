#include "zinc/os_unix.h"
#include "zinc/zstring.h"
#include "parse.h"
#include "test_case.h"
#include "data.h"
#include "type_info.h"

#define NAME "akela-run-test"

int main(int argc, const char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [DIR]\n", NAME);
        return 1;
    }

    const char* dir_name = argv[1];
    if (!Run_validate_directory(dir_name)) {
        return 1;
    }

    Run_data data;
    Run_data_init(&data);

    Run_parse_files(&data, dir_name);

    if (data.errors.head) {
        Zinc_error_list_print(&data.errors);
        Run_data_destroy(&data);
        return 0;
    }

    Run_get_type_info(&data, dir_name);

    if (data.spec_errors.head) {
        Zinc_error_list_print(&data.errors);
        Run_data_destroy(&data);
        return 0;
    }

    //Run_test_cases(&data);
    //Run_print_results(&data);

    Run_data_destroy(&data);

    return 0;
}