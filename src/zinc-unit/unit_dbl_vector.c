#include "zinc/dbl_vector.h"

#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_dbl_vector_expand_right(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_dbl_vector dv;
    Zinc_dbl_vector_init(&dv, sizeof(double));

    Zinc_expect_size_t_equal(test, dv.start, 0, "0 start dv");
    Zinc_expect_size_t_equal(test, dv.end, 0, "0 end dv");
    Zinc_expect_size_t_equal(test, dv.capacity, 0, "0 capacity dv");
    Zinc_expect_size_t_equal(test, dv.item_size, sizeof(double), "0 item_size dv");

    // 4 double
    Zinc_dbl_vector_expand_right(&dv, 32);
    Zinc_expect_size_t_equal(test, dv.start, 32, "1 start dv");
    Zinc_expect_size_t_equal(test, dv.end, 32, "1 end dv");
    Zinc_expect_size_t_equal(test, dv.capacity, 64, "1 capacity dv");
    Zinc_expect_size_t_equal(test, dv.item_size, sizeof(double), "1 item_size dv");

    // 4 double
    Zinc_dbl_vector_expand_right(&dv, 32);
    Zinc_expect_size_t_equal(test, dv.start, 32, "2 start dv");
    Zinc_expect_size_t_equal(test, dv.end, 32, "2 end dv");
    Zinc_expect_size_t_equal(test, dv.capacity, 64, "2 capacity dv");
    Zinc_expect_size_t_equal(test, dv.item_size, sizeof(double), "2 item_size dv");

    Zinc_dbl_vector_destroy(&dv);
}

void Zinc_unit_dbl_vector_append(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_dbl_vector dv;
    Zinc_dbl_vector_init(&dv, sizeof(double));

    double items[4] = {0.5, 1.5, 2.5, 3.5};
    Zinc_dbl_vector_append(&dv, items, 4);

    double* p = NULL;
    size_t size = 0;

    p = Zinc_dbl_vector_slice(&dv, &size);
    Zinc_expect_double_equal(test, p[0], 0.5, "0");
    Zinc_expect_double_equal(test, p[1], 1.5, "1");
    Zinc_expect_double_equal(test, p[2], 2.5, "2");
    Zinc_expect_double_equal(test, p[3], 3.5, "3");

    items[0] = 4.5;
    items[1] = 5.5;
    items[2] = 6.5;
    items[3] = 7.5;

    Zinc_dbl_vector_append(&dv, &items, 4);
    p = Zinc_dbl_vector_slice(&dv, &size);
    Zinc_expect_double_equal(test, p[0], 0.5, "0");
    Zinc_expect_double_equal(test, p[1], 1.5, "1");
    Zinc_expect_double_equal(test, p[2], 2.5, "2");
    Zinc_expect_double_equal(test, p[3], 3.5, "3");
    Zinc_expect_double_equal(test, p[4], 4.5, "4");
    Zinc_expect_double_equal(test, p[5], 5.5, "5");
    Zinc_expect_double_equal(test, p[6], 6.5, "6");
    Zinc_expect_double_equal(test, p[7], 7.5, "7");

    items[0] = 8.5;
    items[1] = 9.5;
    items[2] = 10.5;
    items[3] = 11.5;

    Zinc_dbl_vector_append(&dv, &items, 4);
    p = Zinc_dbl_vector_slice(&dv, &size);
    Zinc_expect_double_equal(test, p[0], 0.5, "0");
    Zinc_expect_double_equal(test, p[1], 1.5, "1");
    Zinc_expect_double_equal(test, p[2], 2.5, "2");
    Zinc_expect_double_equal(test, p[3], 3.5, "3");
    Zinc_expect_double_equal(test, p[4], 4.5, "4");
    Zinc_expect_double_equal(test, p[5], 5.5, "5");
    Zinc_expect_double_equal(test, p[6], 6.5, "6");
    Zinc_expect_double_equal(test, p[7], 7.5, "7");
    Zinc_expect_double_equal(test, p[8], 8.5, "8");
    Zinc_expect_double_equal(test, p[9], 9.5, "9");
    Zinc_expect_double_equal(test, p[10], 10.5, "10");
    Zinc_expect_double_equal(test, p[11], 11.5, "11");

    Zinc_dbl_vector_destroy(&dv);
}

