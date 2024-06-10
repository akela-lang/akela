#include <stdio.h>
#include <time.h>
#include "buffer.h"
#include <string.h>
#include <stdbool.h>
#include "piece.h"

void get_offset(struct tm* tm, char offset[7])
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

void datetime_format(struct tm* tm, struct buffer* dt)
{
    char s[100];
    char offset[7];

    get_offset(tm, offset);

    snprintf(s, 100, "%d-%02d-%02dT%02d:%02d:%02d%s",
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec, offset);
    buffer_copy_str(dt, s);
}

void datetime_get_current_t(time_t* t)
{
    time(t);
}

void datetime_get_utc_tm(time_t* t, struct tm* tm)
{
    gmtime_r(t, tm);
}
void datetime_get_local_tm(time_t* t, struct tm* tm)
{
    localtime_r(t, tm);
}

time_t datetime_to_tm(struct buffer* dt, struct tm* tm)
{
    time_t t0;
    struct tm tm0;
    datetime_get_current_t(&t0);
    datetime_get_local_tm(&t0, &tm0);
    int isdst = tm0.tm_isdst;

    struct buffer date;
    buffer_init(&date);
    struct buffer time;
    buffer_init(&time);
    get_piece(dt, 'T', 0, &date);
    get_piece(dt, 'T', 1, &time);

    struct buffer year;
    buffer_init(&year);
    struct buffer month;
    buffer_init(&month);
    struct buffer day;
    buffer_init(&day);
    get_piece(&date, '-', 0, &year);
    get_piece(&date, '-', 1, &month);
    get_piece(&date, '-', 2, &day);

    struct buffer time_display;
    buffer_init(&time_display);
    struct buffer time_offset;
    buffer_init(&time_offset);

    bool is_utc = false;
    bool is_negative_offset = false;
    struct buffer offset_hours;
    buffer_init(&offset_hours);
    struct buffer offset_minutes;
    buffer_init(&offset_minutes);

    buffer_finish(&time);
    if (strchr(time.buf, 'Z')) {
        is_utc = true;
        get_piece(&time, 'Z', 0, &time_display);
    } else if (strchr(time.buf, '+')) {
        get_piece(&time, '+', 0, &time_display);
        get_piece(&time, '+', 1, &time_offset);
        get_piece(&time_offset, ':', 0, &offset_hours);
        get_piece(&time_offset, ':', 1, &offset_minutes);
    } else if (strchr(time.buf, '-')) {
        is_negative_offset = true;
        get_piece(&time, '-', 0, &time_display);;
        get_piece(&time, '-', 1, &time_offset);
        get_piece(&time_offset, ':', 0, &offset_hours);
        get_piece(&time_offset, ':', 1, &offset_minutes);
    }

    struct buffer hour;
    buffer_init(&hour);
    struct buffer minute;
    buffer_init(&minute);
    struct buffer second;
    buffer_init(&second);
    get_piece(&time_display, ':', 0, &hour);
    get_piece(&time_display, ':', 1, &minute);
    get_piece(&time_display, ':', 2, &second);

    buffer_finish(&year);
    tm->tm_year = (int)strtol(year.buf, NULL, 10) - 1900;

    buffer_finish(&month);
    tm->tm_mon = (int)strtol(month.buf, NULL, 10) - 1;

    buffer_finish(&day);
    tm->tm_mday = (int)strtol(day.buf, NULL, 10);

    tm->tm_wday = 0;
    tm->tm_yday = 0;

    buffer_finish(&hour);
    tm->tm_hour = (int)strtol(hour.buf, NULL, 10);

    buffer_finish(&minute);
    tm->tm_min = (int)strtol(minute.buf, NULL, 10);

    buffer_finish(&second);
    tm->tm_sec = (int)strtol(second.buf, NULL, 10);

    time_t t;
    if (is_utc) {
        tm->tm_gmtoff = 0;
        tm->tm_isdst = 0;
        tm->tm_zone = NULL;
        t = timegm(tm);
    } else {
        buffer_finish(&offset_hours);
        long offset_hours_l = strtol(offset_hours.buf, NULL, 10);

        buffer_finish(&offset_minutes);
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

    buffer_destroy(&date);
    buffer_destroy(&time);
    buffer_destroy(&year);
    buffer_destroy(&month);
    buffer_destroy(&day);
    buffer_destroy(&time_display);
    buffer_destroy(&time_offset);
    buffer_destroy(&hour);
    buffer_destroy(&minute);
    buffer_destroy(&second);
    buffer_destroy(&offset_hours);
    buffer_destroy(&offset_minutes);
    return t;
}

void datetime_print_tm(struct tm* tm)
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