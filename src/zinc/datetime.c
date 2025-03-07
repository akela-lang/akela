#include "os.h"
#include <stdio.h>
#include <time.h>
#include "zstring.h"
#include <string.h>
#include <stdbool.h>
#include "piece.h"
#include <stdlib.h>
#include "datetime.h"
#include <stdint.h>

#if IS_WIN
#include <windows.h>
#endif

time_t Zinc_datetime_get_local_t(struct tm* tm);
time_t Zinc_datetime_get_gmt_t(struct tm* tm);

long get_gmtoff(time_t t, struct tm* tm, Zinc_time_type type)
{
#if IS_WIN
    if (type == Zinc_time_type_local) {
        TIME_ZONE_INFORMATION tz;
        DWORD result = GetTimeZoneInformation(&tz);

        // Convert the base UTC offset from minutes to seconds
        long base_offset = -tz.Bias * 60;

        // Check if daylight saving time is in effect for this specific time
        struct tm local_tm;
        localtime_s(&local_tm, &t);
        if (local_tm.tm_isdst > 0) {
            base_offset -= tz.DaylightBias * 60;
        }
        else {
            base_offset -= tz.StandardBias * 60;
        }

        return base_offset;
    }
    else if (type == Zinc_time_type_ny) {
        time_t t2 = Zinc_datetime_get_gmt_t(tm);
        int64_t delta = t2 - t;
        return delta;
    }
    else {
        return 0;
    }
#elif IS_UNIX
    return tm->tm_gmtoff;
#else
    #error "unsupported OS"
#endif
}

void Zinc_get_offset(time_t t, struct tm* tm, char offset[7], Zinc_time_type type)
{
    long tm_gmtoff = get_gmtoff(t, tm, type);
    if (tm_gmtoff == 0) {
        strncpy(offset, "Z", 7);
    } else {
        bool is_negative = false;
        if (tm_gmtoff < 0) {
            is_negative = true;
            tm_gmtoff *= -1;
        }
        long hours = tm_gmtoff / 3600;
        long minutes = tm_gmtoff % 3600;
        sprintf(offset, "%c%02ld:%02ld", is_negative ? '-': '+', hours, minutes);
    }
}

void Zinc_datetime_format(time_t t, Zinc_string* dt, Zinc_time_type type)
{
    bool is_ny = false;
    char tz = NULL;
    if (type == Zinc_time_type_ny) {
        tz = Zinc_datetime_to_ny();
        is_ny = true;
    }

    struct tm tm;

    if (type == Zinc_time_type_local || type == Zinc_time_type_ny) {
        Zinc_datetime_get_local_tm(&t, &tm);
    }

    if (type == Zinc_time_type_gmt) {
        Zinc_datetime_get_gmt_tm(&t, &tm);
    }

    char s[100];
    char offset[7];
    Zinc_get_offset(t, &tm, offset, type);

    snprintf(s, 100, "%d-%02d-%02dT%02d:%02d:%02d%s",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec, offset);
    Zinc_string_add_str(dt, s);

    if (is_ny) {
        Zinc_datetime_from_ny(tz);
    }
}

time_t Zinc_datetime_current()
{
    return time(NULL);
}

void Zinc_datetime_get_gmt_tm(time_t* t, struct tm* tm)
{
#if IS_UNIX
    gmtime_r(t, tm);
#elif IS_WIN
    gmtime_s(tm, t);
#else
    #error "unsupported OS"
#endif
}

void Zinc_datetime_get_local_tm(time_t* t, struct tm* tm)
{
#if IS_UNIX
    localtime_r(t, tm);
#elif IS_WIN
    localtime_s(tm, t);
#else
    #error "unsupported OS"
#endif
}

time_t Zinc_datetime_get_local_t(struct tm* tm)
{
    return mktime(tm);
}

time_t Zinc_datetime_get_gmt_t(struct tm* tm)
{
#if IS_UNIX
    return timegm(tm);
#elif IS_WIN
    return _mkgmtime(tm);
#else
#error "unsupported OS"
#endif
}

