#ifndef ZINC_OS_WIN_H
#define ZINC_OS_WIN_H

#include "api.h"
#include "result.h"
#include "zstring.h"
#include "string_list.h"

#if defined(_WIN32) || defined(WIN32)

ZINC_API enum Zinc_result Zinc_get_temp_file(FILE** fp, Zinc_string* name);
ZINC_API void Zinc_set_console_utf8();
ZINC_API void Zinc_split_path(Zinc_string* path, Zinc_string* dir, Zinc_string* filename);
Zinc_result Zinc_is_reg_file(Zinc_string* path);

#endif


#endif
