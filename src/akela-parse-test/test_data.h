#ifndef APT_TEST_DATA_H
#define APT_TEST_DATA_H

#include "zinc/spec_error.h"

typedef struct Apt_test_data {
    Zinc_spec_error_list list;
    Zinc_string name1;
    Zinc_string name2;
} Apt_test_data;

void Apt_test_data_init(Apt_test_data* data);
void Apt_test_data_create(Apt_test_data** data);
void Apt_test_data_destroy(Apt_test_data* data);

#endif