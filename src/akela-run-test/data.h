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

typedef struct Art_case_data Art_case_data;
struct Art_case_data {
    Zinc_string description;
    bool snapshot;
    bool has_error;
    Zinc_input_bounds source_bounds;
    Zinc_input_bounds llvm_bounds;
    Zinc_string llvm;
    Cent_comp_table* ct;
    Cent_value* value;
    Zinc_spec_error_list spec_errors;
    void* return_address;
    long long return_size;
};

typedef struct Art_suite_data Art_suite_data;
struct Art_suite_data {
    Zinc_string path;
    Zinc_string description;
    Zinc_string name;
    Zinc_error_list errors;
};

typedef struct Art_top_data Art_top_data;
struct Art_top_data {
    Zinc_string dir_path;
    bool has_solo;
    Cent_comp_table* type_info;
    Cob_re regex_re;
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

void Art_case_data_init(Art_case_data* case_data);
void Art_case_data_create(Art_case_data** case_data);
void Art_case_data_destroy(Art_case_data* case_data);

void Art_suite_data_init(Art_suite_data* suite_data);
void Art_suite_data_create(Art_suite_data** suite_data);
void Art_suite_data_destroy(Art_suite_data* suite_data);

void Art_top_data_init(Art_top_data* top_data);
void Art_top_data_create(Art_top_data** top_data);
void Art_top_data_destroy(Art_top_data* top_data);

#endif