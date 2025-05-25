#include "data.h"
#include "zinc/memory.h"

void Apt_case_data_init(Apt_case_data *case_data)
{
    case_data->snapshot = false;
    case_data->has_error = false;
    Zinc_string_init(&case_data->source_name);
    Zinc_input_bounds_init(&case_data->source_bounds);
    Zinc_input_bounds_init(&case_data->ast_bounds);
    Zinc_string_init(&case_data->description);
    Zinc_error_list_init(&case_data->errors);
    Zinc_spec_error_list_init(&case_data->spec_errors);
    case_data->next = NULL;
    case_data->prev = NULL;
}

void Apt_case_data_create(Apt_case_data** case_data)
{
    Zinc_malloc_safe((void**)case_data, sizeof(Apt_case_data));
    Apt_case_data_init(*case_data);
}

void Apt_case_data_destroy(Apt_case_data *case_data)
{
    Zinc_string_destroy(&case_data->source_name);
    Zinc_string_destroy(&case_data->description);
    Zinc_error_list_destroy(&case_data->errors);
    Zinc_spec_error_list_destroy(&case_data->spec_errors);
}

void Apt_case_data_list_init(Apt_case_data_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Apt_case_data_list_create(Apt_case_data_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Apt_case_data_list));
    Apt_case_data_list_init(*list);
}

void Apt_case_data_list_add(Apt_case_data_list* list, Apt_case_data* case_data)
{
    if (list->head && list->tail) {
        list->tail->next = case_data;
        case_data->prev = list->tail;
        list->tail = case_data;
    } else {
        list->head = case_data;
        list->tail = case_data;
    }
}

void Apt_case_data_list_destroy(Apt_case_data_list* list)
{
    Apt_case_data* tc = list->head;
    while (tc) {
        Apt_case_data* temp = tc;
        tc = tc->next;
        Apt_case_data_destroy(temp);
        free(temp);
    }
}

void Apt_suite_data_init(Apt_suite_data *suite_data)
{
    Zinc_string_init(&suite_data->path);
    Zinc_string_init(&suite_data->name);
    Zinc_string_init(&suite_data->description);
    Zinc_string_init(&suite_data->text);
    Zinc_error_list_init(&suite_data->errors);
    Apt_case_data_list_init(&suite_data->list);
    Zinc_string_init(&suite_data->name);
    suite_data->next = NULL;
    suite_data->prev = NULL;
}

void Apt_suite_data_create(Apt_suite_data** suite_data)
{
    Zinc_malloc_safe((void**)suite_data, sizeof(Apt_suite_data));
    Apt_suite_data_init(*suite_data);
}

void Apt_suite_data_destroy(Apt_suite_data* suite_data)
{
    Zinc_string_destroy(&suite_data->path);
    Zinc_string_destroy(&suite_data->name);
    Zinc_string_destroy(&suite_data->description);
    Zinc_string_destroy(&suite_data->text);
    Zinc_error_list_destroy(&suite_data->errors);
    Apt_case_data_list_destroy(&suite_data->list);
}

void Apt_suite_data_list_init(Apt_suite_data_list* list)
{
    list->head = NULL;
    list->tail = NULL;
}

void Apt_suite_data_list_create(Apt_suite_data_list** list)
{
    Zinc_malloc_safe((void**)list, sizeof(Apt_suite_data_list));
    Apt_suite_data_list_init(*list);
}

void Apt_suite_data_list_add(Apt_suite_data_list* list, Apt_suite_data* suite_data)
{
    if (list->head && list->tail) {
        list->tail->next = suite_data;
        suite_data->prev = list->tail;
        list->tail = suite_data;
    } else {
        list->head = suite_data;
        list->tail = suite_data;
    }
}

void Apt_suite_data_list_destroy(Apt_suite_data_list* list)
{
    Apt_suite_data* ts = list->head;
    while (ts) {
        Apt_suite_data* temp = ts;
        ts = ts->next;
        Apt_suite_data_destroy(temp);
        free(temp);
    }
}

void Apt_top_data_init(Apt_top_data* top_data)
{
    Zinc_string_init(&top_data->dir_path);
    Zinc_error_list_init(&top_data->errors);
}

void Apt_top_data_create(Apt_top_data** top_data)
{
    Zinc_malloc_safe((void**)top_data, sizeof(Apt_top_data));
    Apt_top_data_init(*top_data);
}

void Apt_top_data_destroy(Apt_top_data* top_data)
{
    Zinc_string_destroy(&top_data->dir_path);
    Zinc_error_list_destroy(&top_data->errors);
}