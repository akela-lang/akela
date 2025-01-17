#ifndef APT_PARSE_H
#define APT_PARSE_H

#include <stdbool.h>

bool Apt_validate_directory(char* path);
void Apt_parse_files(char* dir_name);

#endif
