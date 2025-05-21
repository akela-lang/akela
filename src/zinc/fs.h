#ifndef ZINC_FS_H
#define ZINC_FS_H

#include "string_list.h"

void Zinc_path_join(struct Zinc_string* src1, struct Zinc_string* src2, struct Zinc_string* dest);
bool Zinc_file_exists(const char* filename);
Zinc_result Zinc_is_directory(const char* path, bool* is_dir);
int Zinc_fopen_s(FILE **f, const char *name, const char *mode);
void Zinc_path_append(Zinc_string* path, Zinc_string* path2);
void Zinc_path_append_str(Zinc_string* path, const char* path2);
void Zinc_get_cwd(struct Zinc_string* cwd);
enum Zinc_result Zinc_get_temp_file(FILE** fp_out, struct Zinc_string* name);
enum Zinc_result Zinc_close_temp_file(FILE* fp);
enum Zinc_result Zinc_delete_temp_file(struct Zinc_string* name);
enum Zinc_result Zinc_get_exe_path(char** path);
void Zinc_split_path(struct Zinc_string* path, struct Zinc_string* dir, struct Zinc_string* filename);

#if IS_WIN
wchar_t* Zinc_char_to_wchar(const char* str);
char* Zinc_wchar_to_char(wchar_t* wstr);
void Zinc_list_files2(const wchar_t* directory, Zinc_string_list* files);
#endif

void Zinc_list_files(const char* directory, Zinc_string_list* files);
Zinc_result Zinc_is_reg_file(Zinc_string* path);

#endif