#include "zinc/vector.h"
#include "zinc/unit_test.h"

void test_vector_double()
{
    Zinc_test_name(__func__);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(double));

    double v0 = 5;
    double v1[4] = {1, 2, 3, 4};
    double v2 = 150;
    VectorAdd(v, &v0, 1);
    VectorAdd(v, v1, 4);
    VectorAdd(v, &v2, 1);
    Zinc_expect_double_equal(VECTOR_DOUBLE(v, 0), v0, "v0");
    Zinc_expect_double_equal(VECTOR_DOUBLE(v, 1), v1[0], "v1[0]");
    Zinc_expect_double_equal(VECTOR_DOUBLE(v, 2), v1[1], "v1[1]");
    Zinc_expect_double_equal(VECTOR_DOUBLE(v, 3), v1[2], "v1[2]");
    Zinc_expect_double_equal(VECTOR_DOUBLE(v, 4), v1[3], "v1[3]");
    Zinc_expect_double_equal(VECTOR_DOUBLE(v, 5), v2, "v2");

    VectorDestroy(v);
    free(v);
}

void test_vector_double_large()
{
    Zinc_test_name(__func__);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(double));

    double value[100];
    for (int i = 0; i < 100; i++) {
        value[i] = i;
    }
    VectorAdd(v, value, 100);

    for (int i = 0; i < 100; i++) {
        Zinc_expect_double_equal(VECTOR_DOUBLE(v, i), value[i], "value[i]");
    }

    VectorDestroy(v);
    free(v);
}

void test_vector()
{
    test_vector_double();
    test_vector_double_large();
}
