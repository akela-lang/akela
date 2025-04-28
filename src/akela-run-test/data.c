#include "data.h"
#include "zinc/memory.h"
#include "cobble/compile_data.h"
#include "centipede/module_finder_file.h"

void Art_pair_init(Art_pair* pair)
{
    pair->matched = true;
    pair->actual = NULL;
    pair->expected = NULL;
}

void Art_pair_create(Art_pair** pair)
{
    Zinc_malloc_safe((void**)pair, sizeof(Art_pair));
    Art_pair_init(*pair);
}

void Art_pair_destroy(Art_pair* pair)
{
    Zinc_string_destroy(pair->actual);
    free(pair->actual);
    Zinc_string_destroy(pair->expected);
    free(pair->expected);
}

void Art_test_init(Art_test* test)
{
    Zinc_string_init(&test->description);
    test->snapshot = false;
    test->has_error = false;
    test->ct = NULL;
    test->value = NULL;
    test->return_address = NULL;
    Zinc_input_bounds_init(&test->source_bounds);
    Zinc_input_bounds_init(&test->llvm_bounds);
    Zinc_string_init(&test->llvm);
    Zinc_spec_error_list_init(&test->spec_errors);
    test->test = NULL;
    test->next = NULL;
    test->prev = NULL;
}

void Art_test_create(Art_test** test)
{
    Zinc_malloc_safe((void**)test, sizeof(Art_test));
    Art_test_init(*test);
}

void Art_test_destroy(Art_test* test)
{
    Zinc_string_destroy(&test->description);
    Cent_comp_table_destroy(test->ct);
    free(test->ct);
    Zinc_string_destroy(&test->llvm);
    Zinc_spec_error_list_destroy(&test->spec_errors);
    free(test->return_address);
}

void Art_suite_init(Art_suite* suite)
{
    Zinc_string_init(&suite->path);
    Zinc_string_init(&suite->description);
    Zinc_string_init(&suite->name);
    Zinc_error_list_init(&suite->errors);
    suite->test = NULL;
    suite->next = NULL;
    suite->prev = NULL;
    suite->head = NULL;
    suite->tail = NULL;
}

void Art_suite_create(Art_suite** suite)
{
    Zinc_malloc_safe((void**)suite, sizeof(Art_suite));
    Art_suite_init(*suite);
}

void Art_suite_destroy(Art_suite* suite)
{
    Zinc_string_destroy(&suite->path);
    Zinc_string_destroy(&suite->description);
    Zinc_string_destroy(&suite->name);
    Zinc_error_list_destroy(&suite->errors);
    Art_test* test = suite->head;
    while (test) {
        Art_test* temp = test;
        test = test->next;
        Art_test_destroy(temp);
        free(temp);
    }
}

void Art_suite_add(Art_suite* suite, Art_test* test)
{
    if (suite->head && suite->tail) {
        suite->tail->next = test;
        test->prev = suite->tail;
        suite->tail = test;
    } else {
        suite->head = test;
        suite->tail = test;
    }
}

void Art_data_init(Art_data* data)
{
    Zinc_string_init(&data->dir_path);
    data->has_solo = false;
    data->type_info = NULL;
    data->regex_re = Cob_compile_str("^/(.*)/\n?$");
    data->test = NULL;
    data->head = NULL;
    data->tail = NULL;
}

void Art_data_destroy(Art_data* data)
{
    Zinc_string_destroy(&data->dir_path);
    Cent_comp_table_destroy(data->type_info);
    free(data->type_info);
    Cob_re_destroy(&data->regex_re);

    Art_suite* suite = data->head;
    while (suite) {
        Art_suite* temp = suite;
        suite = suite->next;
        Art_suite_destroy(temp);
        free(temp);
    }

    if (data->test) {
        Zinc_test_destroy(data->test);
        free(data->test);
    }
}

void Art_data_add(Art_data* data, Art_suite* suite)
{
    if (data->head && data->tail) {
        data->tail->next = suite;
        suite->prev = data->tail;
        data->tail = suite;
    } else {
        data->head = suite;
        data->tail = suite;
    }
}