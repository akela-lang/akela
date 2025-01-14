#include "zinc/datetime.h"
#include "zinc/unit_test.h"
#include <time.h>
#include "zinc/piece.h"

void test_datetime_utc()
{
    test_name(__func__);

    struct Zinc_string dt;
    Zinc_string_init(&dt);
    time_t t;
    t = 0;
    struct tm tm;
    datetime_get_utc_tm(&t, &tm);
    datetime_format(&tm, &dt);
    expect_str(&dt, "1970-01-01T00:00:00Z", "1970-01-01T00:00:00Z");
    Zinc_string_destroy(&dt);
}

void test_datetime_utc2()
{
    test_name(__func__);

    struct Zinc_string dt;
    Zinc_string_init(&dt);
    time_t t;
    t = 25200;
    struct tm tm;
    datetime_get_utc_tm(&t, &tm);
    tm.tm_hour = 0;
    tm.tm_gmtoff = -25200;
    datetime_format(&tm, &dt);
    expect_str(&dt, "1970-01-01T00:00:00-07:00", "1970-01-01T00:00:00-0700");
    Zinc_string_destroy(&dt);
}

void test_datetime_utc3()
{
    test_name(__func__);
    time_t t;
    datetime_get_current_t(&t);
    struct tm tm;
    datetime_get_utc_tm(&t, &tm);
    struct Zinc_string dt;
    Zinc_string_init(&dt);
    datetime_format(&tm, &dt);
    struct tm tm2;
    time_t t2 = datetime_to_tm(&dt, &tm2);
    expect_true(t == t2, "t == t2");
    Zinc_string_destroy(&dt);
}

void test_datetime_local()
{
    test_name(__func__);
    time_t t;
    datetime_get_current_t(&t);
    struct tm tm;
    datetime_get_local_tm(&t, &tm);
    struct Zinc_string dt;
    Zinc_string_init(&dt);
    datetime_format(&tm, &dt);
    struct tm tm2;
    time_t t2 = datetime_to_tm(&dt, &tm2);
    struct Zinc_string dt2;
    Zinc_string_init(&dt2);
    datetime_format(&tm2, &dt2);
    expect_long_equal(t, t2, "t == t2");
    Zinc_string_destroy(&dt);
    Zinc_string_destroy(&dt2);
}

void test_datetime()
{
    test_datetime_utc();
    test_datetime_utc2();
    test_datetime_utc3();
    test_datetime_local();
}