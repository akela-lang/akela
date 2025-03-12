#ifndef AKELA_PARSE_TEST_DATA_H
#define AKELA_PARSE_TEST_DATA_H

#include "zinc/zstring.h"
#include "zinc/spec_error.h"
#include "centipede/value.h"
#include "centipede/comp_table.h"

typedef struct Apt_test_case Apt_test_case;
struct Apt_test_case {
    bool solo;
    bool mute;
    Zinc_string text;
    Zinc_string source;
    Zinc_string expected;
    Zinc_string name;
    Zinc_error_list expected_errors;
    Cent_comp_table* text_ct;
    Cent_comp_table* expected_ct;
    Apt_test_case* next;
    Apt_test_case* prev;
};

typedef struct Apt_test_list Apt_test_list;
struct Apt_test_list {
    Apt_test_case* head;
    Apt_test_case* tail;
};

typedef struct Apt_test_suite Apt_test_suite;
struct Apt_test_suite {
    Zinc_string text;
    Apt_test_list list;
    bool solo;
    bool mute;
    Zinc_string name;
    Apt_test_suite* next;
    Apt_test_suite* prev;
};

typedef struct Apt_suite_list Apt_suite_list;
struct Apt_suite_list {
    Apt_test_suite* head;
    Apt_test_suite* tail;
};

void Apt_test_case_init(Apt_test_case *tc);
void Apt_test_case_create(Apt_test_case** tc);
void Apt_test_case_destroy(Apt_test_case *tc);

void Apt_test_list_init(Apt_test_list* list);
void Apt_test_list_create(Apt_test_list** list);
void Apt_test_list_add(Apt_test_list* list, Apt_test_case* tc);
void Apt_test_list_destroy(Apt_test_list* list);

void Apt_test_suite_init(Apt_test_suite *ts);
void Apt_test_suite_create(Apt_test_suite** ts);
void Apt_test_suite_destroy(Apt_test_suite* ts);

void Apt_suite_list_init(Apt_suite_list* list);
void Apt_suite_list_create(Apt_suite_list** list);
void Apt_suite_list_add(Apt_suite_list* list, Apt_test_suite* ts);
void Apt_suite_list_destroy(Apt_suite_list* list);

#endif