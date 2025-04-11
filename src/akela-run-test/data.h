#ifndef AKELA_RUN_TEST_DATA_H
#define AKELA_RUN_TEST_DATA_H

#include <centipede/module_finder.h>
#include <zinc/spec_error.h>

#include "cobble/compile.h"
#include "centipede/value.h"
#include "centipede/module_finder_file.h"
#include "centipede/comp_table.h"
#include "zinc/input_unicode_string.h"

typedef struct Run_cent_data {
    Cent_module_finder_file* mf;
    Cent_comp_table* ct;
    Zinc_vector* v;
    Zinc_input_unicode_string* input;
    Zinc_string file_name;
    bool valid;
} Run_cent_data;

typedef struct Run_test {
    Zinc_string ake;
    Zinc_string llvm;
    Zinc_string config;
    struct Run_test* next;
    struct Run_test* prev;
    Run_cent_data* config_data;
    bool solo;
    bool mute;
    void* return_address;
    long long return_size;
    Zinc_string name;
} Run_test;

typedef struct Run_test_list {
    Run_test* head;
    Run_test* tail;
} Run_test_list;

typedef struct Run_data {
    Cob_re ext_re;
    Cob_re separator_re;
    Cob_re regex_re;
    Run_test_list tests;
    size_t test_count;
    size_t test_passed_count;
    size_t test_failed_count;
    bool has_solo;
    Run_cent_data* type_info;
    Zinc_error_list errors;
    Zinc_spec_error_list spec_errors;
} Run_data;

typedef struct {
    bool matched;
    Zinc_string* actual;
    Zinc_string* expected;
} Run_pair;

typedef struct Art_test Art_test;
struct Art_test {
    bool solo;
    bool mute;
    bool snapshot;
    Zinc_spec_error_list spec_errors;
};

typedef struct Art_suite Art_suite;
struct Art_suite {
    Zinc_string path;
    Zinc_string description;
    bool solo;
    bool mute;
    bool has_solo;
    Zinc_error_list errors;
    Zinc_vector tests;
};

typedef struct Art_data Art_data;
struct Art_data {
    bool has_solo;
    Zinc_vector suites;
};

typedef enum {
    Run_type_int8,
    Run_type_int16,
    Run_type_int32,
    Run_type_int64,
    Run_type_nat8,
    Run_type_nat16,
    Run_type_nat32,
    Run_type_nat64,
    Run_type_real16,
    Run_type_real32,
    Run_type_real64,
    Run_type_bool,
} Run_type;

void Run_config_data_init(Run_cent_data* config_data, Zinc_string* file_name);
void Run_cent_data_create(Run_cent_data** config_data, Zinc_string* file_name);
void Run_cent_data_destroy(Run_cent_data* config_data);

void Run_test_init(Run_test *test);
void Run_test_create(Run_test** test);
void Run_test_destroy(Run_test* test);

void Run_test_list_init(Run_test_list* list);
void Run_test_list_create(Run_test_list** list);
void Run_test_list_add(Run_test_list* list, Run_test* test);
void Run_test_list_destroy(Run_test_list* list);

void Run_data_init(Run_data* data);
void Run_data_create(Run_data** data);
void Run_data_destroy(Run_data* data);

void Run_pair_init(Run_pair* pair);
void Run_pair_create(Run_pair** pair);
void Run_pair_destroy(Run_pair* pair);

void Art_test_init(Art_test* test);
void Art_test_destroy(Art_test* test);

void Art_suite_init(Art_suite* suite);
void Art_suite_destroy(Art_suite* suite);

void Art_data_init(Art_data* data);
void Art_data_destroy(Art_data* data);

#endif