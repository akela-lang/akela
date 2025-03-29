#include "data.h"
#include "zinc/memory.h"

void Apt_test_case_init(Apt_test_case *tc)
{
    tc->solo = false;
    tc->mute = false;
    Zinc_string_init(&tc->text);
    Zinc_string_init(&tc->source);
    Zinc_string_init(&tc->expected);
    Zinc_string_init(&tc->name);
    Zinc_error_list_init(&tc->expected_errors);
    tc->text_ct = NULL;
    tc->expected_ct = NULL;
    tc->next = NULL;
    tc->prev = NULL;
}

void Apt_test_case_create(Apt_test_case** tc)
{
    Zinc_malloc_safe((void**)tc, sizeof(Apt_test_case));
    Apt_test_case_init(*tc);
}

void Apt_test_case_destroy(Apt_test_case *tc)
{
    Zinc_string_destroy(&tc->text);
    Zinc_string_destroy(&tc->source);
    Zinc_string_destroy(&tc->expected);
    Zinc_string_destroy(&tc->name);
    Zinc_error_list_destroy(&tc->expected_errors);
    Cent_comp_table_destroy(tc->text_ct);
    Cent_comp_table_destroy(tc->expected_ct);
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

void Apt_test_list_add(Apt_test_list* list, Apt_test_case* tc)
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
    Apt_test_case* tc = list->head;
    while (tc) {
        Apt_test_case* temp = tc;
        tc = tc->next;
        Apt_test_case_destroy(temp);
        free(temp);
    }
}

void Apt_test_suite_init(Apt_test_suite *ts)
{
    Zinc_string_init(&ts->path);
    Zinc_string_init(&ts->name);
    Zinc_string_init(&ts->description);
    Zinc_string_init(&ts->text);
    Apt_test_list_init(&ts->list);
    ts->solo = false;
    ts->mute = false;
    Zinc_string_init(&ts->name);
    ts->next = NULL;
    ts->prev = NULL;
}

void Apt_test_suite_create(Apt_test_suite** ts)
{
    Zinc_malloc_safe((void**)ts, sizeof(Apt_test_suite));
    Apt_test_suite_init(*ts);
}

void Apt_test_suite_destroy(Apt_test_suite* ts)
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

void Apt_suite_list_add(Apt_suite_list* list, Apt_test_suite* ts)
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
    Apt_test_suite* ts = list->head;
    while (ts) {
        Apt_test_suite* temp = ts;
        ts = ts->next;
        Apt_test_suite_destroy(temp);
        free(temp);
    }
}

void Apt_data_init(Apt_data* data)
{
    Zinc_string_init(&data->dir_path);
    data->test_suite_sep = Cob_compile_str("^======\n?$");
    data->test_case_sep = Cob_compile_str("^######\n?$");
    data->section_sep = Cob_compile_str("^###\n?$");
    Apt_suite_list_init(&data->suites);
    data->fp = NULL;
}

void Apt_data_create(Apt_data** data)
{
    Zinc_malloc_safe((void**)data, sizeof(Apt_data));
    Apt_data_init(*data);
}

void Apt_data_destroy(Apt_data* data)
{
    Zinc_string_destroy(&data->dir_path);
    Cob_re_destroy(&data->test_suite_sep);
    Cob_re_destroy(&data->test_case_sep);
    Cob_re_destroy(&data->section_sep);
    Apt_suite_list_destroy(&data->suites);
}