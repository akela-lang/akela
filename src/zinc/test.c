#include "test.h"
#include "memory.h"

void Zinc_test_init(Zinc_test* test)
{
    Zinc_string_init(&test->name);
    test->solo = false;
    test->mute = false;

    test->has_solo = false;
    test->test_count = 0;
    test->pass_count = 0;
    test->fail_count = 0;

    test->next = NULL;
    test->prev = NULL;
    test->head = NULL;
    test->tail = NULL;
    test->parent = NULL;
}

void Zinc_test_create(Zinc_test** test)
{
    Zinc_malloc_safe((void**)test, sizeof(Zinc_test));
    Zinc_test_init(*test);
}

void Zinc_test_add(Zinc_test* p, Zinc_test* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }

    c->parent = p;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Zinc_test_destroy(Zinc_test* test)
{
    Zinc_test* p = test->head;
    while (p) {
        Zinc_test* temp = p;
        p = p->next;
        Zinc_test_destroy(temp);
        free(temp);
    }
}

bool Zinc_test_should_run(Zinc_test* test)
{
    return !test->mute && (!test->parent || !test->parent->has_solo || (test->solo));
}
