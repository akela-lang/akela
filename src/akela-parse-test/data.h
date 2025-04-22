#ifndef AKELA_PARSE_TEST_DATA_H
#define AKELA_PARSE_TEST_DATA_H

#include <cobble/compile_data.h>
#include <cobble/compile.h>
#include <zinc/test.h>

#include "zinc/zstring.h"
#include "zinc/spec_error.h"
#include "centipede/value.h"
#include "centipede/comp_table.h"
#include "zinc/error.h"

typedef struct Apt_test Apt_test;
struct Apt_test {
    Zinc_test* test;
    bool snapshot;
    bool has_error;
    Zinc_string source_path;
    Zinc_string source_name;
    Zinc_input_bounds source_bounds;
    Zinc_string ast_path;
    Zinc_input_bounds ast_bounds;
    Zinc_string description;
    Zinc_error_list expected_errors;
    Apt_test* next;
    Apt_test* prev;
};

typedef struct Apt_test_list Apt_test_list;
struct Apt_test_list {
    Apt_test* head;
    Apt_test* tail;
};

typedef struct Apt_suite Apt_suite;
struct Apt_suite {
    Zinc_string path;
    Zinc_string name;
    Zinc_string description;
    Zinc_string text;
    Apt_test_list list;
    Zinc_test* test;
    Apt_suite* next;
    Apt_suite* prev;
};

typedef struct Apt_suite_list Apt_suite_list;
struct Apt_suite_list {
    Apt_suite* head;
    Apt_suite* tail;
};

typedef struct Apt_data Apt_data;
struct Apt_data
{
    Zinc_string dir_path;
    Apt_suite_list suites;
    Zinc_error_list errors;
    Zinc_spec_error_list spec_errors;
    Zinc_test* test;
};

void Apt_test_init(Apt_test *test);
void Apt_test_create(Apt_test** test);
void Apt_test_destroy(Apt_test *test);

void Apt_test_list_init(Apt_test_list* list);
void Apt_test_list_create(Apt_test_list** list);
void Apt_test_list_add(Apt_test_list* list, Apt_test* tc);
void Apt_test_list_destroy(Apt_test_list* list);

void Apt_suite_init(Apt_suite *suite);
void Apt_suite_create(Apt_suite** ts);
void Apt_suite_destroy(Apt_suite* ts);

void Apt_suite_list_init(Apt_suite_list* list);
void Apt_suite_list_create(Apt_suite_list** list);
void Apt_suite_list_add(Apt_suite_list* list, Apt_suite* ts);
void Apt_suite_list_destroy(Apt_suite_list* list);

void Apt_data_init(Apt_data* data);
void Apt_data_create(Apt_data** data);
void Apt_data_destroy(Apt_data* data);

#endif