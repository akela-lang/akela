#ifndef _OS_WIN_H
#define _OS_WIN_H

#include "alba_api.h"
#include "zinc/result.h"
#include "zinc/buffer.h"

#if defined(_WIN32) || defined(WIN32)

/* dynamic-output buff */
ALBA_API enum result win_temp_filename(char** buff);

/* dynamic-output-none */
ALBA_API void set_console_utf8();

ALBA_API char* get_exe_path();

ALBA_API void split_path(struct buffer* path, struct buffer* dir, struct buffer* filename);

ALBA_API void path_join(struct buffer* path, struct buffer* filename);

#endif


#endif
