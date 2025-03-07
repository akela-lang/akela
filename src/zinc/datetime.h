#ifndef ZINC_DATETIME_H
#define ZINC_DATETIME_H

#include "api.h"
#include <time.h>
#include "zstring.h"

typedef enum Zinc_time_type {
	Zinc_time_type_local,
	Zinc_time_type_ny,
	Zinc_time_type_gmt
} Zinc_time_type;

void Zinc_datetime_format(time_t t, Zinc_string* dt, Zinc_time_type type);
ZINC_API time_t Zinc_datetime_current();
ZINC_API void Zinc_datetime_get_gmt_tm(time_t* t, struct tm* tm);
ZINC_API void Zinc_datetime_get_local_tm(time_t* t, struct tm* tm);
ZINC_API time_t Zinc_datetime_to_tm(struct Zinc_string* dto, struct tm* tm);
ZINC_API void Zinc_datetime_print_tm(struct tm* tm);
ZINC_API time_t Zinc_datetime_ny_minus_year(time_t t);
ZINC_API time_t Zinc_datetime_ny_minus_day(time_t t);
ZINC_API time_t Zinc_tm_to_gmt_t(struct tm* tm);
ZINC_API void Zinc_datetime_triple(
    time_t t,
    Zinc_string* local_dt,
    Zinc_string* ny_dt,
    Zinc_string* gmt_dt);
time_t Zinc_datetime_last_ny_eod(time_t t);
time_t Zinc_last_ny_sod(time_t t);
char* Zinc_datetime_to_ny();
void Zinc_datetime_from_ny(char* old_tz);

#endif