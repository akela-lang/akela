#ifndef OS_UNIX_H
#define OS_UNIX_H

#ifdef __unix__
#include "api.h"
#include <stdbool.h>
#include "buffer_list.h"

ZINC_API enum result get_temp_file(FILE** fp_out, struct buffer* name);
ZINC_API enum result close_temp_file(FILE* fp);
ZINC_API enum result delete_temp_file(struct buffer* name);
ZINC_API enum result get_user_home_directory(struct buffer* dir);
ZINC_API void path_join(struct buffer* src1, struct buffer* src2, struct buffer* dest);
ZINC_API enum result get_user_app_directory(struct buffer* lower_name, struct buffer* dir);
ZINC_API enum result make_directory(struct buffer* dir);
ZINC_API enum result delete_directory(struct buffer* dir);
ZINC_API bool file_exists(struct buffer* filename);
ZINC_API enum result get_dir_files(struct buffer* dir, struct buffer_list* bl);
ZINC_API enum result get_exe_path(char** path);
ZINC_API void split_path(struct buffer* path, struct buffer* dir, struct buffer* filename);
ZINC_API int fopen_s(FILE **f, const char *name, const char *mode);
ZINC_API void Zinc_get_cwd(struct buffer* cwd);

#endif

#endif