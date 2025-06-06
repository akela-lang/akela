#ifndef ZINC_OS_UNIX_H
#define ZINC_OS_UNIX_H

#ifdef __unix__
#include "api.h"
#include <stdbool.h>
#include "string_list.h"

ZINC_API enum Zinc_result Zinc_get_user_home_directory(struct Zinc_string* dir);
ZINC_API enum Zinc_result Zinc_get_user_app_directory(struct Zinc_string* lower_name, struct Zinc_string* dir);
ZINC_API enum Zinc_result Zinc_make_directory(struct Zinc_string* dir);
ZINC_API enum Zinc_result Zinc_delete_directory(struct Zinc_string* dir);
ZINC_API enum Zinc_result Zinc_get_dir_files(struct Zinc_string* dir, Zinc_string_list* bl);

#endif

#endif