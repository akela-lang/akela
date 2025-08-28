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

typedef struct Apt_case_data Apt_case_data;
struct Apt_case_data {
    bool snapshot;
    bool has_error;
    Zinc_string source_name;
    Zinc_input_bounds source_bounds;
    Zinc_input_bounds ast_bounds;
    Zinc_string description;
    Zinc_error_list errors;
    Zinc_spec_error_list spec_errors;
    Apt_case_data* next;
    Apt_case_data* prev;
};

typedef struct Apt_case_data_list Apt_case_data_list;
struct Apt_case_data_list {
    Apt_case_data* head;
    Apt_case_data* tail;
};

typedef struct Apt_suite_data Apt_suite_data;
struct Apt_suite_data {
    Zinc_string path;
    Zinc_string name;
    Zinc_string description;
    Zinc_string text;
    Zinc_error_list errors;
    Apt_case_data_list list;
    Apt_suite_data* next;
    Apt_suite_data* prev;
};

typedef struct Apt_suite_data_list Apt_suite_data_list;
struct Apt_suite_data_list {
    Apt_suite_data* head;
    Apt_suite_data* tail;
};

typedef struct Apt_top_data Apt_top_data;
struct Apt_top_data
{
    Zinc_string dir_path;
};

void Apt_case_data_init(Apt_case_data *case_data);
void Apt_case_data_create(Apt_case_data** case_data);
void Apt_case_data_destroy(Apt_case_data *case_data);

void Apt_case_data_list_init(Apt_case_data_list* list);
void Apt_case_data_list_create(Apt_case_data_list** list);
void Apt_case_data_list_add(Apt_case_data_list* list, Apt_case_data* case_data);
void Apt_case_data_list_destroy(Apt_case_data_list* list);

void Apt_suite_data_init(Apt_suite_data *suite_data);
void Apt_suite_data_create(Apt_suite_data** suite_data);
void Apt_suite_data_destroy(Apt_suite_data* suite_data);

void Apt_suite_data_list_init(Apt_suite_data_list* list);
void Apt_suite_data_list_create(Apt_suite_data_list** list);
void Apt_suite_data_list_add(Apt_suite_data_list* list, Apt_suite_data* suite_data);
void Apt_suite_data_list_destroy(Apt_suite_data_list* list);

void Apt_top_data_init(Apt_top_data* top_data);
void Apt_top_data_create(Apt_top_data** top_data);
void Apt_top_data_destroy(Apt_top_data* top_data);

#endif