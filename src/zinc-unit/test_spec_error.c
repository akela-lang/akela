#include "zinc/unit_test.h"
#include "zinc/spec_error.h"

void test_spec_error_add()
{
    Zinc_test_name(__func__);

    Zinc_spec_error_list list;
    Zinc_spec_error_list_init(&list);

    Zinc_spec_error* e0 = NULL;
    Zinc_spec_error_create(&e0);

    Zinc_spec_error* e1 = NULL;
    Zinc_spec_error_create(&e1);

    Zinc_spec_error* e2 = NULL;
    Zinc_spec_error_create(&e2);

    Zinc_spec_error_list_add(&list, e0);
    Zinc_spec_error_list_add(&list, e1);
    Zinc_spec_error_list_add(&list, e2);

    Zinc_expect_ptr_equal(list.head, e0, "head list");
    Zinc_expect_ptr_equal(e0->prev, NULL, "prev e0");
    Zinc_expect_ptr_equal(e0->next, e1, "next e0");
    Zinc_expect_ptr_equal(e1->prev, e0, "prev e1");
    Zinc_expect_ptr_equal(e1->next, e2, "next e1");
    Zinc_expect_ptr_equal(e2->prev, e1, "prev e2");
    Zinc_expect_ptr_equal(e2->next, NULL, "next e2");
    Zinc_expect_ptr_equal(list.tail, e2, "tail list");

    Zinc_spec_error_list_destroy(&list);
}

void test_spec_error()
{
    test_spec_error_add();
}