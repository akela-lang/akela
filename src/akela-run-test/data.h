#ifndef AKELA_RUN_TEST_DATA_H
#define AKELA_RUN_TEST_DATA_H

#include "cobble/compile.h"
#include "centipede/value.h"

typedef struct Run_test {
    Zinc_string ake;
    Zinc_string llvm;
    Zinc_string config_string;
    Cent_value* config;
    struct Run_test* next;
    struct Run_test* prev;
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
} Run_data;

typedef struct {
    bool matched;
    Zinc_string* actual;
    Zinc_string* expected;
} Run_pair;

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

#endif