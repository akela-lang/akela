#include "zinc/unit_test.h"
#include "centipede/value.h"

void test_value_set()
{
    test_name(__func__);

    Cent_value* value = NULL;
    Cent_value_create(&value);

    Cent_value* v0 = NULL;
    Cent_value_create(&v0);

    Cent_value* v1 = NULL;
    Cent_value_create(&v1);

    Cent_value_set_str(value, "a", v0);
    Cent_value_set_str(value, "b", v1);

    Cent_value* temp0 = Cent_value_get_str(value, "a");
    expect_ptr_equal(temp0, v0, "ptr equal temp0");

    Cent_value* temp1 = Cent_value_get_str(value, "b");
    expect_ptr_equal(temp1, v1, "ptr equal temp1");

    Cent_value_destroy(value);
    free(value);
}

void test_value_add()
{
    test_name(__func__);

    Cent_value* value = NULL;
    Cent_value_create(&value);

    Cent_value* v0 = NULL;
    Cent_value_create(&v0);

    Cent_value* v1 = NULL;
    Cent_value_create(&v1);

    Cent_value* v2 = NULL;
    Cent_value_create(&v2);

    Cent_value_add(value, v0);
    Cent_value_add(value, v1);
    Cent_value_add(value, v2);

    expect_ptr_equal(value->head, v0, "head value");
    expect_ptr_equal(v0->prev, NULL, "prev v0");
    expect_ptr_equal(v0->next, v1, "next v0");
    expect_ptr_equal(v1->prev, v0, "prev v1");
    expect_ptr_equal(v1->next, v2, "next v1");
    expect_ptr_equal(v2->prev, v1, "prev v2");
    expect_ptr_equal(v2->next, NULL, "next v2");
    expect_ptr_equal(value->tail, v2, "tail value");

    Cent_value_destroy(value);
    free(value);
}

void test_value()
{
    test_value_set();
    test_value_add();
}