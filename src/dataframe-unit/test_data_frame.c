#include "zinc/unit_test.h"
#include "dataframe/data_frame.h"

void TestDataFrame1()
{
    Zinc_test_name(__func__);

    Series* s0 = NULL;
    SeriesCreate(&s0);

    Series* s1 = NULL;
    SeriesCreate(&s1);

    DataFrame* df = NULL;
    DataFrameCreate(&df);

    DataFrameAdd(df, s0);

    DataFrameAdd(df, s1);

    Zinc_assert_ptr(s0, "ptr s0");
    Zinc_assert_ptr(s1, "ptr s1");
    Zinc_assert_ptr(df, "ptr df");

    Zinc_expect_ptr_equal(df->head, s0, "head");
    Zinc_expect_ptr_equal(s0->prev, NULL, "s0 prev");
    Zinc_expect_ptr_equal(s0->next, s1, "s0 next");
    Zinc_expect_ptr_equal(s1->prev, s0, "s1 prev");
    Zinc_expect_ptr_equal(s1->next, NULL, "s1 next");
    Zinc_expect_ptr_equal(df->tail, s1, "tail");

    DataFrameDestroy(df);
    free(df);
}

void TestDataFrame()
{
    TestDataFrame1();
}