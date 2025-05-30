#include "dataframe/data_frame.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void DfUnit_DataFrame1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Series* s0 = NULL;
    SeriesCreate(&s0);

    Series* s1 = NULL;
    SeriesCreate(&s1);

    DataFrame* df = NULL;
    DataFrameCreate(&df);

    DataFrameAdd(df, s0);

    DataFrameAdd(df, s1);

    Zinc_assert_ptr(test, s0, "ptr s0");
    Zinc_assert_ptr(test, s1, "ptr s1");
    Zinc_assert_ptr(test, df, "ptr df");

    Zinc_expect_ptr_equal(test, df->head, s0, "head");
    Zinc_expect_ptr_equal(test, s0->prev, NULL, "s0 prev");
    Zinc_expect_ptr_equal(test, s0->next, s1, "s0 next");
    Zinc_expect_ptr_equal(test, s1->prev, s0, "s1 prev");
    Zinc_expect_ptr_equal(test, s1->next, NULL, "s1 next");
    Zinc_expect_ptr_equal(test, df->tail, s1, "tail");

    DataFrameDestroy(df);
    free(df);
}

void DfUnit_DataFrame(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, DfUnit_DataFrame1);

        return;
    }

    Zinc_test_perform(test);
}