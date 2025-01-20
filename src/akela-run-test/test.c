#include "zinc/unit_test.h"
#include "data.h"

void run_unit_test_list();

int main()
{
    run_unit_test_list();
    Zinc_print_results();
    return 0;
}

void run_unit_test_list()
{
    Zinc_test_name(__func__);

    Run_test_list* list = NULL;
    Run_test_list_create(&list);

    Run_test* t0 = NULL;
    Run_test_create(&t0);

    Run_test* t1 = NULL;
    Run_test_create(&t1);

    Run_test* t2 = NULL;
    Run_test_create(&t2);

    Run_test_list_add(list, t0);
    Run_test_list_add(list, t1);
    Run_test_list_add(list, t2);

    Zinc_expect_ptr_equal(list->head, t0, "head list");
    Zinc_expect_ptr_equal(t0->prev, NULL, "prev t0");
    Zinc_expect_ptr_equal(t0->next, t1, "next t0");
    Zinc_expect_ptr_equal(t1->prev, t0, "prev t1");
    Zinc_expect_ptr_equal(t1->next, t2, "next t1");
    Zinc_expect_ptr_equal(t2->prev, t1, "prev t2");
    Zinc_expect_ptr_equal(t2->next, NULL, "next t2");
    Zinc_expect_ptr_equal(list->tail, t2, "tail list");

    Run_test_list_destroy(list);
}