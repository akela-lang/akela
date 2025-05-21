#ifndef ZINC_TEST_H
#define ZINC_TEST_H

#include <stdbool.h>
#include "zstring.h"

typedef struct Zinc_test Zinc_test;
typedef void (*Zinc_func)(Zinc_test* test);

struct Zinc_test {
    Zinc_string name;
    bool solo;
    bool mute;

    bool has_solo;
    bool ran;
    bool pass;
    bool dry_run;
    Zinc_func func;
    void* data;

    size_t check_count;
    size_t check_passed;
    size_t check_failed;

    bool has_printed;

    Zinc_test *next;
    Zinc_test *prev;
    Zinc_test *head;
    Zinc_test *tail;
    Zinc_test *parent;
};

typedef struct Zinc_test_stat Zinc_test_stat;
struct Zinc_test_stat {
    size_t count;
    size_t skip;
    size_t passed;
    size_t failed;
    size_t check_count;
    size_t check_passed;
    size_t check_failed;
};

void Zinc_test_init(Zinc_test* test);
void Zinc_test_create(Zinc_test** test);
void Zinc_test_add(Zinc_test* p, Zinc_test* c);
void Zinc_test_destroy(Zinc_test* test);
bool Zinc_test_should_run(Zinc_test* test);
void Zinc_test_stat_init(Zinc_test_stat* stat);
void Zinc_test_count(Zinc_test* test, Zinc_test_stat* stat);
void Zinc_test_print(Zinc_test_stat* stat);
void Zinc_test_print_unseen(Zinc_test* test);
void Zinc_test_register(Zinc_test* test, Zinc_func func);
void Zinc_test_register_data(Zinc_test* test, Zinc_func func, void* data);
void Zinc_test_perform(Zinc_test* test);

#endif