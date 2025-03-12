#ifndef APT_PARSE_H
#define APT_PARSE_H

#include <stdbool.h>
#include "data.h"

bool Apt_validate_directory(char* path);
void Apt_parse_files(Apt_data* data);

#endif
