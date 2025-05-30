#include "zinc/spec_error.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_spec_error_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

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

    Zinc_expect_ptr_equal(test, list.head, e0, "head list");
    Zinc_expect_ptr_equal(test, e0->prev, NULL, "prev e0");
    Zinc_expect_ptr_equal(test, e0->next, e1, "next e0");
    Zinc_expect_ptr_equal(test, e1->prev, e0, "prev e1");
    Zinc_expect_ptr_equal(test, e1->next, e2, "next e1");
    Zinc_expect_ptr_equal(test, e2->prev, e1, "prev e2");
    Zinc_expect_ptr_equal(test, e2->next, NULL, "next e2");
    Zinc_expect_ptr_equal(test, list.tail, e2, "tail list");

    Zinc_spec_error_list_destroy(&list);
}

void Zinc_unit_spec_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_spec_error_add);

        return;
    }

    Zinc_test_perform(test);
}