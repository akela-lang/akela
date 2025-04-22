#ifndef ZINC_TEST_H
#define ZINC_TEST_H

#include <stdbool.h>
#include "zstring.h"

typedef struct Zinc_test Zinc_test;

struct Zinc_test {
    Zinc_string name;
    bool solo;
    bool mute;

    bool has_solo;
    size_t test_count;
    size_t pass_count;
    size_t fail_count;

    Zinc_test *next;
    Zinc_test *prev;
    Zinc_test *head;
    Zinc_test *tail;
    Zinc_test *parent;
};

void Zinc_test_init(Zinc_test* test);
void Zinc_test_create(Zinc_test** test);
void Zinc_test_add(Zinc_test* p, Zinc_test* c);
void Zinc_test_destroy(Zinc_test* test);
bool Zinc_test_should_run(Zinc_test* test);

#endif