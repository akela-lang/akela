#include "centipede/value.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void CentUnit_value_set(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_dag);

    Cent_value* v0 = NULL;
    Cent_value_create(&v0);

    Cent_value* v1 = NULL;
    Cent_value_create(&v1);

    Cent_value_set_str(value, "a", v0);
    Cent_value_set_str(value, "b", v1);

    Cent_value* temp0 = Cent_value_get_str(value, "a");
    Zinc_test_expect_ptr_equal(test, temp0, v0, "ptr equal temp0");

    Cent_value* temp1 = Cent_value_get_str(value, "b");
    Zinc_test_expect_ptr_equal(test, temp1, v1, "ptr equal temp1");

    Cent_value_destroy(value);
    free(value);
}

void CentUnit_value_add(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Cent_value* value = NULL;
    Cent_value_create(&value);
    Cent_value_set_type(value, Cent_value_type_dag);

    Cent_value* v0 = NULL;
    Cent_value_create(&v0);

    Cent_value* v1 = NULL;
    Cent_value_create(&v1);

    Cent_value* v2 = NULL;
    Cent_value_create(&v2);

    Cent_value_add(value, v0);
    Cent_value_add(value, v1);
    Cent_value_add(value, v2);

    Zinc_test_expect_ptr_equal(test, value->data.dag.head, v0, "head value");
    Zinc_test_expect_ptr_equal(test, v0->prev, NULL, "prev v0");
    Zinc_test_expect_ptr_equal(test, v0->next, v1, "next v0");
    Zinc_test_expect_ptr_equal(test, v1->prev, v0, "prev v1");
    Zinc_test_expect_ptr_equal(test, v1->next, v2, "next v1");
    Zinc_test_expect_ptr_equal(test, v2->prev, v1, "prev v2");
    Zinc_test_expect_ptr_equal(test, v2->next, NULL, "next v2");
    Zinc_test_expect_ptr_equal(test, value->data.dag.tail, v2, "tail value");

    Cent_value_destroy(value);
    free(value);
}

void CentUnit_value(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CentUnit_value_set);
        Zinc_test_register(test, CentUnit_value_add);

        return;
    }

    Zinc_test_perform(test);
}