#ifndef ZINC_DATETIME_H
#define ZINC_DATETIME_H

#include "api.h"
#include <time.h>
#include "zstring.h"

ZINC_API void Zinc_datetime_format(struct tm* tm, struct Zinc_string* dt);
ZINC_API void Zinc_datetime_get_current_t(time_t* t);
ZINC_API time_t Zinc_datetime_current();
ZINC_API void Zinc_datetime_get_utc_tm(time_t* t, struct tm* tm);
ZINC_API void Zinc_datetime_get_local_tm(time_t* t, struct tm* tm);
ZINC_API time_t Zinc_datetime_to_tm(struct Zinc_string* dto, struct tm* tm);
ZINC_API void Zinc_datetime_print_tm(struct tm* tm);
ZINC_API time_t Zinc_ny_minus_year(time_t t);
ZINC_API time_t Zinc_tm_to_gmt_t(struct tm* tm);
ZINC_API void Zinc_datetime_triple(
    time_t t,
    struct tm* local_tm,
    struct tm* ny_tm,
    struct tm* gmt_tm,
    Zinc_string* local_dt,
    Zinc_string* ny_dt,
    Zinc_string* gmt_dt);
time_t Zinc_last_ny_eod(time_t t);
time_t Zinc_last_ny_sod(time_t t);
char* Zinc_datetime_to_ny();
void Zinc_datetime_from_ny(char* old_tz);

#endif