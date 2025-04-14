#include <zinc-unit/test_hash_map_string.h>

#include "zinc/os_unix.h"
#include "zinc/zstring.h"
#include "parse.h"
#include "test_case.h"
#include "data.h"
#include "type_info.h"

#define NAME "akela-run-test"

bool Art_print_errors(Art_data* data);

int main(int argc, const char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [DIR]\n", NAME);
        return 1;
    }

    const char* dir_path = argv[1];
    if (!Run_validate_directory(dir_path)) {
        return 1;
    }

    Art_data data;
    Art_data_init(&data);

    Zinc_string_add_str(&data.dir_path, dir_path);
    Run_parse_files(&data, dir_path);

    if (Art_print_errors(&data)) {
        Art_data_destroy(&data);
        return 1;
    }

    Run_get_type_info(&data);

    if (Art_print_errors(&data)) {
        Art_data_destroy(&data);
        return 1;
    }

    //Run_test_cases(&data);
    //Run_print_results(&data);

    Art_data_destroy(&data);

    return 0;
}

bool Art_print_errors(Art_data* data)
{
    bool has_errors = false;
    for (size_t i = 0; i < data->suites.count; i++) {
        Art_suite* suite = (Art_suite*)ZINC_VECTOR_PTR(&data->suites, i);
        if (suite->errors.head) {
            has_errors = true;
            Zinc_error_list_print(&suite->errors);
        }

        for (size_t j = 0; j < suite->tests.count; j++) {
            Art_test* test = (Art_test*)ZINC_VECTOR_PTR(&suite->tests, j);
            if (test->spec_errors.head) {
                has_errors = true;
                Zinc_spec_error_list_print(&test->spec_errors);
            }
        }
    }
    return has_errors;
}