#include "data.h"
#include "zinc/memory.h"
#include "cobble/compile_data.h"

void Run_test_init(Run_test *test)
{
    Zinc_string_init(&test->ake);
    Zinc_string_init(&test->llvm);
    Zinc_string_init(&test->config_string);
    test->config = NULL;
    test->next = NULL;
    test->prev = NULL;
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
    Zinc_string_destroy(&test->config_string);
    Cent_value_destroy(test->config);
    free(test->config);
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
    data->ext_re = Cob_compile_str("\\.run$");
    data->separator_re = Cob_compile_str("^###");
    data->regex_re = Cob_compile_str("^/(.*)/\n?$");
    data->test_count = 0;
    data->test_failed_count = 0;
    data->test_passed_count = 0;
    data->has_solo = false;
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