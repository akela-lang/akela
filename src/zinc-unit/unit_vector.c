#include "zinc/vector.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_vector_double(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(double));

    double v0 = 5;
    double v1[4] = {1, 2, 3, 4};
    double v2 = 150;
    Zinc_vector_add(v, &v0, 1);
    Zinc_vector_add(v, v1, 4);
    Zinc_vector_add(v, &v2, 1);
    Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, 0), v0, "v0");
    Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, 1), v1[0], "v1[0]");
    Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, 2), v1[1], "v1[1]");
    Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, 3), v1[2], "v1[2]");
    Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, 4), v1[3], "v1[3]");
    Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, 5), v2, "v2");

    Zinc_vector_destroy(v);
    free(v);
}

void Zinc_unit_vector_double_large(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(double));

    double value[100];
    for (int i = 0; i < 100; i++) {
        value[i] = i;
    }
    Zinc_vector_add(v, value, 100);

    for (int i = 0; i < 100; i++) {
        Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, i), value[i], "value[i]");
    }

    Zinc_vector_destroy(v);
    free(v);
}

void Zinc_unit_vector_shift(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(double));

    double x = 0.5;
    for (int i = 0; i < 100; i++) {
        Zinc_vector_add(v, &x, 1);
        x += 1.0;
    }

    x = -0.5;
    for (int i = 0; i < 100; i++) {
        Zinc_vector_shift(v, &x, 1);
        x -= 1.0;
    }

    x = -99.5;
    Zinc_test_expect_size_t_equal(test, v->count, 200, "count");
    for (int i = 0; i < 200; i++) {
        Zinc_test_expect_double_equal(test, ZINC_VECTOR_DOUBLE(v, i), x, "x");
        x += 1.0;
    }

    Zinc_vector_destroy(v);
    free(v);
}

void Zinc_unit_vector(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_vector_double);
        Zinc_test_register(test, Zinc_unit_vector_double_large);
        Zinc_test_register(test, Zinc_unit_vector_shift);

        return;
    }

    Zinc_test_perform(test);
}
