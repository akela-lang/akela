#include "test.h"
#include "memory.h"

void Zinc_test_init(Zinc_test* test)
{
    Zinc_string_init(&test->name);
    test->solo = false;
    test->mute = false;

    test->has_solo = false;
    test->ran = false;
    test->pass = false;

    test->has_printed = false;

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

void Zinc_test_stat_init(Zinc_test_stat* stat)
{
    stat->count = 0;
    stat->skip = 0;
    stat->pass = 0;
    stat->fail = 0;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Zinc_test_count(Zinc_test* test, Zinc_test_stat* stat)
{
    if (test->head) {
        Zinc_test* p = test->head;
        while (p) {
            Zinc_test_count(p, stat);
            p = p->next;
        }
    } else {
        if (test->ran) {
            stat->count++;
            if (test->pass) {
                stat->pass++;
            } else {
                stat->fail++;
            }
        } else {
            stat->skip++;
        }
    }
}

void Zinc_test_print(Zinc_test_stat* stat)
{
    double pct = 100.0;
    if (stat->pass + stat->fail > 0) {
        pct = (double)stat->pass / ((double)stat->pass + (double)stat->fail) * 100.0;
    }
    printf("pct: %.1f%%\n", pct);
    printf("pass: %zu\n", stat->pass);
    printf("fail: %zu\n", stat->fail);
    printf("skip: %zu\n", stat->skip);
}