#ifndef DATETIME_H
#define DATETIME_H

#include "api.h"
#include <time.h>
#include "buffer.h"

ZINC_API void datetime_format(struct tm* tm, struct Zinc_string* dt);
ZINC_API void datetime_get_current_t(time_t* t);
ZINC_API void datetime_get_utc_tm(time_t* t, struct tm* tm);
ZINC_API void datetime_get_local_tm(time_t* t, struct tm* tm);
ZINC_API time_t datetime_to_tm(struct Zinc_string* dto, struct tm* tm);
ZINC_API void datetime_print_tm(struct tm* tm);

#endif