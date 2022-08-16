#ifndef _OS_WIN_H
#define _OS_WIN_H

#include "alba_api.h"

#if defined(_WIN32) || defined(WIN32)

/* dynamic-output buff */
ALBA_API enum result win_temp_filename(char** buff);

/* dynamic-output-none */
ALBA_API void set_console_utf8();

#endif


#endif
