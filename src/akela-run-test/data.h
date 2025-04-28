#ifndef AKELA_RUN_TEST_DATA_H
#define AKELA_RUN_TEST_DATA_H

#include <zinc/spec_error.h>
#include "cobble/compile.h"
#include "centipede/value.h"
#include "centipede/comp_table.h"
#include "zinc/input_unicode_string.h"

typedef struct {
    bool matched;
    Zinc_string* actual;
    Zinc_string* expected;
} Art_pair;

typedef struct Art_test Art_test;
struct Art_test {
    Zinc_string description;
    bool snapshot;
    bool has_error;
    Zinc_test* test;
    Zinc_input_bounds source_bounds;
    Zinc_input_bounds llvm_bounds;
    Zinc_string llvm;
    Cent_comp_table* ct;
    Cent_value* value;
    Zinc_spec_error_list spec_errors;
    void* return_address;
    long long return_size;
    Art_test* next;
    Art_test* prev;
};

typedef struct Art_suite Art_suite;
struct Art_suite {
    Zinc_string path;
    Zinc_string description;
    Zinc_string name;
    Zinc_test* test;
    Zinc_error_list errors;
    Art_suite* next;
    Art_suite* prev;
    Art_test* head;
    Art_test* tail;
};

typedef struct Art_data Art_data;
struct Art_data {
    Zinc_string dir_path;
    bool has_solo;
    Cent_comp_table* type_info;
    Cob_re regex_re;
    Zinc_test* test;
    Art_suite* head;
    Art_suite* tail;
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

void Art_pair_init(Art_pair* pair);
void Art_pair_create(Art_pair** pair);
void Art_pair_destroy(Art_pair* pair);

void Art_test_init(Art_test* test);
void Art_test_create(Art_test** test);
void Art_test_destroy(Art_test* test);

void Art_suite_init(Art_suite* suite);
void Art_suite_create(Art_suite** suite);
void Art_suite_destroy(Art_suite* suite);
void Art_suite_add(Art_suite* suite, Art_test* test);

void Art_data_init(Art_data* data);
void Art_data_destroy(Art_data* data);
void Art_data_add(Art_data* data, Art_suite* suite);

#endif