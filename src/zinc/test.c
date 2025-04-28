#include "test.h"
#include "memory.h"
#include "list.h"
#include <assert.h>

void Zinc_test_init(Zinc_test* test)
{
    Zinc_string_init(&test->name);
    test->solo = false;
    test->mute = false;

    test->has_solo = false;
    test->ran = false;
    test->pass = false;
    test->dry_run = false;
    test->func = NULL;
    test->has_printed = false;
    test->check_count = 0;
    test->check_passed = 0;
    test->check_failed = 0;

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

void Zinc_test_stat_init(Zinc_test_stat* stat)
{
    stat->count = 0;
    stat->skip = 0;
    stat->passed = 0;
    stat->failed = 0;
    stat->check_count = 0;
    stat->check_passed = 0;
    stat->check_failed = 0;
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
                stat->passed++;
            } else {
                stat->failed++;
            }
            stat->check_count += test->check_count;
            stat->check_passed += test->check_passed;
            stat->check_failed += test->check_failed;
        } else {
            stat->skip++;
        }
    }
}

void Zinc_test_print(Zinc_test_stat* stat)
{
    double pct = 100.0;
    if (stat->passed + stat->failed > 0) {
        pct = (double)stat->passed / ((double)stat->passed + (double)stat->failed) * 100.0;
    }
    printf(
        "%.1f%% (%zu/%zu test cases) (%zu skipped) (%zu/%zu checks)\n",
        pct,
        stat->passed,
        stat->passed + stat->failed,
        stat->skip,
        stat->check_passed,
        stat->check_passed + stat->check_failed);
}

void Zinc_test_print_unseen(Zinc_test* test)
{
    Zinc_list tests;
    Zinc_list_init(&tests);
    Zinc_test* p = test;
    while (p) {
        Zinc_list_add_item(&tests, p);
        p = p->parent;
    }
    Zinc_list_node* node = tests.tail;
    while (node) {
        Zinc_test* test2 = node->item;
        if (!test2->has_printed) {
            test2->has_printed = true;
            fprintf(stderr, "%s\n", Zinc_string_c_str(&test2->name));
        }
        node = node->prev;
    }
}

void Zinc_test_register(Zinc_test* test, Zinc_func func)
{
    Zinc_test* new_test = NULL;
    Zinc_test_create(&new_test);
    new_test->func = func;
    new_test->dry_run = test->dry_run;
    Zinc_test_add(test, new_test);
    func(new_test);
}

bool Zinc_test_should_run(Zinc_test* test)
{
    return !test->mute && (!test->parent || !test->parent->has_solo || (test->solo));
}

void Zinc_test_perform(Zinc_test* test)
{
    Zinc_test* p = test->head;
    while (p) {
        if (Zinc_test_should_run(p)) {
            p->dry_run = false;
            p->ran = true;
            p->pass = true;
            p->func(p);
        }
        p = p->next;
    }
}