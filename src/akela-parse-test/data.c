#include "data.h"
#include "zinc/memory.h"

void Apt_test_init(Apt_test *test)
{
    test->test = NULL;
    test->snapshot = false;
    test->has_error = false;
    Zinc_string_init(&test->source_path);
    Zinc_string_init(&test->source_name);
    Zinc_input_bounds_init(&test->source_bounds);
    Zinc_string_init(&test->ast_path);
    Zinc_input_bounds_init(&test->ast_bounds);
    Zinc_string_init(&test->description);
    Zinc_error_list_init(&test->expected_errors);
    test->next = NULL;
    test->prev = NULL;
}

void Apt_test_create(Apt_test** test)
{
    Zinc_malloc_safe((void**)test, sizeof(Apt_test));
    Apt_test_init(*test);
}

void Apt_test_destroy(Apt_test *test)
{
    Zinc_string_destroy(&test->source_path);
    Zinc_string_destroy(&test->source_name);
    Zinc_string_destroy(&test->ast_path);
    Zinc_string_destroy(&test->description);
    Zinc_error_list_destroy(&test->expected_errors);
}

void Apt_test_list_init(Apt_test_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Apt_test_list_create(Apt_test_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Apt_test_list));
    Apt_test_list_init(*list);
}

void Apt_test_list_add(Apt_test_list* list, Apt_test* tc)
{
    if (list->head && list->tail) {
        list->tail->next = tc;
        tc->prev = list->tail;
        list->tail = tc;
    } else {
        list->head = tc;
        list->tail = tc;
    }
}

void Apt_test_list_destroy(Apt_test_list* list)
{
    Apt_test* tc = list->head;
    while (tc) {
        Apt_test* temp = tc;
        tc = tc->next;
        Apt_test_destroy(temp);
        free(temp);
    }
}

void Apt_suite_init(Apt_suite *suite)
{
    Zinc_string_init(&suite->path);
    Zinc_string_init(&suite->name);
    Zinc_string_init(&suite->description);
    Zinc_string_init(&suite->text);
    Apt_test_list_init(&suite->list);
    suite->test = NULL;
    Zinc_string_init(&suite->name);
    suite->next = NULL;
    suite->prev = NULL;
}

void Apt_suite_create(Apt_suite** ts)
{
    Zinc_malloc_safe((void**)ts, sizeof(Apt_suite));
    Apt_suite_init(*ts);
}

void Apt_suite_destroy(Apt_suite* ts)
{
    Zinc_string_destroy(&ts->path);
    Zinc_string_destroy(&ts->name);
    Zinc_string_destroy(&ts->description);
    Zinc_string_destroy(&ts->text);
    Apt_test_list_destroy(&ts->list);
}

void Apt_suite_list_init(Apt_suite_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Apt_suite_list_create(Apt_suite_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Apt_suite_list));
    Apt_suite_list_init(*list);
}

void Apt_suite_list_add(Apt_suite_list* list, Apt_suite* ts)
{
    if (list->head && list->tail) {
        list->tail->next = ts;
        ts->prev = list->tail;
        list->tail = ts;
    } else {
        list->head = ts;
        list->tail = ts;
    }
}

void Apt_suite_list_destroy(Apt_suite_list* list)
{
    Apt_suite* ts = list->head;
    while (ts) {
        Apt_suite* temp = ts;
        ts = ts->next;
        Apt_suite_destroy(temp);
        free(temp);
    }
}

void Apt_data_init(Apt_data* data)
{
    Zinc_string_init(&data->dir_path);
    Apt_suite_list_init(&data->suites);
    Zinc_error_list_init(&data->errors);
    Zinc_spec_error_list_init(&data->spec_errors);
    data->test = NULL;
}

void Apt_data_create(Apt_data** data)
{
    Zinc_malloc_safe((void**)data, sizeof(Apt_data));
    Apt_data_init(*data);
}

void Apt_data_destroy(Apt_data* data)
{
    Zinc_string_destroy(&data->dir_path);
    Apt_suite_list_destroy(&data->suites);
    Zinc_error_list_destroy(&data->errors);
    Zinc_spec_error_list_destroy(&data->spec_errors);
    Zinc_test_destroy(data->test);
}