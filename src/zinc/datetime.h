#ifndef ZINC_DATETIME_H
#define ZINC_DATETIME_H

#include "api.h"
#include <time.h>
#include "zstring.h"

ZINC_API void Zinc_datetime_format(struct tm* tm, struct Zinc_string* dt);
ZINC_API void Zinc_datetime_get_current_t(time_t* t);
ZINC_API void Zinc_datetime_get_utc_tm(time_t* t, struct tm* tm);
ZINC_API void Zinc_datetime_get_local_tm(time_t* t, struct tm* tm);
ZINC_API time_t Zinc_datetime_to_tm(struct Zinc_string* dto, struct tm* tm);
ZINC_API void Zinc_datetime_print_tm(struct tm* tm);

#endif