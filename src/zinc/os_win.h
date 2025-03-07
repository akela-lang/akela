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

wchar_t* Zinc_char_to_wchar(const char* str);
char* Zinc_wchar_to_char(wchar_t* wstr);
void Zinc_list_files(const char* directory, Zinc_string_list* files);
void Zinc_list_files2(const wchar_t* directory, Zinc_string_list* files);

#endif


#endif
