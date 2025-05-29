#include "zinc/os.h"
#include "zinc/datetime.h"
#include <time.h>
#include "zinc/piece.h"
#include "zinc/os_unix.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_datetime_utc(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string dt;
    Zinc_string_init(&dt);
    time_t t;
    t = 0;
    Zinc_datetime_format(t, &dt, Zinc_time_type_gmt);
    Zinc_test_expect_string(test, &dt, "1970-01-01T00:00:00Z", "gmt format");
    Zinc_string_destroy(&dt);
}

#if IS_UNIX
void Zinc_unit_datetime_utc2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string dt;
    Zinc_string_init(&dt);
    time_t t;
    t = 25200;
    struct tm tm;
    Zinc_datetime_get_gmt_tm(&t, &tm);
    tm.tm_hour = 0;
    tm.tm_gmtoff = -25200;
    t = mktime(&tm);
    Zinc_datetime_format(t, &dt, Zinc_time_type_local);
    Zinc_test_expect_string(test, &dt, "1970-01-01T00:00:00-07:00", "1970-01-01T00:00:00-0700");
    Zinc_string_destroy(&dt);
}
#endif

void Zinc_unit_datetime_utc3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    time_t t;
    t = Zinc_datetime_current();
    Zinc_string dt;
    Zinc_string_init(&dt);
    Zinc_datetime_format(t, &dt, Zinc_time_type_gmt);
    struct tm tm2;
    time_t t2 = Zinc_datetime_to_tm(&dt, &tm2);
    Zinc_test_expect_true(test, t == t2, "t == t2");
    Zinc_string_destroy(&dt);
}

void Zinc_unit_datetime_local(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    time_t t;
    t = Zinc_datetime_current();
    struct tm tm;
    Zinc_datetime_get_local_tm(&t, &tm);
    Zinc_string dt;
    Zinc_string_init(&dt);
    Zinc_datetime_format(t, &dt, Zinc_time_type_local);
    struct tm tm2;
    time_t t2 = Zinc_datetime_to_tm(&dt, &tm2);
    Zinc_string dt2;
    Zinc_string_init(&dt2);
    Zinc_datetime_format(t2, &dt2, Zinc_time_type_local);
    Zinc_test_expect_long_equal(test, t, t2, "t == t2");
    Zinc_string_destroy(&dt);
    Zinc_string_destroy(&dt2);
}

void Zinc_unit_datetime_ny_standard(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    time_t t = 1735689600;

    Zinc_string local_dt;
    Zinc_string ny_dt;
    Zinc_string gmt_dt;
    Zinc_string_init(&local_dt);
    Zinc_string_init(&ny_dt);
    Zinc_string_init(&gmt_dt);

    Zinc_datetime_triple(t, &local_dt, &ny_dt, &gmt_dt);
    //Zinc_test_expect_string(&local_dt, "2024-12-31T17:00:00-07:00", "local_dt");
    Zinc_test_expect_string(test, &ny_dt, "2024-12-31T19:00:00-05:00", "ny_dt");
    Zinc_test_expect_string(test, &gmt_dt, "2025-01-01T00:00:00Z", "gmt_dt");
    Zinc_string_destroy(&local_dt);
    Zinc_string_destroy(&ny_dt);
    Zinc_string_destroy(&gmt_dt);
}

void Zinc_unit_datetime_ny_dst(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    time_t t = 1719792000;

    Zinc_string local_dt;
    Zinc_string ny_dt;
    Zinc_string gmt_dt;
    Zinc_string_init(&local_dt);
    Zinc_string_init(&ny_dt);
    Zinc_string_init(&gmt_dt);

    Zinc_datetime_triple(t, &local_dt, &ny_dt, &gmt_dt);
    //Zinc_expect_string(&local_dt, "2024-06-30T18:00:00-06:00", "local_dt");
    Zinc_test_expect_string(test, &ny_dt, "2024-06-30T20:00:00-04:00", "ny_dt");
    Zinc_test_expect_string(test, &gmt_dt, "2024-07-01T00:00:00Z", "gmt_dt");

    Zinc_string_destroy(&local_dt);
    Zinc_string_destroy(&ny_dt);
    Zinc_string_destroy(&gmt_dt);
}

void Zinc_unit_datetime_eod_before_eod(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string dt;
    time_t t = 1739646000;
    Zinc_string_init(&dt);

    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2025-02-15T14:00:00-05:00", "dt");

    t = Zinc_datetime_last_ny_eod(t);

    Zinc_string_clear(&dt);
    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2025-02-14T17:00:00-05:00", "dt");

    Zinc_string_destroy(&dt);
}

void Zinc_unit_datetime_eod_after_eod(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string dt;
    time_t t = 1739667600;
    Zinc_string_init(&dt);

    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2025-02-15T20:00:00-05:00", "dt");

    t = Zinc_datetime_last_ny_eod(t);

    Zinc_string_clear(&dt);
    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2025-02-15T17:00:00-05:00", "dt");

    Zinc_string_destroy(&dt);
}

void Zinc_unit_test_datetime_ny_minus_year(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string dt;

    time_t t = 1742083200;

    Zinc_string_init(&dt);

    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2025-03-15T20:00:00-04:00", "dt");

    t = Zinc_datetime_ny_minus_year(t);

    Zinc_string_clear(&dt);
    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2024-03-15T20:00:00-04:00", "dt");

    Zinc_string_destroy(&dt);
}

void Zinc_unit_datetime_ny_minus_day(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string dt;

    time_t t = 1742083200;

    Zinc_string_init(&dt);

    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2025-03-15T20:00:00-04:00", "dt");

    t = Zinc_datetime_ny_minus_day(t);

    Zinc_string_clear(&dt);
    Zinc_datetime_format(t, &dt, Zinc_time_type_ny);
    Zinc_test_expect_string(test, &dt, "2025-03-14T20:00:00-04:00", "dt");

    Zinc_string_destroy(&dt);
}

void Zinc_unit_datetime(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_datetime_utc);
#if IS_UNIX
        Zinc_test_register(test, Zinc_unit_datetime_utc2);
#endif
        Zinc_test_register(test, Zinc_unit_datetime_utc3);
        Zinc_test_register(test, Zinc_unit_datetime_local);
        Zinc_test_register(test, Zinc_unit_datetime_ny_standard);
        Zinc_test_register(test, Zinc_unit_datetime_ny_dst);
        Zinc_test_register(test, Zinc_unit_datetime_eod_before_eod);
        Zinc_test_register(test, Zinc_unit_datetime_eod_after_eod);
        Zinc_test_register(test, Zinc_unit_test_datetime_ny_minus_year);
        Zinc_test_register(test, Zinc_unit_datetime_ny_minus_day);

        return;
    }

    Zinc_test_perform(test);
}