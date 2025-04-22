#include "zinc/unit_test.h"
#include "zinc/test.h"

void test_test_add()
{
    Zinc_test_name(__func__);

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

    Zinc_expect_ptr_equal(list->head, t0, "head list");
    Zinc_expect_ptr_equal(t0->prev, NULL, "prev t0");
    Zinc_expect_ptr_equal(t0->next, t1, "next t0");
    Zinc_expect_ptr_equal(t1->prev, t0, "prev t1");
    Zinc_expect_ptr_equal(t1->next, t2, "next t1");
    Zinc_expect_ptr_equal(t2->prev, t1, "prev t2");
    Zinc_expect_ptr_equal(t2->next, NULL, "next t2");
    Zinc_expect_ptr_equal(list->tail, t2, "tail list");

    Zinc_test_destroy(list);
    free(list);
}

void test_test()
{
    test_test_add();
}