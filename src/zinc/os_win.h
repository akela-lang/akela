#ifndef ZINC_OS_WIN_H
#define ZINC_OS_WIN_H

#include "api.h"
#include "result.h"
#include "zstring.h"

#if defined(_WIN32) || defined(WIN32)

ZINC_API enum result win_temp_filename(char** buff);
ZINC_API void set_console_utf8();
ZINC_API char* get_exe_path();
ZINC_API void split_path(struct buffer* path, struct buffer* dir, struct buffer* filename);
ZINC_API void path_join(struct buffer* path, struct buffer* filename);

#endif


#endif