void Zinc_unit_dbl_vector_prepend(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_dbl_vector dv;
    Zinc_dbl_vector_init(&dv, sizeof(double));

    double items[12] = {
        0.5,
        1.5,
        2.5,
        3.5,
        4.5,
        5.5,
        6.5,
        7.5,
        8.5,
        9.5,
        10.5,
        11.5
    };
    Zinc_dbl_vector_prepend(&dv, &items[8], 4);
    size_t size;
    double* p;
    p = Zinc_dbl_vector_slice(&dv, &size);

    Zinc_expect_double_equal(test, p[0], 8.5, "0 8");
    Zinc_expect_double_equal(test, p[1], 9.5, "0 9");
    Zinc_expect_double_equal(test, p[2], 10.5, "0 10");
    Zinc_expect_double_equal(test, p[3], 11.5, "0 11");

    Zinc_dbl_vector_prepend(&dv, &items[4], 4);
    p = Zinc_dbl_vector_slice(&dv, &size);
    Zinc_expect_double_equal(test, p[0], 4.5, "1 4");
    Zinc_expect_double_equal(test, p[1], 5.5, "1 5");
    Zinc_expect_double_equal(test, p[2], 6.5, "1 6");
    Zinc_expect_double_equal(test, p[3], 7.5, "1 7");
    Zinc_expect_double_equal(test, p[4], 8.5, "1 8");
    Zinc_expect_double_equal(test, p[5], 9.5, "1 9");
    Zinc_expect_double_equal(test, p[6], 10.5, "1 10");
    Zinc_expect_double_equal(test, p[7], 11.5, "1 11");

    Zinc_dbl_vector_prepend(&dv, &items[0], 4);
    p = Zinc_dbl_vector_slice(&dv, &size);
    Zinc_expect_double_equal(test, p[0], 0.5, "2 0");
    Zinc_expect_double_equal(test, p[1], 1.5, "2 1");
    Zinc_expect_double_equal(test, p[2], 2.5, "2 2");
    Zinc_expect_double_equal(test, p[3], 3.5, "2 3");
    Zinc_expect_double_equal(test, p[4], 4.5, "2 4");
    Zinc_expect_double_equal(test, p[5], 5.5, "2 5");
    Zinc_expect_double_equal(test, p[6], 6.5, "2 6");
    Zinc_expect_double_equal(test, p[7], 7.5, "2 7");
    Zinc_expect_double_equal(test, p[8], 8.5, "2 8");
    Zinc_expect_double_equal(test, p[9], 9.5, "2 9");
    Zinc_expect_double_equal(test, p[10], 10.5, "2 10");
    Zinc_expect_double_equal(test, p[11], 11.5, "2 11");

    Zinc_dbl_vector_destroy(&dv);
}

void Zinc_unit_dbl_vector_append_and_prepend(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_dbl_vector dv;
    Zinc_dbl_vector_init(&dv, sizeof(double));

    double items[32] = {
        0.5,    // prepend 3
        1.5,
        2.5,
        3.5,
        4.5,
        5.5,
        6.5,
        7.5,
        8.5,    // prepend 1
        9.5,
        10.5,
        11.5,
        12.5,
        13.5,
        14.5,
        15.5,
        16.5,   // append 0
        17.5,
        18.5,
        19.5,
        20.5,
        21.5,
        22.5,
        23.5,
        24.5,   // append 2
        25.5,
        26.5,
        27.5,
        28.5,
        29.5,
        30.5,
        31.5,
    };

    Zinc_dbl_vector_append(&dv, &items[16], 8);
    Zinc_dbl_vector_prepend(&dv, &items[8], 8);
    Zinc_dbl_vector_append(&dv, &items[24], 8);
    Zinc_dbl_vector_prepend(&dv, &items[0], 8);

    double* p = NULL;
    size_t size = 0;
    p = Zinc_dbl_vector_slice(&dv, &size);
    Zinc_expect_double_equal(test, p[0], 0.5, "0");
    Zinc_expect_double_equal(test, p[1], 1.5, "1");
    Zinc_expect_double_equal(test, p[2], 2.5, "2");
    Zinc_expect_double_equal(test, p[3], 3.5, "3");
    Zinc_expect_double_equal(test, p[4], 4.5, "4");
    Zinc_expect_double_equal(test, p[5], 5.5, "5");
    Zinc_expect_double_equal(test, p[6], 6.5, "6");
    Zinc_expect_double_equal(test, p[7], 7.5, "7");
    Zinc_expect_double_equal(test, p[8], 8.5, "8");
    Zinc_expect_double_equal(test, p[9], 9.5, "9");
    Zinc_expect_double_equal(test, p[10], 10.5, "10");
    Zinc_expect_double_equal(test, p[11], 11.5, "11");
    Zinc_expect_double_equal(test, p[12], 12.5, "12");
    Zinc_expect_double_equal(test, p[13], 13.5, "13");
    Zinc_expect_double_equal(test, p[14], 14.5, "14");
    Zinc_expect_double_equal(test, p[15], 15.5, "15");
    Zinc_expect_double_equal(test, p[16], 16.5, "16");
    Zinc_expect_double_equal(test, p[17], 17.5, "17");
    Zinc_expect_double_equal(test, p[18], 18.5, "18");
    Zinc_expect_double_equal(test, p[19], 19.5, "19");
    Zinc_expect_double_equal(test, p[20], 20.5, "20");
    Zinc_expect_double_equal(test, p[21], 21.5, "21");
    Zinc_expect_double_equal(test, p[22], 22.5, "22");
    Zinc_expect_double_equal(test, p[23], 23.5, "23");
    Zinc_expect_double_equal(test, p[24], 24.5, "24");
    Zinc_expect_double_equal(test, p[25], 25.5, "25");
    Zinc_expect_double_equal(test, p[26], 26.5, "26");
    Zinc_expect_double_equal(test, p[27], 27.5, "27");
    Zinc_expect_double_equal(test, p[28], 28.5, "28");
    Zinc_expect_double_equal(test, p[29], 29.5, "29");
    Zinc_expect_double_equal(test, p[30], 30.5, "30");
    Zinc_expect_double_equal(test, p[31], 31.5, "31");

    Zinc_dbl_vector_destroy(&dv);
}

void Zinc_unit_dbl_vector(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_dbl_vector_expand_right);
        Zinc_test_register(test, Zinc_unit_dbl_vector_append);
        Zinc_test_register(test, Zinc_unit_dbl_vector_prepend);
        Zinc_test_register(test, Zinc_unit_dbl_vector_append_and_prepend);

        return;
    }

    Zinc_test_perform(test);
}
