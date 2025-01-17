#include "test_data.h"
#include "zinc/memory.h"

void Apt_test_data_init(Apt_test_data* data)
{
    Zinc_spec_error_list_init(&data->list);
    Zinc_string_init(&data->name1);
    Zinc_string_init(&data->name2);
}

void Apt_test_data_create(Apt_test_data** data)
{
    Zinc_malloc_safe((void**)data, sizeof(Apt_test_data));
    Apt_test_data_init(*data);
}

void Apt_test_data_destroy(Apt_test_data* data)
{
    Zinc_spec_error_list_destroy(&data->list);
    Zinc_string_destroy(&data->name1);
    Zinc_string_destroy(&data->name2);
}