#ifndef _OS_WIN_H
#define _OS_WIN_H

#include "akela_api.h"
#include "zinc/result.h"
#include "zinc/buffer.h"

#if defined(_WIN32) || defined(WIN32)

/* dynamic-output buff */
AKELA_API enum result win_temp_filename(char** buff);

/* dynamic-output-none */
AKELA_API void set_console_utf8();

AKELA_API char* get_exe_path();

AKELA_API void split_path(struct buffer* path, struct buffer* dir, struct buffer* filename);

AKELA_API void path_join(struct buffer* path, struct buffer* filename);

#endif


#endif
