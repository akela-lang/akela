#ifndef OS_UNIX_H
#define OS_UNIX_H

#ifdef __unix__
#include "api.h"
#include <stdbool.h>
#include "string_list.h"

ZINC_API enum Zinc_result Zinc_get_temp_file(FILE** fp_out, struct Zinc_string* name);
ZINC_API enum Zinc_result Zinc_close_temp_file(FILE* fp);
ZINC_API enum Zinc_result Zinc_delete_temp_file(struct Zinc_string* name);
ZINC_API enum Zinc_result Zinc_get_user_home_directory(struct Zinc_string* dir);
ZINC_API void Zinc_path_join(struct Zinc_string* src1, struct Zinc_string* src2, struct Zinc_string* dest);
ZINC_API enum Zinc_result Zinc_get_user_app_directory(struct Zinc_string* lower_name, struct Zinc_string* dir);
ZINC_API enum Zinc_result Zinc_make_directory(struct Zinc_string* dir);
ZINC_API enum Zinc_result Zinc_delete_directory(struct Zinc_string* dir);
ZINC_API bool Zinc_file_exists(struct Zinc_string* filename);
ZINC_API enum Zinc_result Zinc_get_dir_files(struct Zinc_string* dir, Zinc_string_list* bl);
ZINC_API enum Zinc_result Zinc_get_exe_path(char** path);
ZINC_API void Zinc_split_path(struct Zinc_string* path, struct Zinc_string* dir, struct Zinc_string* filename);
ZINC_API int Zinc_fopen_s(FILE **f, const char *name, const char *mode);
ZINC_API void Zinc_get_cwd(struct Zinc_string* cwd);

#endif

#endif