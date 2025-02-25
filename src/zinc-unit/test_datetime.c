#include "zinc/datetime.h"
#include "zinc/unit_test.h"
#include <time.h>
#include "zinc/piece.h"

void test_datetime_utc()
{
    Zinc_test_name(__func__);

    struct Zinc_string dt;
    Zinc_string_init(&dt);
    time_t t;
    t = 0;
    struct tm tm;
    Zinc_datetime_get_utc_tm(&t, &tm);
    Zinc_datetime_format(&tm, &dt);
    Zinc_expect_string(&dt, "1970-01-01T00:00:00Z", "1970-01-01T00:00:00Z");
    Zinc_string_destroy(&dt);
}

void test_datetime_utc2()
{
    Zinc_test_name(__func__);

    struct Zinc_string dt;
    Zinc_string_init(&dt);
    time_t t;
    t = 25200;
    struct tm tm;
    Zinc_datetime_get_utc_tm(&t, &tm);
    tm.tm_hour = 0;
    tm.tm_gmtoff = -25200;
    Zinc_datetime_format(&tm, &dt);
    Zinc_expect_string(&dt, "1970-01-01T00:00:00-07:00", "1970-01-01T00:00:00-0700");
    Zinc_string_destroy(&dt);
}

void test_datetime_utc3()
{
    Zinc_test_name(__func__);
    time_t t;
    Zinc_datetime_get_current_t(&t);
    struct tm tm;
    Zinc_datetime_get_utc_tm(&t, &tm);
    struct Zinc_string dt;
    Zinc_string_init(&dt);
    Zinc_datetime_format(&tm, &dt);
    struct tm tm2;
    time_t t2 = Zinc_datetime_to_tm(&dt, &tm2);
    Zinc_expect_true(t == t2, "t == t2");
    Zinc_string_destroy(&dt);
}

void test_datetime_local()
{
    Zinc_test_name(__func__);
    time_t t;
    Zinc_datetime_get_current_t(&t);
    struct tm tm;
    Zinc_datetime_get_local_tm(&t, &tm);
    Zinc_string dt;
    Zinc_string_init(&dt);
    Zinc_datetime_format(&tm, &dt);
    struct tm tm2;
    time_t t2 = Zinc_datetime_to_tm(&dt, &tm2);
    Zinc_string dt2;
    Zinc_string_init(&dt2);
    Zinc_datetime_format(&tm2, &dt2);
    Zinc_expect_long_equal(t, t2, "t == t2");
    Zinc_string_destroy(&dt);
    Zinc_string_destroy(&dt2);
}

void test_datetime_current_ny()
{
    Zinc_test_name(__func__);

    struct tm local_tm;
    struct tm ny_tm;
    struct tm gmt_tm;
    Zinc_string local_dt;
    Zinc_string ny_dt;
    Zinc_string gmt_dt;

    time_t current = Zinc_datetime_current();
    time_t end = Zinc_last_ny_eod(current);
    time_t end_minus_year = Zinc_ny_minus_year(end);
    time_t start = Zinc_last_ny_sod(end_minus_year);

    Zinc_string_init(&local_dt);
    Zinc_string_init(&ny_dt);
    Zinc_string_init(&gmt_dt);

    Zinc_datetime_triple(start, &local_tm, &ny_tm, &gmt_tm, &local_dt, &ny_dt, &gmt_dt);
    // printf("start t: %ld\n", start);
    // printf("start local time: %s\n", Zinc_string_c_str(&local_dt));
    // printf("start ny time: %s\n", Zinc_string_c_str(&ny_dt));
    // printf("start gmt time: %s\n", Zinc_string_c_str(&gmt_dt));

    Zinc_datetime_triple(end, &local_tm, &ny_tm, &gmt_tm, &local_dt, &ny_dt, &gmt_dt);
    // printf("end t: %ld\n", end);
    // printf("end local time: %s\n", Zinc_string_c_str(&local_dt));
    // printf("end ny time: %s\n", Zinc_string_c_str(&ny_dt));
    // printf("end gmt time: %s\n", Zinc_string_c_str(&gmt_dt));

    Zinc_string_destroy(&local_dt);
    Zinc_string_destroy(&ny_dt);
    Zinc_string_destroy(&gmt_dt);

    Zinc_expect_true(true, "true");
}

void test_datetime()
{
    test_datetime_utc();
    test_datetime_utc2();
    test_datetime_utc3();
    test_datetime_local();
    test_datetime_current_ny();
}