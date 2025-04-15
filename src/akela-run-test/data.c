#include "data.h"
#include "zinc/memory.h"
#include "cobble/compile_data.h"
#include "centipede/module_finder_file.h"

void Run_config_data_init(Run_cent_data* config_data, Zinc_string* file_name)
{
    config_data->mf = NULL;
    config_data->ct = NULL;
    config_data->v = NULL;
    config_data->input = NULL;
    Zinc_string_init(&config_data->file_name);
    Zinc_string_add_string(&config_data->file_name, file_name);
    config_data->valid = false;
}

void Run_cent_data_create(Run_cent_data** config_data, Zinc_string* file_name)
{
    Zinc_malloc_safe((void**)config_data, sizeof(Run_cent_data));
    Run_config_data_init(*config_data, file_name);
}

void Run_cent_data_destroy(Run_cent_data* config_data)
{
    if (config_data) {
        Cent_comp_table_destroy(config_data->ct);
        free(config_data->ct);
        Zinc_string_destroy(&config_data->file_name);
    }
}

void Run_test_init(Run_test *test)
{
    Zinc_string_init(&test->ake);
    Zinc_string_init(&test->llvm);
    Zinc_string_init(&test->config);
    test->next = NULL;
    test->prev = NULL;
    test->return_address = NULL;
    test->return_size = 0;
    Zinc_string_init(&test->name);
    test->solo = false;
    test->mute = false;
}

void Run_test_create(Run_test** test)
{
    Zinc_malloc_safe((void**)test, sizeof(Run_test));
    Run_test_init(*test);
}

void Run_test_destroy(Run_test* test)
{
    Zinc_string_destroy(&test->ake);
    Zinc_string_destroy(&test->llvm);
    Zinc_string_destroy(&test->config);
    Run_cent_data_destroy(test->config_data);
    free(test->config_data);
    free(test->return_address);
    Zinc_string_destroy(&test->name);
}

void Run_test_list_init(Run_test_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Run_test_list_create(Run_test_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Run_test_list));
    Run_test_list_init(*list);
}

void Run_test_list_add(Run_test_list* list, Run_test* test)
{
    if (list->head && list->tail) {
        list->tail->next = test;
        test->prev = list->tail;
        list->tail = test;
    } else {
        list->head = test;
        list->tail = test;
    }
}

void Run_test_list_destroy(Run_test_list* list)
{
    Run_test* p = list->head;
    while (p) {
        Run_test* temp = p;
        p = p->next;
        Run_test_destroy(temp);
        free(temp);
    }
}

void Run_data_init(Run_data* data)
{
    data->ext_re = Cob_compile_str("\\.md$");
    data->separator_re = Cob_compile_str("^###");
    data->regex_re = Cob_compile_str("^/(.*)/\n?$");
    data->test_count = 0;
    data->test_failed_count = 0;
    data->test_passed_count = 0;
    data->has_solo = false;
    Run_test_list_init(&data->tests);
    data->type_info = NULL;
    Zinc_error_list_init(&data->errors);
    Zinc_spec_error_list_init(&data->spec_errors);
}

void Run_data_create(Run_data** data)
{
    Zinc_malloc_safe((void**)data, sizeof(Run_data));
    Run_data_init(*data);
}

void Run_data_destroy(Run_data* data)
{
    Cob_re_destroy(&data->ext_re);
    Cob_re_destroy(&data->separator_re);
    Cob_re_destroy(&data->regex_re);
    Run_test_list_destroy(&data->tests);
    Run_cent_data_destroy(data->type_info);
    free(data->type_info);
    Zinc_error_list_destroy(&data->errors);
    Zinc_spec_error_list_destroy(&data->spec_errors);
}

void Run_pair_init(Run_pair* pair)
{
    pair->matched = true;
    pair->actual = NULL;
    pair->expected = NULL;
}

void Run_pair_create(Run_pair** pair)
{
    Zinc_malloc_safe((void**)pair, sizeof(Run_pair));
    Run_pair_init(*pair);
}

void Run_pair_destroy(Run_pair* pair)
{
    Zinc_string_destroy(pair->actual);
    free(pair->actual);
    Zinc_string_destroy(pair->expected);
    free(pair->expected);
}

void Art_test_init(Art_test* test)
{
    Zinc_string_init(&test->description);
    test->solo = false;
    test->mute = false;
    test->snapshot = false;
    test->has_error = false;
    test->value = NULL;
    Zinc_input_bounds_init(&test->source_bounds);
    Zinc_input_bounds_init(&test->llvm_bounds);
    Zinc_string_init(&test->llvm);
    Zinc_spec_error_list_init(&test->spec_errors);
}

void Art_test_destroy(Art_test* test)
{
    Zinc_string_destroy(&test->description);
    Cent_value_free(test->value);
    Zinc_string_destroy(&test->llvm);
    Zinc_spec_error_list_destroy(&test->spec_errors);
}

void Art_suite_init(Art_suite* suite)
{
    Zinc_string_init(&suite->path);
    Zinc_string_init(&suite->description);
    Zinc_string_init(&suite->name);
    suite->solo = false;
    suite->mute = false;
    suite->has_solo = false;
    Zinc_error_list_init(&suite->errors);
    Zinc_vector_init(&suite->tests, sizeof(Art_test));
}

void Art_suite_destroy(Art_suite* suite)
{
    Zinc_string_destroy(&suite->path);
    Zinc_string_destroy(&suite->description);
    Zinc_string_destroy(&suite->name);
    Zinc_error_list_destroy(&suite->errors);
    Zinc_vector_destroy(&suite->tests);
}

void Art_data_init(Art_data* data)
{
    Zinc_string_init(&data->dir_path);
    data->test_count = 0;
    data->test_passed_count = 0;
    data->test_failed_count = 0;
    data->has_solo = false;
    Zinc_vector_init(&data->suites, sizeof(Art_suite));
    data->type_info = NULL;
    data->regex_re = Cob_compile_str("^/(.*)/\n?$");
}

void Art_data_destroy(Art_data* data)
{
    Zinc_string_destroy(&data->dir_path);
    Zinc_vector_destroy(&data->suites);
    Cent_comp_table_destroy(data->type_info);
    Cob_re_destroy(&data->regex_re);
}