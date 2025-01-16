#include "zinc/unit_test.h"
#include "zinc/list.h"
#include "zinc/memory.h"

void destroy_int(int* x)
{
    free(x);
}

void test_list_1()
{
    test_name(__func__);

    struct Zinc_list l;
    Zinc_list_init(&l);

    int* n0 = NULL;
    Zinc_malloc_safe((void**)&n0, sizeof(int));
    *n0 = 0;
    Zinc_list_add_item(&l, n0);

    int* n1 = NULL;
    Zinc_malloc_safe((void**)&n1, sizeof(int));
    *n1 = 1;
    Zinc_list_add_item(&l, n1);

    int* n2 = NULL;
    Zinc_malloc_safe((void**)&n2, sizeof(int));
    *n2 = 2;
    Zinc_list_add_item(&l, n2);

    int* x0 = Zinc_list_get(&l, 0);
    assert_ptr(x0, "ptr x0");
    expect_ptr_equal(x0, n0, "x0");
    expect_int_equal(*x0, 0, "0");

    int* x1 = Zinc_list_get(&l, 1);
    assert_ptr(x1, "ptr x1");
    expect_ptr_equal(x1, n1, "x1");
    expect_int_equal(*x1, 1, "1");

    int* x2 = Zinc_list_get(&l, 2);
    assert_ptr(x2, "ptr x2");
    expect_ptr_equal(x2, n2, "x2");
    expect_int_equal(*x2, 2, "2");

    Zinc_list_destroy(&l, (Zinc_list_node_destroy)destroy_int);
}

void test_list_remove_first_item()
{
    test_name(__func__);

    int one = 1;
    int two = 2;
    int three = 3;

    struct Zinc_list* l = NULL;
    Zinc_list_create(&l);

    Zinc_list_add_item(l, &one);
    Zinc_list_add_item(l, &two);
    Zinc_list_add_item(l, &three);

    int *tmp = NULL;

    tmp = list_remove_first_item(l);
    assert_ptr(tmp, "ptr 1");
    expect_int_equal(*tmp, one, "one");

    tmp = list_remove_first_item(l);
    assert_ptr(tmp, "ptr 2");
    expect_int_equal(*tmp, two, "two");

    tmp = list_remove_first_item(l);
    assert_ptr(tmp, "ptr 3");
    expect_int_equal(*tmp, three, "three");

    tmp = list_remove_first_item(l);
    expect_null(tmp, "ptr null");

    Zinc_list_destroy(l, NULL);
    free(l);
}

void test_list()
{
    test_list_1();
    test_list_remove_first_item();
}