time_t Zinc_datetime_to_tm(struct Zinc_string* dt, struct tm* tm)
{
    time_t t0;
    struct tm tm0;
    t0 = Zinc_datetime_current();
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
#if IS_UNIX
        tm->tm_gmtoff = 0;
        tm->tm_zone = NULL;
        tm->tm_isdst = 0;
        t = timegm(tm);
#elif IS_WIN
        tm->tm_isdst = 0;
        t = _mkgmtime(tm);
#else
    #error "unsupported OS"
#endif
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
#if IS_UNIX
        tm->tm_gmtoff = tm_gmtoff;
        tm->tm_zone = NULL;
#elif IS_WIN
#else
        #error "unsupported OS"
#endif
        tm->tm_isdst = isdst;
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
#if IS_UNIX
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
#elif IS_WIN
    printf("year: %d\n", tm->tm_year);
    printf("month: %d\n", tm->tm_mon);
    printf("day: %d\n", tm->tm_mday);
    printf("hour: %d\n", tm->tm_hour);
    printf("min: %d\n", tm->tm_min);
    printf("sec: %d\n", tm->tm_sec);
    printf("is dst: %d\n", tm->tm_isdst);
    printf("w day: %d\n", tm->tm_wday);
    printf("y day: %d\n", tm->tm_yday);
#else
    #error "unsupported OS"
#endif
}

char* Zinc_datetime_to_ny()
{
#if IS_UNIX
    char* current_tz = getenv("TZ");
    setenv("TZ", "America/New_York", 1);
    tzset();
    return current_tz;
#elif IS_WIN
    char* current_tz = getenv("TZ");
    _putenv("TZ=EST5EDT");
    _tzset();
    return current_tz;
#else
    #error "unsupported OS"
#endif
}

void Zinc_datetime_from_ny(char* old_tz)
{
#if IS_UNIX
    if (old_tz) {
        setenv("TZ", old_tz, 1);
    } else {
        unsetenv("TZ");
    }
    tzset();
#elif IS_WIN
    if (old_tz) {
        _putenv_s("TZ", old_tz);
    } else {
        putenv("TZ=");
    }
    _tzset();
#else
        #error "unsupported OS"
#endif
}

time_t Zinc_datetime_ny_minus_year(time_t t)
{
    char* tz = Zinc_datetime_to_ny();

    struct tm tm;
#if IS_UNIX
    localtime_r(&t, &tm);
#elif IS_WIN
	localtime_s(&tm, &t);
#else
    #error "unsupported OS"
#endif

    tm.tm_year--;

    t = mktime(&tm);

    Zinc_datetime_from_ny(tz);

    return t;
}

time_t Zinc_datetime_ny_minus_day(time_t t)
{
    char* tz = Zinc_datetime_to_ny();

    struct tm tm;
#if IS_UNIX
    localtime_r(&t, &tm);
#elif IS_WIN
    localtime_s(&tm, &t);
#else
    #error "unsupported OS"
#endif

    tm.tm_mday--;

    t = mktime(&tm);

    Zinc_datetime_from_ny(tz);

    return t;
}

time_t Zinc_tm_to_gmt_t(struct tm* tm)
{
#if IS_UNIX
    return timegm(tm);
#elif IS_WIN
	return _mkgmtime(tm);
#else
    #error "unsupported OS"
#endif
}

void Zinc_datetime_triple(
    time_t t,
    Zinc_string* local_dt,
    Zinc_string* ny_dt,
    Zinc_string* gmt_dt)
{
    Zinc_string_clear(local_dt);
    Zinc_string_clear(ny_dt);
    Zinc_string_clear(gmt_dt);
    Zinc_datetime_format(t, local_dt, Zinc_time_type_local);
    Zinc_datetime_format(t, ny_dt, Zinc_time_type_ny);
    Zinc_datetime_format(t, gmt_dt, Zinc_time_type_gmt);
}

time_t Zinc_datetime_last_ny_eod(time_t t)
{
    char* tz = Zinc_datetime_to_ny();

    struct tm tm;
#if IS_UNIX
    localtime_r(&t, &tm);
#elif IS_WIN
    localtime_s(&tm, &t);
#endif
    TIME_ZONE_INFORMATION tz2;
    DWORD result = GetTimeZoneInformation(&tz2);
    printf("\n");

    if (tm.tm_hour < 17) {
        tm.tm_mday--;
    }
    tm.tm_hour = 17;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    t = mktime(&tm);

    Zinc_datetime_from_ny(tz);

    return t;
}

time_t Zinc_last_ny_sod(time_t t)
{
    char* tz = Zinc_datetime_to_ny();

    struct tm tm;
#if IS_UNIX
    localtime_r(&t, &tm);
#elif IS_WIN
    localtime_s(&tm, &t);
#endif

    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    t = mktime(&tm);
    Zinc_datetime_from_ny(tz);

    return t;
}