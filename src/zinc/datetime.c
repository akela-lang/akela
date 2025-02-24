#include <stdio.h>
#include <time.h>
#include "zstring.h"
#include <string.h>
#include <stdbool.h>
#include "piece.h"

void Zinc_get_offset(struct tm* tm, char offset[7])
{
    if (tm->tm_gmtoff == 0) {
        strncpy(offset, "Z", 7);
    } else {
        bool is_negative = false;
        long tm_gmtoff = tm->tm_gmtoff;
        if (tm_gmtoff < 0) {
            is_negative = true;
            tm_gmtoff *= -1;
        }
        long hours = tm_gmtoff / 3600;
        long minutes = tm_gmtoff % 3600;
        sprintf(offset, "%c%02ld:%02ld", is_negative ? '-': '+', hours, minutes);
    }
}

void Zinc_datetime_format(struct tm* tm, struct Zinc_string* dt)
{
    char s[100];
    char offset[7];

    Zinc_get_offset(tm, offset);

    snprintf(s, 100, "%d-%02d-%02dT%02d:%02d:%02d%s",
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec, offset);
    Zinc_string_add_str(dt, s);
}

void Zinc_datetime_get_current_t(time_t* t)
{
    time(t);
}

void Zinc_datetime_get_utc_tm(time_t* t, struct tm* tm)
{
    gmtime_r(t, tm);
}
void Zinc_datetime_get_local_tm(time_t* t, struct tm* tm)
{
    localtime_r(t, tm);
}

time_t Zinc_datetime_to_tm(struct Zinc_string* dt, struct tm* tm)
{
    time_t t0;
    struct tm tm0;
    Zinc_datetime_get_current_t(&t0);
    Zinc_datetime_get_local_tm(&t0, &tm0);
    int isdst = tm0.tm_isdst;

    struct Zinc_string date;
    Zinc_string_init(&date);
    struct Zinc_string time;
    Zinc_string_init(&time);
    Zinc_get_piece(dt, 'T', 0, &date);
    Zinc_get_piece(dt, 'T', 1, &time);

    struct Zinc_string year;
    Zinc_string_init(&year);
    struct Zinc_string month;
    Zinc_string_init(&month);
    struct Zinc_string day;
    Zinc_string_init(&day);
    Zinc_get_piece(&date, '-', 0, &year);
    Zinc_get_piece(&date, '-', 1, &month);
    Zinc_get_piece(&date, '-', 2, &day);

    struct Zinc_string time_display;
    Zinc_string_init(&time_display);
    struct Zinc_string time_offset;
    Zinc_string_init(&time_offset);

    bool is_utc = false;
    bool is_negative_offset = false;
    struct Zinc_string offset_hours;
    Zinc_string_init(&offset_hours);
    struct Zinc_string offset_minutes;
    Zinc_string_init(&offset_minutes);

    Zinc_string_finish(&time);
    if (strchr(time.buf, 'Z')) {
        is_utc = true;
        Zinc_get_piece(&time, 'Z', 0, &time_display);
    } else if (strchr(time.buf, '+')) {
        Zinc_get_piece(&time, '+', 0, &time_display);
        Zinc_get_piece(&time, '+', 1, &time_offset);
        Zinc_get_piece(&time_offset, ':', 0, &offset_hours);
        Zinc_get_piece(&time_offset, ':', 1, &offset_minutes);
    } else if (strchr(time.buf, '-')) {
        is_negative_offset = true;
        Zinc_get_piece(&time, '-', 0, &time_display);;
        Zinc_get_piece(&time, '-', 1, &time_offset);
        Zinc_get_piece(&time_offset, ':', 0, &offset_hours);
        Zinc_get_piece(&time_offset, ':', 1, &offset_minutes);
    }

    struct Zinc_string hour;
    Zinc_string_init(&hour);
    struct Zinc_string minute;
    Zinc_string_init(&minute);
    struct Zinc_string second;
    Zinc_string_init(&second);
    Zinc_get_piece(&time_display, ':', 0, &hour);
    Zinc_get_piece(&time_display, ':', 1, &minute);
    Zinc_get_piece(&time_display, ':', 2, &second);

    Zinc_string_finish(&year);
    tm->tm_year = (int)strtol(year.buf, NULL, 10) - 1900;

    Zinc_string_finish(&month);
    tm->tm_mon = (int)strtol(month.buf, NULL, 10) - 1;

    Zinc_string_finish(&day);
    tm->tm_mday = (int)strtol(day.buf, NULL, 10);

    tm->tm_wday = 0;
    tm->tm_yday = 0;

    Zinc_string_finish(&hour);
    tm->tm_hour = (int)strtol(hour.buf, NULL, 10);

    Zinc_string_finish(&minute);
    tm->tm_min = (int)strtol(minute.buf, NULL, 10);

    Zinc_string_finish(&second);
    tm->tm_sec = (int)strtol(second.buf, NULL, 10);

    time_t t;
    if (is_utc) {
        tm->tm_gmtoff = 0;
        tm->tm_isdst = 0;
        tm->tm_zone = NULL;
        t = timegm(tm);
    } else {
        Zinc_string_finish(&offset_hours);
        long offset_hours_l = strtol(offset_hours.buf, NULL, 10);

        Zinc_string_finish(&offset_minutes);
        long offset_minutes_l = strtol(offset_minutes.buf, NULL, 10);

        long tm_gmtoff = 0;
        tm_gmtoff += offset_hours_l * 3600;
        tm_gmtoff += offset_minutes_l * 60;
        if (is_negative_offset) {
            tm_gmtoff = tm_gmtoff * -1;
        }
        tm->tm_gmtoff = tm_gmtoff;
        tm->tm_isdst = isdst;
        tm->tm_zone = NULL;
        t = mktime(tm);
    }

    Zinc_string_destroy(&date);
    Zinc_string_destroy(&time);
    Zinc_string_destroy(&year);
    Zinc_string_destroy(&month);
    Zinc_string_destroy(&day);
    Zinc_string_destroy(&time_display);
    Zinc_string_destroy(&time_offset);
    Zinc_string_destroy(&hour);
    Zinc_string_destroy(&minute);
    Zinc_string_destroy(&second);
    Zinc_string_destroy(&offset_hours);
    Zinc_string_destroy(&offset_minutes);
    return t;
}

void Zinc_datetime_print_tm(struct tm* tm)
{
    printf("year: %d\n", tm->tm_year);
    printf("month: %d\n", tm->tm_mon);
    printf("day: %d\n", tm->tm_mday);
    printf("hour: %d\n", tm->tm_hour);
    printf("min: %d\n", tm->tm_min);
    printf("sec: %d\n", tm->tm_sec);
    printf("offset: %ld\n", tm->tm_gmtoff);
    printf("is dst: %d\n", tm->tm_isdst);
    printf("zone: %s\n", tm->tm_zone);
    printf("w day: %d\n", tm->tm_wday);
    printf("y day: %d\n", tm->tm_yday);
}

void Zinc_t_to_ny_tm(time_t t, struct tm* ny_tm)
{
    char* current_tz = getenv("TZ");
    setenv("TZ", "America/New_York", 1);
    tzset();
    localtime_r(&t, ny_tm);
    if (current_tz) {
        setenv("TZ", current_tz, 1);
    }
}

void Zinc_tm_local_minus_year(struct tm* tm)
{
    tm->tm_year--;
    mktime(tm);
}