#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_test_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_test* list = NULL;
    Zinc_test_create(&list);

    Zinc_test* t0 = NULL;
    Zinc_test_create(&t0);

    Zinc_test* t1 = NULL;
    Zinc_test_create(&t1);

    Zinc_test* t2 = NULL;
    Zinc_test_create(&t2);

    Zinc_test_add(list, t0);
    Zinc_test_add(list, t1);
    Zinc_test_add(list, t2);

    Zinc_expect_ptr_equal(test, list->head, t0, "head list");
    Zinc_expect_ptr_equal(test, t0->prev, NULL, "prev t0");
    Zinc_expect_ptr_equal(test, t0->next, t1, "next t0");
    Zinc_expect_ptr_equal(test, t1->prev, t0, "prev t1");
    Zinc_expect_ptr_equal(test, t1->next, t2, "next t1");
    Zinc_expect_ptr_equal(test, t2->prev, t1, "prev t2");
    Zinc_expect_ptr_equal(test, t2->next, NULL, "next t2");
    Zinc_expect_ptr_equal(test, list->tail, t2, "tail list");

    Zinc_test_destroy(list);
    free(list);
}

void Zinc_unit_test(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_test_add);

        return;
    }

    Zinc_test_perform(test);
}