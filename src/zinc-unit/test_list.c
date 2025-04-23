#include "zinc/unit_test.h"
#include "zinc/list.h"
#include "zinc/memory.h"

void test_list_1()
{
    Zinc_test_name(__func__);

    Zinc_list l;
    Zinc_list_init(&l);

    Zinc_list_node* n0 = NULL;
    Zinc_list_node_create(&n0);

    Zinc_list_node* n1 = NULL;
    Zinc_list_node_create(&n1);

    Zinc_list_node* n2 = NULL;
    Zinc_list_node_create(&n2);

    Zinc_list_add(&l, n0);
    Zinc_list_add(&l, n1);
    Zinc_list_add(&l, n2);

    Zinc_expect_ptr_equal(l.head, n0, "head l");
    Zinc_expect_ptr_equal(n0->prev, NULL, "prev n0");
    Zinc_expect_ptr_equal(n0->next, n1, "next n0");
    Zinc_expect_ptr_equal(n1->prev, n0, "prev n1");
    Zinc_expect_ptr_equal(n1->next, n2, "next n1");
    Zinc_expect_ptr_equal(n2->prev, n1, "prev n2");
    Zinc_expect_ptr_equal(n2->next, NULL, "next n2");
    Zinc_expect_ptr_equal(l.tail, n2, "tail l");

    Zinc_list_destroy(&l, (Zinc_list_node_destroy)free);
}

void test_list_remove_first_item()
{
    Zinc_test_name(__func__);

    int one = 1;
    int two = 2;
    int three = 3;

    struct Zinc_list* l = NULL;
    Zinc_list_create(&l);

    Zinc_list_add_item(l, &one);
    Zinc_list_add_item(l, &two);
    Zinc_list_add_item(l, &three);

    int *tmp = NULL;

    tmp = Zinc_list_remove_first_item(l);
    Zinc_assert_ptr(tmp, "ptr 1");
    Zinc_expect_int_equal(*tmp, one, "one");

    tmp = Zinc_list_remove_first_item(l);
    Zinc_assert_ptr(tmp, "ptr 2");
    Zinc_expect_int_equal(*tmp, two, "two");

    tmp = Zinc_list_remove_first_item(l);
    Zinc_assert_ptr(tmp, "ptr 3");
    Zinc_expect_int_equal(*tmp, three, "three");

    tmp = Zinc_list_remove_first_item(l);
    Zinc_expect_null(tmp, "ptr null");

    Zinc_list_destroy(l, NULL);
    free(l);
}

void test_list()
{
    test_list_1();
    test_list_remove_first_item();
}