#ifndef ZINC_OS_WIN_H
#define ZINC_OS_WIN_H

#include "api.h"
#include "result.h"
#include "zstring.h"

#if defined(_WIN32) || defined(WIN32)

ZINC_API enum result Zinc_win_temp_filename(char** buff);
ZINC_API void Zinc_set_console_utf8();
ZINC_API char* Zinc_get_exe_path();
ZINC_API void Zinc_split_path(Zinc_string* path, Zinc_string* dir, Zinc_string* filename);
ZINC_API void Zinc_path_join(Zinc_string* path, Zinc_string* filename);

#endif


#endif
